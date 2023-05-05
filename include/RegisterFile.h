#pragma once
#include "systemc.h"
#include <vector>
using namespace std;
class RegisterFile : public sc_module
{
public:
	//input
	sc_in<sc_uint<5> > pSrcRegAddr1;
	sc_in<sc_uint<5> > pSrcRegAddr2;
	sc_in<sc_uint<5> > pSrcRegAddr3;
	sc_in<sc_uint<32> > pAluOut;
	sc_in<sc_uint<32> > pInData;
	sc_in<bool> pRegWrite;
	sc_in<bool> pMemToReg;
	sc_in<bool> pClk;
	sc_in<bool> pReset;
	//output
	sc_out<sc_uint<32> > pSrcRegData1;
	sc_out<sc_uint<32> > pSrcRegData2;

	RegisterFile(sc_module_name nm)
		:sc_module(nm)
	{
		SC_HAS_PROCESS(RegisterFile);

		SC_CTHREAD(RegFileThread, pClk.pos());
		reset_signal_is(pReset, true`);
		
		pSrcRegData1.write(0x0);
		pSrcRegData2.write(0x0);

		for (uint8_t i = 0; i < 32; i++)
			registers.push_back(0);

	}
private:
	void RegFileThread();
	vector<uint32_t> registers;
};