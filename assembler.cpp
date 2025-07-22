
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

#include "assembler.h"

const std::string asm_filename = "Add.asm";
const std::string hack_filename = "y.hack";
const bool verbose = true;

std::unordered_map<std::string, std::string> dest_map;
std::unordered_map<std::string, std::string> comp_map;
std::unordered_map<std::string, std::string> jump_map;


void initialize_tables(){
    dest_map["null"] =  "000";
    dest_map["M"] =     "001";
    dest_map["D"] =     "010";
    dest_map["MD"] =    "011";
    dest_map["A"] =     "100";
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
    comp_map["!A"] =    "110000";
    comp_map["D+1"] =   "011111";
    comp_map["A+1"] =   "110111";
    comp_map["D-1"] =   "001110";
    comp_map["A-1"] =   "110010";
    comp_map["D+A"] =   "000010";
    comp_map["D-A"] =   "010011";
    comp_map["A-D"] =   "000111";
    comp_map["D&A"] =   "000000";
    // std::cout << jump_map["JMP" ] << "\n"; // call like this
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

std::string decimal_to_binary_str(std::string m){
    int num = std::stoi(m);
    std::string s = std::bitset<15>(num).to_string();
    std::cout << "converting: " << m << "\n";
    std::cout << "to: " << s << "\n";
    return s;
}

std::string translate_dest(std::string m){
    
}

std::string translate_comp(std::string m){

}

std::string translate_jump(std::string m){

}

std::string get_symbol(std::string m){
    return "000000000000000";
}

std::string assemble_a_instruction(std::string m){
    std::cout << "a instruction \n";
    std::string prefix = "0";
    std::string value;
    m = m.substr(1, m.size());
    std::cout << m << "\n";
    if(is_number(m)){
        std::cout << "d \n";
        value = decimal_to_binary_str(m);
    } else {
        std::cout << "sdf\n";
        value = get_symbol(m);
    }

    return prefix+value;
}

std::string assemble_c_instruction(std::string m){
    std::string prefix = "111";
    
    // is a=1? A v M
    std::string a_bit = "0";
     if (m.find("M") != std::string::npos) {
        a_bit = "1";
        
        int i = m.find("M");
        m = m.substr(0, i)+"A"+m.substr(i+1, m.size());
    }
    //
    std::string comp = "000000";

    std::string dest = "000";

    std::string jump = "000";

    return prefix+a_bit+comp+dest+jump;
}

#ifndef TEST_MODE
int main(int argc, char *argv[]){

    initialize_tables();


    std::ifstream infile(asm_filename);
    // std::cout << "argc == " << argc << '\n';
    int i = 0;
    // while(1){
    //     i ++;
    //     std::string line;
    //     std::getline(std::cin, line);
    //     std::cout << "line";
    //     std::cout << "line:" << i << "\n";
    //     if (line == ""){break;}
    // }

    std::string line;
    while (std::getline(infile, line)){
        // std::istringstream iss(line);
        
        // clean input of anything unnecessary
        // commented lines, blank lines, whitespace, endline characters
        if(line.substr(0,2) == "//"){
            continue;
        }
        if (line.empty() || std::all_of(line.begin(), line.end(), is_whitespace_char)){
            continue;
        }
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        line = line.substr(0, line.size()-1);
        

        // determine type of command (@)
        std::string bytecode;
        if(line.substr(0,1) == "@"){
            bytecode = assemble_a_instruction(line);
        } else if(line.substr(1,2) == "@"){
            bytecode = assemble_c_instruction(line);
        } else {
            throw std::runtime_error("E: unknown instruction type");
        }

        if(!verbose){
            continue;
        }
        std::cout << line << "\n";
        std::cout << bytecode << "\n";
        std::cout << "l:" << line.size() << "\n";
        for (int j=0; j<line.size()-1; j++){
            std::cout << line[j] << "\n";
        }
        std::cout << "------\n"; 
    }




    return 0;
}
#endif