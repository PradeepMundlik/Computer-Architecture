#include <bits/stdc++.h>

using namespace std;

vector < string > disassembledCode;
int i;
int num_labels;

string extractFirstWord(const string& input) {
    // Find the position of the first space character
    size_t pos = input.find(' ');

    // If a space is found, extract the substring up to that position
    if (pos != string::npos) {
        return input.substr(0, pos);
    } else {
        // If no space is found, the whole input is the first word
        return input;
    }
    return input;
}

string decodeR(const uint32_t & machineCode) {
    uint8_t rd = (machineCode >> 7) & 0x1F;
    uint8_t rs1 = (machineCode >> 15) & 0x1F;
    uint8_t rs2 = (machineCode >> 20) & 0x1F;
    uint8_t funct3 = (machineCode >> 12) & 0x7;
    uint8_t funct7 = (machineCode >> 25) & 0x7F;

    string ins = "";

    switch (funct3) {
    case 0x0:
        switch (funct7) {
        case 0x0:
            ins = "add";
            break;
        case 0x20:
            ins = "sub";
            break;
        }
        break;

    case 0x4:
        if (funct7 == 0x0)
            ins = "xor";
        break;

    case 0x6:
        if (funct7 == 0x0)
            ins = "or";
        break;

    case 0x7:
        if (funct7 == 0x0)
            ins = "and";
        break;

    case 0x5:
        if (funct7 == 0x0)
            ins = "srl";
        else if (funct7 == 0x20)
            ins = "sra";
        break;

    case 0x1:
        if (funct7 == 0x0)
            ins = "sll";
        break;

    case 0x2:
        if (funct7 == 0x0)
            ins = "slt";
        break;

    case 0x3:
        if (funct7 == 0x0)
            ins = "sltu";
        break;

    default:
        cout << "Invalid R instruction" << endl;
        break;
    }

    if (ins == "") {
        cout << "Invalid R instruction" << endl;
        return "";
    }

    string result = ins + " x" + to_string(rd) + " x" + to_string(rs1) + " x" + to_string(rs2);

    return result;
}

string decodeI(const uint32_t & machineCode) {
    uint8_t opcode = machineCode & 0x7F;
    uint8_t rd = (machineCode >> 7) & 0x1F;
    uint8_t rs1 = (machineCode >> 15) & 0x1F;
    uint8_t funct3 = (machineCode >> 12) & 0x7;
    uint32_t imm = (machineCode >> 20) & 0xFFF;
    uint8_t funct7 = (imm >> 6) & 0x3F;
    imm = (machineCode >> 20) & 0xFFF;

    string ins = "";
    string result = "";

    if (opcode == 0b0010011) {
        switch (funct3) {
        case 0x0:
            ins = "addi";
            break;

        case 0x4:
            ins = "xori";
            break;

        case 0x6:
            ins = "ori";
            break;

        case 0x7:
            ins = "andi";
            break;

        case 0x1:
            if (funct7 == 0x0)
                ins = "slli";
            break;

        case 0x5:
            if (funct7 == 0x00)
                ins = "srli";
            if (funct7 == 0x10)
                ins = "srai";
            break;

        case 0x2:
            ins = "slti";
            break;

        case 0x3:
            ins = "sltiu";
            break;

        default:
            cout << "Invalid I instruction" << endl;
            return "";
        }

        result = ins + " x" + to_string(rd) + " x" + to_string(rs1) + " " + to_string(imm);
    } 
    else if (opcode == 0b0000011) {
        switch (funct3) {
        case 0x0:
            ins = "lb";
            break;

        case 0x1:
            ins = "lh";
            break;

        case 0x2:
            ins = "lw";
            break;

        case 0x3:
            ins = "ld";
            break;

        case 0x4:
            ins = "lbu";
            break;

        case 0x5:
            ins = "lhu";
            break;

        case 0x6:
            ins = "lwu";
            break;

        default:
            cout << "Invalid I instruction" << endl;
            return "";
        }

        result = ins + " x" + to_string(rd) + " " + to_string(imm) + "(x" + to_string(rs1) + ")";
    } 
    else if (opcode == 0b1100111) {
        ins = "jalr";
        result = ins + " x" + to_string(rd) + " x" + to_string(rs1) + " " + to_string(imm);
    }

    if (ins == "") {
        cout << "Invalid I instruction" << endl;
        return "";
    }

    return result;
}

string decodeS(const uint32_t & machineCode) {
    uint8_t rs1 = (machineCode >> 15) & 0x1F;
    uint8_t rs2 = (machineCode >> 20) & 0x1F;
    uint8_t funct3 = (machineCode >> 12) & 0x7;
    uint32_t imm = ((machineCode >> 7) & 0x1F) + (((machineCode >> 25) & 0x7F) << 5);

    string ins = "";

    switch (funct3) {
    case 0x0:
        ins = "sb";
        break;

    case 0x1:
        ins = "sh";
        break;

    case 0x2:
        ins = "sw";
        break;

    case 0x3:
        ins = "sd";
        break;
    
    default:
            cout << "Invalid S instruction" << endl;
            return "";
    }

    string result = ins + " x" + to_string(rs2) + " " + to_string(imm) + "(" + "x" + to_string(rs1) + ")";

    return result;
}

