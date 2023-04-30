#include "systemc.h"

// Execute stage
SC_MODULE(Execute) {
    // Inputs
    sc_in<sc_uint<5>> rs1, rs2, rd;
    sc_in<sc_uint<12>> imm;
    sc_in<sc_uint<4>> opcode;
    sc_in<bool> branch, zero;

    // Outputs
    sc_out<sc_uint<32>> alu_result;
    sc_out<sc_uint<32>> pc_next;

    // Execute logic
    void execute() {
        sc_uint<32> result = 0;

        // Perform ALU operation based on opcode
        switch (opcode.read()) {
        case 0x0: // R-type
            switch (funct3) {
            case 0x0: // ADD
                result = rs1.read() + rs2.read();
                break;
            case 0x1: // SLL
                result = rs1.read() << rs2.read();
                break;
            case 0x2: // SLT
                result = (int)rs1.read() < (int)rs2.read();
                break;
            case 0x3: // SLTU
                result = rs1.read() < rs2.read();
                break;
            case 0x4: // XOR
                result = rs1.read() ^ rs2.read();
                break;
            case 0x5: // SRL
                result = rs1.read() >> rs2.read();
                break;
            case 0x6: // OR
                result = rs1.read() | rs2.read();
                break;
            case 0x7: // AND
                result = rs1.read() & rs2.read();
                break;
            }
            break;
        case 0x13: // I-type
            switch (funct3) {
            case 0x0: // ADDI
                result = rs1.read() + imm.read();
                break;
            case 0x1: // SLLI
                result = rs1.read() << imm.read();
                break;
            case 0x2: // SLTI
                result = (int)rs1.read() < (int)imm.read();
                break;
            case 0x3: // SLTIU
                result = rs1.read() < imm.read();
                break;
            case 0x4: // XORI
                result = rs1.read() ^ imm.read();
                break;
            case 0x5: // SRLI or SRAI
                if (imm.read()[5] == 0) { // SRLI
                    result = rs1.read() >> imm.read();
                }
                else { // SRAI
                    result = (int)rs1.read() >> imm.read();
                }
                break;
            case 0x6: // ORI
                result = rs1.read() | imm.read();
                break;
            case 0x7: // ANDI
                result = rs1.read() & imm.read();
                break;
            }
            break;
        case 0x17: // U-type
            result = imm.read();
            break;
        case 0x6f: // J-type
            result = pc.read() + 4;
            break;
        }

        // Check for branch instruction
        if (branch.read()) {
            if (zero.read()) {
                pc_next.write(pc.read() + imm.read());
            }
            else {
                pc_next.write(pc.read() + 4
