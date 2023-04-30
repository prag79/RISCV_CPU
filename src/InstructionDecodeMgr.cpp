#include "InstructionDecodeMgr.h"



void InstructionDecoder::instructionDecodeThread()
{
	while (1)
	{
		switch (currState)
		{
		case Fetch:
		{
			pIRWrite.write(1); //Enable Instruction Register to load the current instruction
			pIorD.write(0); //Enable Fetch Unit to load PC 
			pAluSrcA.write(0); //To ca
			pAluSrcB.write(1);
			if (pDataLoaded.read() == true)
				nextState = Decode;
			else
				nextState = Fetch;
			break;
		}
		case Decode:
			auto instr = pInstrBus.read();
			auto opcode = decodeOpcode();

			std::ostringstream msg;

			switch (opcode)
			{
			case 0x3:
			{
				//load instructions
				decodeLoadInstr(instr);
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
				decodeImmInstr(instr);
				break;
			}
			case 0x23:
			{
				//Store Instructions
				decodeStoreInstr(instr);
				break;
			}
			case 0x33:
			{
				//Alu Type Instructions
				//decodeAluInstr(instr);
				break;
			}
			case 0x37:
			{
				//load upper imm instruction
				//decodeLuiInstr(instr);
				break;
			}
			case 0x63:
			{   //Decode Branch Instructions
				//decodeBranchInstr(instr);
				break;
			}
			default:
			//	wait();
				break;
			}
			break;
		
		}
		wait();
	}
}
//void InstructionDecoder::b_transport(tlm::tlm_generic_payload& trans, sc_time& delay)
//{
//	auto dataPtr = trans.get_data_ptr();
//	auto addr = trans.get_address();
//	auto length = trans.get_data_length();
//
//	auto opcode = decodeOpcode(dataPtr);
//	std::ostringstream msg;
//
//	switch (opcode)
//	{
//	case 0x3:
//	{
//		//load instructions
//		decodeLoadInstr(dataPtr, delay);
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
//		decodeImmInstr(dataPtr, delay);
//		break;
//	}
//	case 0x23:
//	{
//		//Store Instructions
//		decodeStoreInstr(dataPtr, delay);
//		break;
//	}
//	case 0x33:
//	{
//		//Alu Type Instructions
//		decodeAluInstr(dataPtr, delay);
//		break;
//	}
//	case 0x37:
//	{
//		//load upper imm instruction
//		decodeLuiInstr(dataPtr, delay);
//		break;
//	}
//	case 0x63:
//	{   //Decode Branch Instructions
//		decodeBranchInstr(dataPtr, delay);
//		break;
//	}
//
//
//	}
//
//	trans.set_response_status(tlm::tlm_response_status::TLM_OK_RESPONSE);
//}

void InstructionDecoder::decodeLoadInstr(sc_uint<32> instr)
{
	
	sc_uint<32> immVal;
	sc_uint<3> func3;
	while (currState != Fetch)
	{
		switch (currState)
		{

		case Decode:
			pAluSrcA.write(0);
			pAluSrcB.write(0x0);
			pAluOp.write(0);
			pRegSrc1.write(pInstrBus.read().range(19, 15));
			immVal = signExtendLoad();
			pImm.write(immVal);
			pIRWrite.write(0);
			//decodefuncType();
			nextState = MemAddr;
			wait();
			break;
		case MemAddr:
			pAluSrcA.write(1);
			pAluSrcB.write(0x2);
			pOpCode.write(instr.range(6, 0));
			pAluOp.write(0x0);
			nextState = MemRead;
			wait();
			break;
		case MemRead:
			pIorD.write(1);
			if (pDataLoaded.read())
			{
				nextState = MemWrBack;
			}
			else {
				nextState = MemRead;
			}
			wait();
			break;
		case MemWrBack:
			pRegDest.write(instr.range(11, 7));
			pRegWrite.write(1);
			pMemToReg.write(1);
			func3 = instr.range(14, 12);
			loadDataToReg(func3);
			//pFunc3.write(instr.range(14,12));
			nextState = Fetch;
			break;
		default:
			pRegWrite.write(0);
			pAluSrcA.write(0);
			pAluSrcB.write(0);
			pIorD.write(0);
			pIRWrite.write(1);
			pBranch.write(0);
			pAluOp.write(0);
			break;
		}
		currState = nextState;
		//wait();
	}
}

