#pragma once
#include "systemc.h"

class ExecutionUnit : public sc_module
{
public:
	//Input
	sc_in<sc_uint<32> > pInPC;
	sc_in<sc_uint<32> > pSrcRegDataA;
	sc_in<sc_uint<32> > pSrcRegDataB;
	sc_in<bool> pAluSrcA;
	sc_in<sc_uint<2> > pAluSrcB;
	sc_in<sc_uint<32> > pImm;
	sc_in<sc_uint<8> > pAluOp;
	sc_in<sc_uint<7> > pOpCode;
	sc_in<sc_uint<3> > pFunc3;
	sc_in<bool> pClk;
	sc_in<bool> pDataLoaded;
	
	//Output
	sc_out<sc_uint<32> > pAluOut;
	sc_out<bool> pZero;

	ExecutionUnit(sc_module_name nm)
		:sc_module(nm)
	{
		SC_HAS_PROCESS(ExecutionUnit);

		SC_THREAD(ExecutionThread);
		sensitive << pClk.pos();
		
		pAluOut.write(0);
		pZero.write(0);
		mAluResult.write(0);
		currState = nextState = Fetch;
	}
private:
	enum decodeState {
		Fetch,
		Decode,
		MemAddr,
		MemRead,
		MemWrBack,
		MemWrite,
		Execute,
		//ALU_WriteBack,
		Branch,
		Jump
	};
	decodeState currState, nextState;
    sc_signal<sc_uint<32> > mAluResult;
	void ExecutionThread();
};
