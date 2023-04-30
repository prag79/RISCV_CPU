#include "systemc.h"

SC_MODULE(Decode) {
    // Inputs
    sc_in<sc_uint<32>> instr;

    // Outputs
    sc_out<sc_uint<5>> rs1, rs2, rd;
    sc_out<sc_uint<12>> imm;
    sc_out<sc_uint<4>> opcode;

    // Decode logic
    void decode() {
        // Extract fields from instruction
        opcode.write(instr.read().range(6, 2));
        rd.write(instr.read().range(11, 7));
        rs1.write(instr.read().range(19, 15));
        rs2.write(instr.read().range(24, 20));
        imm.write(instr.read().range(31, 20));
    }

    // Constructor
    SC_CTOR(Decode) {
        SC_METHOD(decode);
        sensitive << instr;
    }
};