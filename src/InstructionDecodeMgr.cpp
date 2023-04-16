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

	auto opcode = decodeOpcode(dataPtr);
	std::ostringstream msg;

	switch (opcode)
	{
	case 0x3:
	{
		//load instructions
		decodeLoadInstr(dataPtr, delay);
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
		decodeImmInstr(dataPtr, delay);
		break;
	}
	case 0x23:
	{
		//Store Instructions
		decodeStoreInstr(dataPtr, delay);
		break;
	}
	case 0x33:
	{
		//Alu Type Instructions
		decodeAluInstr(dataPtr, delay);
		break;
	}
	case 0x37:
	{
		//load upper imm instruction
		decodeLuiInstr(dataPtr, delay);
		break;
	}
	case 0x63:
	{   //Decode Branch Instructions
		decodeBranchInstr(dataPtr, delay);
		break;
	}


	}

	trans.set_response_status(tlm::tlm_response_status::TLM_OK_RESPONSE);
}

void InstructionDecoder::decodeLoadInstr(uint8_t* data, sc_time& delay)
{
	currState = S1;
	nextState = S1;
	while (currState != S4)
	{
		switch (currState)
		{
		case S1:
			nextState = S2;
			wait(delay);
			break;
		case S2:
			nextState = S3;
			wait(10);

			break;
		case S3:
			nextState = S4;
			wait(10);
			break;
		case S4:
			wait(10);
			break;
		}
		currState = nextState;

	}
}
void InstructionDecoder::decodeStoreInstr(uint8_t* data, sc_time& delay)
{
	currState = S1;
	nextState = S1;
	while (currState != S4)
	{
		switch (currState)
		{
		case S1:
			nextState = S2;
			wait(delay);
			break;
		case S2:
			nextState = S3;
			wait(10);

			break;
		case S3:
			nextState = S4;
			wait(10);
			break;
		case S4:
			wait(10);
			break;
		}
		currState = nextState;
	}
}
void InstructionDecoder::decodeImmInstr(uint8_t* dataPtr, sc_time& delay)
{

}

void InstructionDecoder::decodeAluInstr(uint8_t* dataPtr, sc_time& delay)
{

}

void InstructionDecoder::decodeLuiInstr(uint8_t* dataPtr, sc_time& delay)
{

}

void InstructionDecoder::decodeBranchInstr(uint8_t* dataPtr, sc_time& delay)
{

}