pair<string, int> decodeB(const uint32_t & machineCode) {
    uint8_t rs1 = (machineCode >> 15) & 0x1F;
    uint8_t rs2 = (machineCode >> 20) & 0x1F;
    uint8_t funct3 = (machineCode >> 12) & 0x7;
    int32_t imm = ((machineCode >> 7) & 0x1F) + (machineCode >> 24) & 0xFF;

    string ins = "";

    switch (funct3) {
    case 0x0:
        ins = "beq";
        break;

    case 0x1:
        ins = "bne";
        break;

    case 0x4:
        ins = "blt";
        break;

    case 0x5:
        ins = "bge";
        break;

    case 0x6:
        ins = "bltu";
        break;

    case 0x7:
        ins = "bgeu";
        break;

    default:
            cout << "Invalid B instruction" << endl;
            return {"", 0};
    }



    string result = ins + " x" + to_string(rs1) + " x" + to_string(rs2);

    return {result, int(imm)/4};
}

string decodeU(const uint32_t & machineCode) {
    uint8_t opcode = machineCode & 0x7F;
    uint8_t rd = (machineCode >> 7) & 0x1F;
    uint8_t imm = (machineCode >> 12) & 0xFFFFF;

    string ins = "";
    string result = "";

    stringstream ss;
    ss << hex << machineCode;
    int l = ss.str().length();

    if (opcode == 0b0110111) {
        ins = "lui";
        result = ins + " x" + to_string(rd) + " 0x" + ss.str().substr(0, l - 3);
    }

    if (ins == "") {
        cout << "Invalid U instruction" << endl;
        return "";
    }

    return result;
}

pair<string, int> decodeJ(const uint32_t & machineCode) {
    uint8_t opcode = machineCode & 0x7F;
    uint8_t rd = (machineCode >> 7) & 0x1F;
    int32_t imm20 = (machineCode >> 31) & 0x1; // Bit 20 (the sign bit)
    int32_t imm10_1 = (machineCode >> 21) & 0x3FF; // Bits 10-1
    int32_t imm11 = (machineCode >> 20) & 0x1; // Bit 11
    int32_t imm19_12 = (machineCode >> 12) & 0xFF; // Bits 19-12
    int32_t imm = (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);

    string ins = "";

    ins = "jal";

    string result = ins + " x" + to_string(rd);

    return {result, imm/4};
}

// Function to decode a RISC-V instruction
void decodeInstruction(const uint32_t & machineCode) {

    // cout<<machineCode<<endl;
    uint8_t opcode = machineCode & 0x7F;
    uint8_t rd = (machineCode >> 7) & 0x1F;
    uint8_t rs1 = (machineCode >> 15) & 0x1F;
    uint8_t rs2 = (machineCode >> 20) & 0x1F;
    uint8_t funct3 = (machineCode >> 12) & 0x7;
    uint8_t funct7 = (machineCode >> 25) & 0x7F;

    string result = "";

    // Assuming 'opcode' is a variable containing the opcode value
    if (opcode == 0b0110011) {
        disassembledCode[i++] += decodeR(machineCode);
    } 
    else if (opcode == 0b0010011 || opcode == 0b0000011 || opcode == 0b1100111) {
        disassembledCode[i++] += decodeI(machineCode);
    } 
    else if (opcode == 0b1100011) {
        pair<string, int> res = decodeB(machineCode);
    
        int l = res.second;

        if (disassembledCode[i + l] == "") {
            disassembledCode[i + l] += "L" + to_string(num_labels++) + " : ";
            disassembledCode[i] += res.first + " " + "L" + to_string(num_labels-1);
            i++;
        }
        else {
            string label = extractFirstWord(disassembledCode[i+l]);
            disassembledCode[i] += res.first + " " + label;
            i++;
        }
    } 
    else if (opcode == 0b0100011) {
        disassembledCode[i++] += decodeS(machineCode);
    } 
    else if (opcode == 0b1101111) {
        pair<string, int> res = decodeJ(machineCode);
    
        int l = res.second;

        if (disassembledCode[i + l] == "") {
            disassembledCode[i + l] += "L" + to_string(num_labels++) + " : ";
            disassembledCode[i] += res.first + " " + "L" + to_string(num_labels-1);
            i++;
        }
        else {
            string label = extractFirstWord(disassembledCode[i+l]);
            disassembledCode[i] += res.first + " " + label;
            i++;
        }
    } 
    else if (opcode == 0b0110111 || opcode == 0b0010111) {
        disassembledCode[i++] += decodeU(machineCode);
    } 
    else {
        cout << "Invalid Machine Code" << endl;
    }

    return;
}

vector<uint32_t> loadData(const string &fname) {
    
    vector<uint32_t> machineCodeList;
    
    ifstream inputFile(fname);

    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string machineCode_S;

            while (ss >> machineCode_S) {
                // Convert the hexadecimal string to uint32_t
                uint32_t machineCode_hex;
                stringstream(machineCode_S) >> hex >> machineCode_hex;

                machineCodeList.push_back(machineCode_hex);
            }
        }

        inputFile.close();
    } else {
        cout << "Error"<< endl;
    }

    return machineCodeList;
}

int main() {

    vector < uint32_t > machineCodeList = loadData("data.txt");

    int n = machineCodeList.size();

    disassembledCode.resize(n, "");
    i = 0;
    num_labels = 0;

    for (const uint32_t & machineCode: machineCodeList) {
        decodeInstruction(machineCode);
    }

    for(const string & instruction: disassembledCode) {
        cout<<instruction<<endl;
    }

    return 0;
}