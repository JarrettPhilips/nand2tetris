#ifndef ASSEMBLER_H
#define ASSEMBLER_H

void initialize_tables();
bool is_number(std::string m);

std::string assemble_a_instruction(std::string m);
std::string assemble_c_instruction(std::string m);

std::string handle_symbol(std::string key);
std::string handle_label(std::string key, int instruction_counter);

#endif