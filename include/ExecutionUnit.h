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
	sc_in<sc_uint<2> > pPCSrc;
	sc_in<bool> pClk;
	sc_in<bool> pReset;
	sc_in<bool> pDataLoaded;
	
	//Output
	sc_out<sc_uint<32> > pAluOut;
	sc_out<sc_uint<32> > pPCout;
	sc_out<bool> pZero;

	ExecutionUnit(sc_module_name nm)
		:sc_module(nm)
	{
		SC_HAS_PROCESS(ExecutionUnit);

		SC_CTHREAD(ExecutionThread, pClk.pos());
		reset_signal_is(pReset, true);

		SC_METHOD(pcSourceMethod);
		sensitive << mAluResult << mAluOut << pPCSrc;
		
		pAluOut.write(0);
		pZero.write(0);
		mAluResult.write(0);
		//currState = nextState = Fetch;
	}
private:
	/* enum decodeState {
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
	}; */
	//decodeState currState, nextState;
    sc_signal<sc_uint<32> > mAluResult;
	sc_signal<sc_uint<32> > mAluOut;
	void ExecutionThread();
	void pcSourceMethod();
};
