#include "InstructionDecodeUnit.h"

/** instructionDecodeThread
	* Clocked thread to decode instructions and
	* generate control signals based on specific 
	* instruction decoding FSM
	* short and long queue
	* @param 
	* @return void
	**/

void InstructionDecodeUnit::instructionDecodeThread()
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
			pRegWrite.write(0);
			pMemWrite.write(0);
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
		currState = nextState;
		wait();
	}
}


/** decodeLoadInstr
	* implement four states/stages of load instruction
	* FSM, 1st state/stage is Fetch, which is common
	* to all the instruction, remaining states are implemented 
	* in the function;Each state emits specific control signals
	* to enable subsequent stages of the pipeline
	* @param  instr
	* @return void
	**/
void InstructionDecodeUnit::decodeLoadInstr(sc_uint<32> instr)
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

/** decodeStoreInstr
	* Implements three states/stages of store instruction
	* FSM, 1st state/stage is Fetch, which is common
	* to all the instruction, remaining states are implemented 
	* in the function;Each state emits specific control signals
	* to enable subsequent stages of the pipeline
	* @param  instr
	* @return void
	**/
void InstructionDecodeUnit::decodeStoreInstr(sc_uint<32> instr)
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

/** decodeImmInstr
	* Implements four states/stages of immediate instructions
	* FSM, 1st state/stage is Fetch, which is common
	* to all the instruction, remaining states are implemented 
	* in the function;Each state emits specific control signals
	* to enable subsequent stages of the pipeline
	* @param  instr
	* @return void
	**/
void InstructionDecodeUnit::decodeImmInstr(sc_uint<32> instr)
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

//void InstructionDecodeUnit::decodeAluInstr(uint8_t* dataPtr, sc_time& delay)
//{
//
//}
//
//void InstructionDecodeUnit::decodeLuiInstr(uint8_t* dataPtr, sc_time& delay)
//{
//
//}
//
//void InstructionDecodeUnit::decodeBranchInstr(uint8_t* dataPtr, sc_time& delay)
//{
//
//}

sc_uint<32> InstructionDecodeUnit::signExtendLoad()
{
	sc_bit signBit = static_cast<sc_bit> (pInstrBus.read()[31]);
	auto immVal = pInstrBus.read().range(31, 20);
	return (signBit ? (0xFFFFF000 | immVal ) : immVal);
}
sc_uint<32> InstructionDecodeUnit::signExtendStore()
{
	sc_bit signBit = static_cast<sc_bit> (pInstrBus.read()[31]);
	auto immVal = (pInstrBus.read().range(31, 25) << 5) | pInstrBus.read().range(11,7);
	return (signBit ? (0xFFFFF000 | immVal) : immVal);
}

sc_uint<7> InstructionDecodeUnit::decodeOpcode()
{
	return pInstrBus.read().range(6, 0);
}

void InstructionDecodeUnit::loadDataToReg(sc_uint<3> instr)
{
	switch (instr)
	{
	case 0x0: //lb
	{
		pDataBus.write(pInstrBus.read() & 0xff);
		break;
	}
	case 0x1: //lh
	{
		pDataBus.write(pInstrBus.read() & 0xffff);
		break;
	}
	case 0x2://lw
	{
		pDataBus.write(pInstrBus.read());
		break;
	}

	}
}