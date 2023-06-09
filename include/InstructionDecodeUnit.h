#pragma once
#include "systemc.h"
#include "reporting.h"
#include "report.h"


static const char* filename = "InstructionDecodeMgr.cpp";

class InstructionDecodeUnit : public sc_module
{
public:
	
	sc_in<sc_uint<32> > pInstrBus;

	sc_out<sc_uint<5> > pRegSrc1;
	sc_out<sc_uint<5> > pRegSrc2;
	sc_out<sc_uint<5> > pRegDest;

	sc_out<sc_uint<32> > pImm;
	sc_out<sc_uint<32> > pDataBus;
	sc_out<sc_uint<3> > pFunc3;
	sc_out<sc_uint<2> > pPCSrc;
	sc_out<sc_uint<7> > pOpCode;
	sc_out<bool> pRegWrite;
	sc_out<bool> pMemWrite;
	sc_out<sc_uint<8> > pAluOp;
	sc_out<bool> pMemToReg;
	sc_out<bool> pBranch;
	sc_out<bool> pIorD;
	sc_out<bool> pAluSrcA;
	sc_out<sc_uint<2> > pAluSrcB;
	sc_out<bool> pIRWrite;
	sc_out<bool> pRegDst;
	sc_out<bool> pDataLoaded;
	sc_in<bool> pClk;
	sc_in<bool> pReset;

	InstructionDecodeUnit(sc_module_name nm)
		:sc_module(nm)
		{
		SC_HAS_PROCESS(InstructionDecodeUnit);
		
		SC_CTHREAD(instructionDecodeThread, pClk.pos());
		reset_signal_is(pReset, true);
		

		pRegWrite.write(0);
		pAluSrcA.write(0);
		pAluSrcB.write(0);
		pIorD.write(0);
		pIRWrite.write(1);
		pBranch.write(0);
		pAluOp.write(0);
		pPCSrc.write(0);
		pRegSrc1.write(0);
		pRegSrc2.write(0);
		pRegDest.write(0);
		pImm.write(0);
		pFunc3.write(0);
		pPCSrc.write(0);
		pOpCode.write(0);
		
		pMemWrite.write(0);
		
		pMemToReg.write(0);
		pBranch.write(0);
		pIorD.write(0);
		pAluSrcA.write(0);
		pRegDst.write(0);

		currState = Fetch;
		nextState = Fetch;
		std::string ctrlLogFile = "./logs/InstructionDecodeMgr.log";
		mLogFileHandler.open(ctrlLogFile, std::fstream::trunc | std::fstream::out);
		
	}
	
private:
	
	//Main CTHREAD
	void instructionDecodeThread();

	
	/*void decodeSrcRegAddresses();
	void decodeDstRegAddress();
	uint8_t decodeFunct3();
	uint8_t decodeFunct7();*/

	void decodeLoadInstr(sc_uint<32> instr);
	void decodeStoreInstr(sc_uint<32> instr);
	void decodeImmInstr(sc_uint<32> instr);
	/*void decodeAluInstr(uint32_t instr);
	void decodeLuiInstr(uint32_t instr);
	void decodeBranchInstr(uint32_t instr);*/

    //helper functions
	sc_uint<7> decodeOpcode(sc_uint<32> instr);
	void loadDataToReg(sc_uint<3> instr);

	sc_uint<32> signExtendLoad(sc_uint<32> instr);
	sc_uint<32> signExtendStore(sc_uint<32> instr);
	sc_uint<32> signExtendImm(sc_uint<32> instr);

	void resetPorts();
	
	
	std::fstream mLogFileHandler;

	enum decodeState {
		Fetch,
		Decode,
		MemAddr,
		MemRead,
		RegWrBack,
		MemWrite,
		Execute
		
	};
    decodeState currState, nextState;
};