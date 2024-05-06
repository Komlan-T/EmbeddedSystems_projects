// On my honor, I have neither given nor received unauthorized aid on this assignment
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
using namespace std;

struct Instruction {

    string OP;
    pair<string, int> DR;
    pair<string, int> FSO;
    pair<string, int> SSO;
};

void read_files(vector<Instruction>& IM, vector<pair<string, int>>& RF, vector<pair<int, int>>& DM){
    ifstream file1("registers.txt");
    ifstream file2("instructions.txt");
    ifstream file3("datamemory.txt");
    string line;

    if(file1.is_open()){
        while(getline(file1, line)){
            stringstream tokens(line);
            string info;
            string register_name;
            string temp_register_value;
            getline(tokens, info, '<');
            getline(tokens, info, ',');
            register_name = info;
            getline(tokens, info, '>');
            temp_register_value = info;
            int register_value = stoi(temp_register_value);
            pair<string, int> newRegister(register_name, register_value);
            RF.push_back(newRegister);
        }
        file1.close();
    } else {
        cout << "Failed to open registers.txt";
    }

    if(file2.is_open()){
        while(getline(file2, line)){
            stringstream tokens(line);
            string info;
            string opcode;
            string destination_register;
            string first_source_operand;
            string second_source_operand;
            getline(tokens, info, '<');
            getline(tokens, info, ',');
            opcode = info;
            getline(tokens, info, ',');
            destination_register = info;
            getline(tokens, info, ',');
            first_source_operand = info;
            getline(tokens, info, '>');
            second_source_operand = info;
            pair<string, int> _DR(destination_register, 0);
            pair<string, int> _FSO(first_source_operand, 0);
            pair<string, int> _SSO(second_source_operand, 0);
            Instruction newInstruction;
            newInstruction.OP = opcode;
            newInstruction.DR = _DR;
            newInstruction.FSO = _FSO;
            newInstruction.SSO = _SSO;
            IM.push_back(newInstruction);
        }
        file2.close();
    } else {
        cout << "Failed to open instructions.txt";
    }

    if(file3.is_open()){
        while(getline(file3, line)){
            stringstream tokens(line);
            string info;
            string temp_address;
            string temp_value;
            getline(tokens, info, '<');
            getline(tokens, info, ',');
            temp_address = info;
            getline(tokens, info, '>');
            temp_value = info;
            int address = stoi(temp_address);
            int value = stoi(temp_value);
            pair<int, int> newRegister(address, value);
            DM.push_back(newRegister);
        }
        file3.close();
    } else {
        cout << "Failed to open datamemory.txt";
    }
}

void READ(vector<pair<string, int>> registers, Instruction& instruction){
    
    bool parameter_1_Present = false;
    bool parameter_2_Present = false;
    int value1 = 0;
    int value2 = 0;
    pair<string, int> register1 = instruction.FSO;
    pair<string, int> register2 = instruction.SSO;
    string reg1Name = register1.first;
    string reg2Name = register2.first;
    for(int i = 0; i < registers.size(); i++){
        if(registers[i].first == reg1Name){
            parameter_1_Present = true;
            value1 = registers[i].second;
        }
        if(registers[i].first == reg2Name){
            parameter_2_Present = true;
            value2 = registers[i].second;
        }
    }

    if(parameter_1_Present && parameter_2_Present){
        instruction.FSO.second = value1;
        instruction.SSO.second = value2;
    }
}

void DECODE(Instruction& instruction, vector<Instruction>& AIB, vector<Instruction>& LIB, vector<pair<int, int>> DM){
    
    string opcode = instruction.OP;
    int index = 0;
    if(opcode == "ADD"){
        instruction.DR.second = instruction.FSO.second + instruction.SSO.second;
        AIB.push_back(instruction);
    }
    if(opcode == "SUB"){
        instruction.DR.second = instruction.FSO.second - instruction.SSO.second;
        AIB.push_back(instruction);
    }
    if(opcode == "AND"){
        instruction.DR.second = instruction.FSO.second & instruction.SSO.second;
        AIB.push_back(instruction);
    }
    if(opcode == "OR"){
        instruction.DR.second = instruction.FSO.second | instruction.SSO.second;
        AIB.push_back(instruction);
    }
    if(opcode == "LD"){
        index = instruction.FSO.second + instruction.SSO.second;
        for(int i = 0; i < DM.size(); i++){
            if(DM[i].first == index){
                instruction.DR.second = DM[i].second;
                LIB.push_back(instruction);
            }
        }
    }
}

void REB(vector<pair<pair<string, int>, int>> RB){
    for(int i = 0; i < RB.size(); i++){
        for(int j = i + 1; j < RB.size(); j++){
            if(RB[i].second == RB[j].second){
                //cout << "<" << RB[j].first.first << "," << RB[j].first.second << ">,<" << RB[i].first.first << "," << RB[i].first.second << ">";
                cout << "\n";
            } else {
                cout << "<" << RB[i].first.first << "," << RB[i].first.second << ">";
                if(j < RB.size() - 1){
                    cout << ",";
                }
            }
            break;
        }
    }
    cout << "\n";
}

