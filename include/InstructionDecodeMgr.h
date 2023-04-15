#pragma once
#include "systemc.h"
#include "tlm.h"
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/peq_with_get.h"
#include "reporting.h"
#include "report.h"
#define INSTRUCTION_WIDTH 32
#define REG_FILE_ADDR_WIDTH 5

static const char* filename = "InstructionDecodeMgr.cpp";

class InstructionDecoder : public sc_module
{
public:
	//tlm_utils::simple_target_socket<InstructionDecoder, INSTRUCTION_WIDTH, tlm::tlm_base_protocol_types> pInstrBus;
	sc_in<uint32_t> pInstrBus;
	sc_out<uint8_t> pRegAddr0;
	sc_out<uint8_t> pRegAddr1;
	sc_out<uint8_t> pRegAddr2;
	sc_out<bool> pRegWrite;
	sc_out<bool> pAluSrc;
	sc_out<bool> pMemWrite;
	sc_out<bool> pAluOp;
	sc_out<bool> pMemToReg;
	sc_out<bool> pBranch;

	InstructionDecoder(sc_module_name nm, uint8_t instrWidth)
		:sc_module(nm)
		, mInstrWidth(instrWidth)
	{
		SC_HAS_PROCESS(InstructionDecoder);
		
		SC_METHOD(instructionDecodeMethod);
		sensitive << pInstrBus;
		dont_initialize();

		std::string ctrlLogFile = "./logs/InstructionDecodeMgr.log";
		mLogFileHandler.open(ctrlLogFile, std::fstream::trunc | std::fstream::out);
	}
private:
	uint8_t decodeOpcode();
	void decodeSrcRegAddresses();
	void decodeDstRegAddress();
	uint8_t decodeFunct3();
	uint8_t decodeFunct7();
	void decodeLoadInstr();
	void decodeImmInstr();
	void decodeRegInstr();
	void decodeBranchInstr();

	void instructionDecodeMethod();
	uint8_t mInstrWidth;
	std::fstream mLogFileHandler;
	
};