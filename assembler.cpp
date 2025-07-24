/*
Hack Computer Assembler
*/

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm> // for std::all_of
#include <cctype>    // for std::isspace
#include <unordered_map>
#include <bitset>
#include <vector>

#include "assembler.h"

const std::string asm_filename = "Pong.asm";
const std::string hack_filename = "Pong.hack";
const bool verbose = true;

std::unordered_map<std::string, std::string> dest_map;
std::unordered_map<std::string, std::string> comp_map;
std::unordered_map<std::string, std::string> jump_map;
std::unordered_map<std::string, std::string> symbol_map;
int current_address_in_memory = 16;

std::string decimal_to_binary_str(std::string m){
    int num = std::stoi(m);
    std::string s = std::bitset<15>(num).to_string();
    std::cout << "converting: " << m << "\n";
    std::cout << "to: " << s << "\n";
    return s;
}

void initialize_tables(){
    dest_map["null"] =  "000";
    dest_map["M"] =     "001";
    dest_map["D"] =     "010";
    dest_map["MD"] =    "011";
    dest_map["A"] =     "100";
    dest_map["0"] =     "000";
    dest_map["AM"] =    "101";
    dest_map["AD"] =    "110";
    dest_map["AMD"] =   "111";
    
    jump_map["null"] =  "000";
    jump_map["JGT"] =   "001";
    jump_map["JEQ"] =   "010";
    jump_map["JGE"] =   "011";
    jump_map["JLT"] =   "100";
    jump_map["JNE"] =   "101";
    jump_map["JLE"] =   "110";
    jump_map["JMP"] =   "111";

    comp_map["0"] =     "101010";
    comp_map["1"] =     "111111";
    comp_map["-1"] =    "111010";
    comp_map["D"] =     "001100";
    comp_map["A"] =     "110000";
    comp_map["!D"] =    "001101";
    comp_map["!A"] =    "110001";
    comp_map["D+1"] =   "011111";
    comp_map["A+1"] =   "110111";
    comp_map["D-1"] =   "001110";
    comp_map["A-1"] =   "110010";
    comp_map["D+A"] =   "000010";
    comp_map["D-A"] =   "010011";
    comp_map["A-D"] =   "000111";
    comp_map["D&A"] =   "000000";
    comp_map["D|A"] =   "010101";

    symbol_map["SP"] =      decimal_to_binary_str("0");
    symbol_map["LCL"] =     decimal_to_binary_str("1");
    symbol_map["ARG"] =     decimal_to_binary_str("2");
    symbol_map["THIS"] =    decimal_to_binary_str("3");
    symbol_map["THAT"] =    decimal_to_binary_str("4");
    symbol_map["R0"] =      decimal_to_binary_str("0");
    symbol_map["R1"] =      decimal_to_binary_str("1");
    symbol_map["R2"] =      decimal_to_binary_str("2");
    symbol_map["R3"] =      decimal_to_binary_str("3");
    symbol_map["R4"] =      decimal_to_binary_str("4");
    symbol_map["R5"] =      decimal_to_binary_str("5");
    symbol_map["R6"] =      decimal_to_binary_str("6");
    symbol_map["R7"] =      decimal_to_binary_str("7");
    symbol_map["R8"] =      decimal_to_binary_str("8");
    symbol_map["R9"] =      decimal_to_binary_str("9");
    symbol_map["R10"] =     decimal_to_binary_str("10");
    symbol_map["R11"] =     decimal_to_binary_str("11");
    symbol_map["R12"] =     decimal_to_binary_str("12");
    symbol_map["R13"] =     decimal_to_binary_str("13");
    symbol_map["R14"] =     decimal_to_binary_str("14");
    symbol_map["R15"] =     decimal_to_binary_str("15");
    symbol_map["SCREEN"] =  decimal_to_binary_str("16384");
    symbol_map["KBD"] =     decimal_to_binary_str("24576");
}

bool is_substr_in_str(std::string m, std::string substr){
    bool substr_in_str;
    if (m.find(substr) != std::string::npos) {
        return true;
    }
    return false;
}

bool is_whitespace_char(char c){
    return std::isspace(static_cast<unsigned char>(c));
}

bool is_number(std::string m){
    bool is_int = true;
    int size = m.size();
    for(int i=0; i<size; i++){
        if(std::isdigit(m[i]) == 0){
            is_int = false;
            break;
        }
    }
    return is_int;
}

std::string clean_line(std::string line){
    // clean input of anything unnecessary
    // commented lines, blank lines, whitespace, endline characters
    if(line.substr(0,2) == "//"){
        line = "";
    }
    if(line.empty() || std::all_of(line.begin(), line.end(), is_whitespace_char)){
        line = ""; 
    }
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    line = line.substr(0, line.size()-1);
    return line;
}

