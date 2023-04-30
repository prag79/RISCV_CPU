#include "systemc.h"

// Fetch stage
SC_MODULE(Fetch) {
    // Inputs
    sc_in<sc_uint<32>> pc;

    // Outputs
    sc_out<sc_uint<32>> instr;
    sc_out<sc_uint<32>> pc_next;

    // Fetch logic
    void fetch() {
        // Read instruction from memory
        instr.write(memory[pc.read()]);

        // Increment program counter
        pc_next.write(pc.read() + 4);
    }

    // Constructor
    SC_CTOR(Fetch) {
        SC_METHOD(fetch);
        sensitive << pc;
    }

    // Memory array
    sc_uint<32> memory[1024];
};