#include "systemc.h"

// Fetch stage
SC_MODULE(InstructionFetchUnit) {
public:
    // Inputs
    sc_in<sc_uint<32> > pInPC; //PC = PC+4 from ALU
    sc_in<sc_uint<32> > pAluOut; // PC calculated from branch, load and store instruction
    sc_in<sc_uint<32> > pInMemData; // Instr or Data from I/D Memory
    sc_in<bool> pIorD;
    sc_in<bool> pClk;
    sc_in<bool> pDataValid;
    sc_in<bool> pIRWrite;
    sc_in<bool> pReset;

    // Outputs
    sc_out<sc_uint<32> > pAddr;//Address of I/D Memory
    
    sc_out<sc_uint<32> > pOutInstr; //Next Instruction from I/D Memory
    sc_out<sc_uint<32> > pOutData; //RegWrite Back data from I/D Memory
    sc_out<sc_uint<32> > pOutPC; // PC value to be sent to ALU for increment
   
    sc_out<bool> pDataLoaded; //In case, I/D Memory takes multiple cycle

        // Constructor
    SC_CTOR(InstructionFetchUnit) {
        SC_CTHREAD(pcUpdateThread, pClk.pos());
        reset_signal_is(pReset, true);

        SC_METHOD(getInstrOrDataMethod);
        sensitive << pDataValid
    }

private:
    
    void getInstrOrDataMethod();
    void pcUpdateThread();
    
};