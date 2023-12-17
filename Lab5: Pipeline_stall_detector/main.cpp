#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <cctype>
#include <fstream>

using namespace std;

bool isNumeric(const string& str) {
    int i=0;
    if(str[i]=='-') i++;

    for (; i<str.length(); i++) {
        if (!isdigit(str[i])) {
            return false;  
        }
    }
    return true;  
}

// this functions extracts rs, rd and instruction name from string instruction
unordered_map<string, vector<string>> parseInstruction(const string& instruction) {

    vector<string> operands;

    stringstream ss(instruction);
    string token;

    while (ss >> token) {
        if(token[token.length()-1]==',') token.pop_back();
        operands.push_back(token);
    }

    vector<string> instrName = {operands[0]};
    vector<string> destinationRegisters;

    vector<string> sourceRegisters;

    if(operands.size()==4){
        if(!isNumeric(operands[3])){
            sourceRegisters = {operands[2], operands[3]};
        }
        else{
            sourceRegisters = {operands[2]};
        }
        destinationRegisters = {operands[1]};
    }
    else if(operands.size()==3){ // for ld rd 0(rs)
        if(instrName[0] == "sd" || instrName[0] == "sh" || instrName[0] == "sw" || instrName[0] == "sb"){ // sd rs2, offset(rs1)
            int k=0; 
            string source = "";
            while(operands[2][k]!='(') k++;
            k++;
            while (operands[2][k]!=')'){
                source += operands[2][k];
                k++;
            }
            sourceRegisters = {source, operands[1]};
            destinationRegisters = {""};
        }
        else if(instrName[0] == "lui"){ // lui rd, imm
            sourceRegisters = {};
            destinationRegisters = {operands[1]};
        }
        else { // ld rd offset(rs)
            int k=0; 
            string source = "";
            while(operands[2][k]!='(') k++;
            k++;
            while (operands[2][k]!=')'){
                source += operands[2][k];
                k++;
            }
            sourceRegisters = {source};
            destinationRegisters = {operands[1]};
        }
    }
    return {{"instruction",instrName}, {"destinationRegisters",destinationRegisters}, {"sourceRegisters",sourceRegisters}};
}

// Assuming no data forwarding and hazard detection
vector<string> addNops(vector<string> &program){

    vector<string> curr_program;
    int i=0;
    for(string instruction: program){
        i = curr_program.size();
        unordered_map<string, vector<string>> parsedInstr = parseInstruction(instruction);

        vector<string> srcRegisters = parsedInstr["sourceRegisters"];

        int x = 0;
        if(i>=2 && curr_program[i-2] != "NOP"){
            int k = i-2;
            string to_be_written = parseInstruction(curr_program[k])["destinationRegisters"][0];

            for(string rs: srcRegisters){
                if(to_be_written==rs){
                    curr_program.push_back("NOP");
                    x++;
                    i++;
                    break;
                }
            }
        }
        if(i-x>=1 && curr_program[i-1-x] != "NOP"){
            int k = i-1-x;
            string to_be_written = parseInstruction(curr_program[k])["destinationRegisters"][0];
            for(string rs: srcRegisters){
                if(to_be_written==rs){
                    curr_program.push_back("NOP");
                    if(x==0) curr_program.push_back("NOP");
                    break;
                }
            }
        }
        curr_program.push_back(instruction);
    }

    return curr_program;
}

vector<string> addNopsDataForwarding(vector<string> &program){

    vector<string> curr_program;
    int i=0;
    for(string instruction: program){
        i = curr_program.size();
        unordered_map<string, vector<string>> parsedInstr = parseInstruction(instruction);

        vector<string> srcRegisters = parsedInstr["sourceRegisters"];

        if(i>=1 && curr_program[i-1] == "ld" ){
            int k = i-1;
            string to_be_written = parseInstruction(curr_program[k])["destinationRegisters"][0];
            for(string rs: srcRegisters){
                if(to_be_written==rs){
                    curr_program.push_back("NOP");
                    break;
                }
            }
        }
        curr_program.push_back(instruction);
    }

    return curr_program;
}


int main() {

    /*************** Input ************/

    vector<string> program; // Store the input assembly program

    ifstream file("input.txt"); 

    string line;
    while (getline(file, line)) {
        program.push_back(line);
    }

    file.close(); 

    /*************** end Input ************/

    cout<<"\nWithout data forwading: \n"<<endl;
   
    vector<string> finalProgram = addNops(program);
    int totalCycles = finalProgram.size() + 5 - 1;

    for (const string& ins : finalProgram) {
        cout<<ins<<endl;
    }
    cout << "Total: " << totalCycles << " cycles\n" << endl;

    cout<<"With data forwading: \n"<<endl;

    finalProgram = addNopsDataForwarding(program);
    totalCycles = finalProgram.size() + 5 - 1;

    for (const string& ins : finalProgram) {
        cout<<ins<<endl;
    }
    cout << "Total: " << totalCycles << " cycles\n" << endl;

    return 0;
}