int main(){

    vector<Instruction> instruction_memory;
    Instruction instruction_buffer;
    vector<Instruction> arithmetic_instruction_buffer;
    vector<Instruction> load_instruction_buffer;
    pair<string, int> address_buffer;
    vector<pair<pair<string, int>, int>> result_buffer;
    vector<pair<string, int>> register_file;
    vector<pair<int, int>> data_memory;
    read_files(instruction_memory, register_file, data_memory);
    
    for(int i = 0; i < instruction_memory.size(); i++){
        READ(register_file, instruction_memory[i]);
        DECODE(instruction_memory[i], arithmetic_instruction_buffer, load_instruction_buffer, data_memory);
    }

    int j = 0;
    bool arithBuff = false;
    bool loadBuff = false;
    bool adL = false;
    bool lastLoad = false;
    int sum = 0;
    bool present_1 = false;
    bool present_2 = false;
    pair<string, int> temp2;
    pair<string, int> temp6;
    pair<string, int> temp8;

    while(j < 15){
        cout << "STEP " << j << ": \n";


        cout << "INM:";
        Instruction temp;
        int _sum = 0;
        if(instruction_memory.size() != 0){
            for(int i = 0; i < instruction_memory.size(); i++){
                cout << "<" << instruction_memory[i].OP << "," << instruction_memory[i].DR.first << "," << instruction_memory[i].FSO.first << "," << instruction_memory[i].SSO.first << ">";
                if(i < instruction_memory.size() - 1){
                    cout << ",";
                }
            }
            cout << "\n";
            temp = instruction_memory[0];
            instruction_memory.erase(instruction_memory.begin());
        } else {
            cout << "\n";
        }

        cout << "INB:";
        if(!instruction_buffer.OP.empty()){
            if(instruction_buffer.OP != "LD"){
                arithBuff = false;
            } else {
                loadBuff = false;
            }
            cout << "<" << instruction_buffer.OP << "," << instruction_buffer.DR.first << "," << instruction_buffer.FSO.second << "," << instruction_buffer.SSO.second << ">\n";
            if(instruction_memory.size() == 0){
                instruction_buffer.OP.clear();
            }
        } else {
            cout << "\n";
        }
        instruction_buffer = temp;

        cout << "AIB:";
        if (arithBuff && arithmetic_instruction_buffer.size() != 0){
            cout << "<" << arithmetic_instruction_buffer[0].OP << "," << arithmetic_instruction_buffer[0].DR.first << "," << arithmetic_instruction_buffer[0].FSO.second << "," << arithmetic_instruction_buffer[0].SSO.second << ">\n";
            temp2.first = arithmetic_instruction_buffer[0].DR.first;
            temp2.second = arithmetic_instruction_buffer[0].DR.second;
            int iteration = j;
            pair<pair<string, int>, int> x(temp2, iteration);
            result_buffer.push_back(x);
            arithmetic_instruction_buffer.erase(arithmetic_instruction_buffer.begin());
        } else {
            cout << "\n";
        }
        arithBuff = !arithBuff;

        cout << "LIB:";
        if(loadBuff && load_instruction_buffer.size() != 0 && j > 2){
            cout << "<" << load_instruction_buffer[0].OP << "," << load_instruction_buffer[0].DR.first << "," << load_instruction_buffer[0].FSO.second << "," << load_instruction_buffer[0].SSO.second << ">\n";
            address_buffer.first = load_instruction_buffer[0].DR.first; 
            address_buffer.second = load_instruction_buffer[0].DR.second;
            int iteration = j;
            pair<pair<string, int>, int> x(address_buffer, iteration);
            result_buffer.push_back(x);
            sum = load_instruction_buffer[0].FSO.second + load_instruction_buffer[0].SSO.second;
            load_instruction_buffer.erase(load_instruction_buffer.begin());
            adL = false;
        } else{
            cout << "\n";
        }
        loadBuff = !loadBuff;
        
        cout << "ADB:";
        if(adL){
            if(!address_buffer.first.empty()){
                cout << "<" << address_buffer.first << "," << sum << ">\n";
            } else {
                cout << "\n";
            }
        } else {
            cout << "\n";
        }
        adL = !adL;
        if(lastLoad){
            address_buffer.first = "";
        }
        if(load_instruction_buffer.size() == 0){
            lastLoad = true;
        }


        cout << "REB:";
        if(j <= 1 || result_buffer.size() == 0) {
            cout << "\n";
        }
        if(j > 1 && j % 2 != 0){
            REB(result_buffer);
        }
        if(j > 2 && j % 2 == 0 && result_buffer.size() != 0){
            result_buffer.erase(result_buffer.begin());
            cout << "\n";
        }

        
        cout << "RGF:";
        for(int a = 0; a < register_file.size(); a++){
            cout << "<" << register_file[a].first << "," << register_file[a].second << ">";
            if(a < register_file.size() - 1){
                cout << ",";
            }
        }
        cout << "\n";


        cout << "DAM:";
        for(int a = 0; a < data_memory.size(); a++){
            cout << "<" << data_memory[a].first << "," << data_memory[a].second << ">";
            if(a < data_memory.size() - 1){
                cout << ",";
            }
        }
        cout << "\n" << endl;
        j++;
    }

    return 0;
}