void InstructionDecoder::decodeStoreInstr(sc_uint<32> instr)
{
	currState = Decode;
	nextState = Decode;
	sc_uint<32> immVal;
	while (currState != Fetch)
	{
		switch (currState)
		{

		case Decode:
			pAluSrcA.write(0);
			pAluSrcB.write(0x3);
			pAluOp.write(0);
			pRegSrc1.write(pInstrBus.read().range(19, 15));
			immVal = signExtendStore();
			pImm.write(immVal);
			//decodefuncType();
			nextState = MemAddr;
			break;
		case MemAddr:
			pAluSrcA.write(1);
			pAluSrcB.write(0x2);
			//pOpCode.write(instr.range(6, 0));
			pAluOp.write(0x0);
			nextState = MemWrite;
			break;
		case MemWrite:
			pIorD.write(1);
			pRegSrc2.write(instr.range(24, 20));
			pOpCode.write(instr.range(6, 0));
			nextState = Fetch;
			break;
		
		default:
			pRegWrite.write(0);
			pAluSrcA.write(0);
			pAluSrcB.write(0);
			pIorD.write(0);
			pIRWrite.write(0);
			pBranch.write(0);
			pAluOp.write(0);
			break;
		}
		currState = nextState;
		wait();
	}
}

void InstructionDecoder::decodeImmInstr(sc_uint<32> instr)
{
	currState = Decode;
	nextState = Decode;
	sc_uint<32> immVal;
	while (currState != Fetch)
	{
		switch (currState)
		{

		case Decode:
			pAluSrcA.write(0);
			pAluSrcB.write(0x3);
			pAluOp.write(0);
			pRegSrc1.write(pInstrBus.read().range(19, 15));
			
			pImm.write(pInstrBus.read().range(31,20));
			pFunc3.write(pInstrBus.read().range(14, 12));
			
			nextState = Execute;
			break;
		case Execute:
			pAluSrcA.write(1);

			pAluSrcB.write(0x2);
			pAluOp.write(0x0);
			nextState = MemRead;
			break;
		case MemRead:
			pIorD.write(1);
			nextState = MemWrBack;
			break;
		case MemWrBack:
			pRegDest.write(instr.range(11, 7));
			pRegWrite.write(1);
			pMemToReg.write(1);
			pFunc3.write(instr.range(14, 12));
			nextState = Fetch;
			break;
		default:
			pRegWrite.write(0);
			pAluSrcA.write(0);
			pAluSrcB.write(0);
			pIorD.write(0);
			pIRWrite.write(0);
			pBranch.write(0);
			pAluOp.write(0);
			break;
		}
		currState = nextState;
		wait();
	}
}

//void InstructionDecoder::decodeAluInstr(uint8_t* dataPtr, sc_time& delay)
//{
//
//}
//
//void InstructionDecoder::decodeLuiInstr(uint8_t* dataPtr, sc_time& delay)
//{
//
//}
//
//void InstructionDecoder::decodeBranchInstr(uint8_t* dataPtr, sc_time& delay)
//{
//
//}

sc_uint<32> InstructionDecoder::signExtendLoad()
{
	sc_bit signBit = static_cast<sc_bit> (pInstrBus.read()[31]);
	auto immVal = pInstrBus.read().range(31, 20);
	return (signBit ? (0xFFFFF000 | immVal ) : immVal);
}
sc_uint<32> InstructionDecoder::signExtendStore()
{
	sc_bit signBit = static_cast<sc_bit> (pInstrBus.read()[31]);
	auto immVal = (pInstrBus.read().range(31, 25) << 5) | pInstrBus.read().range(11,7);
	return (signBit ? (0xFFFFF000 | immVal) : immVal);
}

sc_uint<7> InstructionDecoder::decodeOpcode()
{
	return pInstrBus.read().range(6, 0);
}

void InstructionDecoder::loadDataToReg(sc_uint<3> instr)
{
	switch (instr)
	{
	case 0x0: //lb
	{
		pDataBus.write(pInstrBus.read() & 0xff);
		break;
	}
	case 0x1:
	{
		pDataBus.write(pInstrBus.read() & 0xffff);
		break;
	}
	case 0x2:
	{
		pDataBus.write(pInstrBus.read());
		break;
	}

	}
}