void print_map(std::unordered_map<std::string, std::string> map){
    std::vector<std::string> keys;
    keys.reserve(map.size());
    std::vector<std::string> vals;
    vals.reserve(map.size());

    for(auto kv : map) {
        keys.push_back(kv.first);
        vals.push_back(kv.second);
        std::cout << "key:" << kv.first << " val:" << kv.second << "\n";
    } 
}

std::string handle_symbol(std::string key){
    // if(verbose == 1){
    //     print_map(symbol_map);
    //     std::cout << "key to handle:" << key << "\n";
    // }
    std::string value = symbol_map[key];
    if(value == ""){
        symbol_map[key] = decimal_to_binary_str(std::to_string(current_address_in_memory));
        current_address_in_memory ++;
        value = symbol_map[key];
    }
    return value;
}

std::string handle_label(std::string key, int instruction_counter){
    std::string value = symbol_map[key];
    if(value == ""){
        symbol_map[key] = decimal_to_binary_str(std::to_string(instruction_counter));
    }
    return value;
}

std::string assemble_a_instruction(std::string m){
    std::cout << "a instruction \n";
    std::string prefix = "0";
    std::string value;
    m = m.substr(1, m.size());
    if(int(m.find(")")) != -1){
        m = m.substr(1, m.size()-2);
    }
    std::cout << m << "\n";
    if(is_number(m)){
        std::cout << "d \n";
        value = decimal_to_binary_str(m);
    } else {
        std::cout << m << "\n";
        value = handle_symbol(m);
    }

    return prefix+value;
}

std::string assemble_c_instruction(std::string m){
    std::string prefix = "111";
    int action_index = int(m.find("="));
    if(action_index == -1){
        action_index = int(m.find(";"));
    }
    std::cout << action_index << "\n";
    std::string m_prefix = m.substr(0, action_index);
    std::string m_action = m.substr(action_index, 1);
    std::string m_suffix = m.substr(action_index+1, m.size());

    std::string a_bit = "0";
    std::string dest;
    std::string comp;
    std::string jump;
    
    std::cout << "action:" << m_action << " suffix:" << m_suffix << "\n";
    if(m_action == "="){
        // is a=1? A v M
        dest = dest_map[m_prefix];
        if(int(m_suffix.find("M")) != -1) {
            a_bit = "1";
            
            int i = int(m_suffix.find("M"));
            m_suffix = m_suffix.substr(0, i)+"A"+m_suffix.substr(i+1, m_suffix.size());
        }
        comp = comp_map[m_suffix];
        jump = "000";
    } else if(m_action == ";") {
        dest = "000";
        comp = comp_map[m_prefix];
        jump = jump_map[m_suffix];
    } else {
        std::cout << "E: unknown computation instruction type\n";
        std::cout << m << "\n";
        throw std::runtime_error("E: unknown computation instruction type");
    }
    std::cout << "prefix:" << prefix << " a:" << a_bit << " comp:" 
        << comp << " dest:" << dest << " jump:" << jump << "\n";
    return prefix+a_bit+comp+dest+jump;
}


#ifndef TEST_MODE
int main(int argc, char *argv[]){
    initialize_tables();
    
    std::ifstream first_pass(asm_filename);
    int i = 0;
    std::string line;
    while(std::getline(first_pass, line)){
        line = clean_line(line);
        if(line == ""){
            continue;
        } else if(int(line.find("(") != -1)){
            std::string label = line.substr(1, line.size()-2);
            handle_label(label, i);
        } else {
            i ++;
        }
    }

    std::ifstream second_pass(asm_filename);
    std::ofstream hack_file;
    hack_file.open(hack_filename);
    i = 0;
    while(std::getline(second_pass, line)){
        line = clean_line(line);
        if(line == "" || (int(line.find("(") != -1))){
            continue;
        }
        // determine type of command (@)
        std::string bytecode;
        if(int(line.find("@") != -1)){
            bytecode = assemble_a_instruction(line);
        } else if(int(line.find("=") != -1) || int(line.find(";") != -1)){
            bytecode = assemble_c_instruction(line);
        } else {
            std::cout << line << "\n";
            throw std::runtime_error("E: unknown instruction type");
        }
        hack_file << bytecode << "\n";


        if(!verbose){
            continue;
        }
        std::cout << line << "\n";
        std::cout << bytecode << "\n";
        std::cout << "l:" << line.size() << "\n";
        // for (int j=0; j<line.size()-1; j++){
        //     std::cout << line[j] << "\n";
        // }
        std::cout << "------\n"; 
    }

    hack_file.close();
    return 0;
}
#endif
