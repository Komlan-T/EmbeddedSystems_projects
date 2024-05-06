// On my honor, I have neither given nor received unauthorized aid on this assignment
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
using namespace std;

struct Register
{
    string name;
    int value;

    Register(){
        name = "";
        value = -1;
    }

    Register(string _name, int _value){
        name = _name;
        value = _value;
    }
};

struct Memory
{
    int address;
    int value;

    Memory(){
        address = -1;
        value = -1;
    }

    Memory(int _address, int _value){
        address = _address;
        value = _value;
    }
};

struct Instruction{
    string opcode;
    Register dest;
    Register src1;
    Register src2;

    Instruction(){
        opcode = "";
        dest = dest;
        src1 = src1;
        src2 = src2;
    }

    Instruction(string _opcode, Register _dest, Register _src1, Register _src2){
        opcode = _opcode;
        dest = _dest;
        src1 = _src1;
        src2 = _src2;
    }
};

struct Place{
    vector<Register> regs;
    vector<Memory> mem;
    vector<Instruction> insts;
};

struct Transition{
    Place input;
    Place output;

    Transition(){
        input = input;
        output = output;
    }

    Transition(Place _input, Place _output){
        input = _input;
        output = _output;
    }

    void read(){

    }
    void decode(){

    }
    void issue1(){

    }
    void issue2(){

    }
    void addr(){

    }
    void alu(){

    }
    void load(){

    }
    void write(){

    }
};

void read_files(Place& REG, Place& INM, Place& DAM){
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
            Register NR(register_name, register_value);
            REG.regs.push_back(NR);
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
            Register DR(destination_register, -1);
            Register FS(first_source_operand, -1);
            Register SS(second_source_operand, -1);
            Instruction NI(opcode, DR, FS, SS);
            INM.insts.push_back(NI);
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
            Memory NM(address, value);
            DAM.mem.push_back(NM);
        }
        file3.close();
    } else {
        cout << "Failed to open datamemory.txt";
    }
}

int main(){
    Place INM, INB, LIB, ADB, DAM, REB, AIB, RGF;
    read_files(RGF, INM, DAM);
    Transition READ(RGF, INB);
    Transition DECODE(INM, INB);
    Transition ISSUE1(INB, AIB);
    Transition ISSUE2(INB, LIB);
    Transition ADDR(LIB, ADB);
    Transition ALU(AIB, REB);
    Transition LOAD(DAM, REB);
    Transition WRITE(REB, RGF);
    return 0;
}