#include "InstructionDecodeUnit.h"

/** instructionDecodeThread
	* Clocked thread to decode instructions and
	* generate control signals every clock cycle based on specific 
	* instruction decoding FSM to enable corresponding stages of the 
	* pipeline
	* @param 
	* @return void
	**/

void InstructionDecodeUnit::instructionDecodeThread()
{
	while (true)
	{
		if(pReset.read())
		{
           resetPorts();
		} 
		else {
		switch (currState)
		{
		case Fetch:
		{
			pIRWrite.write(1); //Enable Instruction Register to load the current instruction
			pIorD.write(0); //Enable Fetch Unit to load PC 
			pRegWrite.write(0);
			pMemWrite.write(0);
			if (pDataLoaded.read() == true) {
				nextState = Decode;
				pAluSrcA.write(0); //Enable loading current value of PC in ALU
			    pAluSrcB.write(1); //Enable offset value of 4 to calculate PC:=PC+4
				pAluOp.write(0); // Enable adder for calculating PC:PC+4 in the fetch stage only
			}
			else
				nextState = Fetch;
			
			break;
		}
		case Decode:
			sc_uint<32> instr = pInstrBus.read();
			sc_uint<7> opcode = decodeOpcode(instr);

			std::ostringstream msg;

			switch (opcode)
			{
			case 0x3:
			{
				decodeLoadInstr(instr); //decode load instruction and generate control signals
				break;
			}
			case 0x13:
			{

				decodeImmInstr(instr);  //decode Immediate instructions and generate control signals
				break;
			}
			case 0x23:
			{
			
				decodeStoreInstr(instr); //decode Store instructions and generate control signals
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
		}
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
		
			pRegSrc1.write(instr.range(19, 15)); //Write address if src1 register
			immVal = signExtendLoad(instr); //sign extend immediate field
			pImm.write(immVal); //Write the sign extended imm value and send it to ALU for addition
			nextState = MemAddr;
			wait();
			break;
		case MemAddr:
			pAluSrcA.write(1);
			pAluSrcB.write(0x2);
			//pOpCode.write(instr.range(6, 0));
			pAluOp.write(0x0);
			nextState = MemRead;
			wait();
			break;
		case MemRead:
			pIorD.write(1); //Enable memory address to come from ALU output
			if (pDataLoaded.read())
			{
				nextState = MemWrBack;
			}
			else {
				nextState = MemRead;
			}
			wait();
			break;
		case RegWrBack:
			pRegDest.write(instr.range(11, 7));//get the dest register index from instr 
			pRegWrite.write(1); //Enable Write Register file
			pMemToReg.write(1); //Enable data write from 
			func3 = instr.range(14, 12);
			loadDataToReg(func3);
			//pFunc3.write(instr.range(14,12));
			nextState = Fetch;
			break;
		default:
			
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
	* to enable corresponding stages of the pipeline
	* @param  instr
	* @return void
	**/
void InstructionDecodeUnit::decodeStoreInstr(sc_uint<32> instr)
{
	
	sc_uint<32> immVal;
	while (currState != Fetch)
	{
		switch (currState)
		{

		case Decode:
			
			pRegSrc1.write(instr.range(19, 15));
			immVal = signExtendStore();
			pImm.write(immVal);
			//decodefuncType();
			nextState = MemAddr;
			wait();
			break;
		case MemAddr:
			pAluSrcA.write(1);
			pAluSrcB.write(0x2);
			//pOpCode.write(instr.range(6, 0));
			pAluOp.write(0x0);
			nextState = MemWrite;
			wait();
			break;
		case MemWrite:
			pIorD.write(1);
			pRegSrc2.write(instr.range(24, 20));
			//pOpCode.write(instr.range(6, 0));
			pMemWrite.write(1);
			nextState = Fetch;
			break;
		
		default:
			
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
	* to enable corresponding stages of the pipeline
	* @param  instr
	* @return void
	**/
void InstructionDecodeUnit::decodeImmInstr(sc_uint<32> instr)
{
	
	sc_uint<32> immVal;
	while (currState != Fetch)
	{
		switch (currState)
		{

		case Decode:
			
			pRegSrc1.write(pInstrBus.read().range(19, 15));
			immVal = signExtendImm();
			pImm.write(immVal);
			pFunc3.write(instr.range(14, 12));
			
			nextState = Execute;
			break;
		case Execute:
			pAluSrcA.write(1);

			pAluSrcB.write(0x2);
			pAluOp.write(0x0);
			nextState = RegWrBack;
			break;
		case RegWrBack:
			pRegDest.write(instr.range(11, 7));
			pRegWrite.write(1);
			pMemToReg.write(1);
			pFunc3.write(instr.range(14, 12));
			nextState = Fetch;
			break;
		default:
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

sc_uint<32> InstructionDecodeUnit::signExtendLoad(sc_uint<32> instr)
{
	sc_bit signBit = static_cast<sc_bit> (instr[31]);
	auto immVal = instr.range(31, 20);
	return (signBit ? (0xFFFFF000 | immVal ) : immVal);
}
sc_uint<32> InstructionDecodeUnit::signExtendStore(sc_uint<32> instr)
{
	sc_bit signBit = static_cast<sc_bit> (instr[31]);
	auto immVal = (instr.range(31, 25) << 5) | instr.range(11,7);
	return (signBit ? (0xFFFFF000 | immVal) : immVal);
}

/** signExtendImm
	* Not complete
	* 
	* @param  instr
	* @return sc_uint<32>
	**/
sc_uint<32> InstructionDecodeUnit::signExtendImm(sc_uint<32> instr)
{
	sc_bit signBit = static_cast<sc_bit> (instr[31]);
	auto immVal = instr.range(31, 20);
	return (signBit ? (0xFFFFF000 | immVal ) : immVal);
}
sc_uint<7> InstructionDecodeUnit::decodeOpcode(sc_uint<32> instr)
{
	return instr.range(6,0);
}

void InstructionDecodeUnit::loadDataToReg(sc_uint<3> instr)
{
	switch (instr)
	{
	case 0x0: //lb
	{
		pDataBus.write(pDataBus.read() & 0xff);
		break;
	}
	case 0x1: //lh
	{
		pDataBus.write(pDataBus.read() & 0xffff);
		break;
	}
	case 0x2://lw
	{
		pDataBus.write(pDataBus.read());
		break;
	}

	}
}

void InstructionDecodeUnit::resetPorts()
	{
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
	}