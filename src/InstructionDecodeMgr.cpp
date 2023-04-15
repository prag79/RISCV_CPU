#include "InstructionDecodeMgr.h"

//void InstructionDecoder::instructionDecodeMethod()
//{
//	auto opcode = decodeOpcode();
//	std::ostringstream msg;
//
//	switch (opcode)
//	{
//	case 0x3:
//	{
//		//load instructions
//		decodeLoadInstr();
//		msg.str("");
//		msg << "Load Instruction Decode Complete: ";
//		REPORT_INFO(filename, __FUNCTION__, msg.str());
//
//		mLogFileHandler << "Load Instruction Decode Complete "
//			<< "  @Time= " << dec << sc_time_stamp().to_double() << " ns"
//			<< endl;
//		break;
//	}
//	case 0x13:
//	{
//		//Immediate instructions
//		decodeImmInstr();
//		break;
//	}
//
//	case 0x33:
//	{
//		//Register Type Instructions
//		decodeRegInstr();
//		break;
//	}
//
//	case 0x63:
//	{   //Decode Branch Instructions
//		decodeBranchInstr();
//		break;
//	}
//
//	}
//}
void InstructionDecoder::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay)
{
	auto dataPtr = trans.get_data_ptr();
	auto addr = trans.get_address();
	auto length = trans.get_data_length();

	auto opcode = decodeOpcode();
	std::ostringstream msg;

	switch (opcode)
	{
	case 0x3:
	{
		//load instructions
		decodeLoadInstr();
		msg.str("");
		msg << "Load Instruction Decode Complete: ";
		REPORT_INFO(filename, __FUNCTION__, msg.str());

		mLogFileHandler << "Load Instruction Decode Complete "
			<< "  @Time= " << dec << sc_time_stamp().to_double() << " ns"
			<< endl;
		break;
	}
	case 0x13:
	{
		//Immediate instructions
		decodeImmInstr();
		break;
	}

	case 0x33:
	{
		//Register Type Instructions
		decodeRegInstr();
		break;
	}

	case 0x63:
	{   //Decode Branch Instructions
		decodeBranchInstr();
		break;
	}

	}

	trans.set_response_status(tlm::tlm_response_status::TLM_OK_RESPONSE);
}