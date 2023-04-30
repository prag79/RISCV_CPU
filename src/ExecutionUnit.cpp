#include "ExecutionUnit.h"

void ExecutionUnit::ExecutionThread()
{
	while (1)
	{
		/*switch (currState)
		{
		case Fetch:
			if (pDataLoaded.read())
			{
				if (pAluSrcA.read() == 0x0)
				{
					if (pAluSrcB.read() == 0x1 && pAluOp.read() == 0x0)
					{
						pAluOut.write(pInPC.read() + 4);
					}
				}
				nextState = Decode;
			}
			else
			{
				nextState = Fetch;
			}
			break;
		case Decode:
			nextState = Execute;
			break;
		case Execute:
			switch (pAluOp.read())
			{
			case 0x0:
			{

			}
			 }
		}*/

		switch (pAluOp.read())
		{
		case 0x0:
			if (pAluSrcA.read() == 0x0 && pAluSrcB.read() == 0x1) //PC = PC+4 Calculation
			{
				if (pDataLoaded.read() == 0x1)
					pAluOut.write(pInPC.read() + 4);
			}
			else if (pAluSrcA.read() == 0x1 && pAluSrcB.read() == 0x2) //Load/Store address offset or addi calculation
			{
				pAluOut.write(pSrcRegDataA.read() + pImm.read());
			} 
			else if (pAluSrcA.read() == 0x1 && pAluSrcB.read() == 0x0) //Reg Type addition
			{
			}
			else if (pAluSrcA.read() == 0x1 && pAluSrcB.read() == 0x3) //Branch offset calculation
			{

			}
			break;
		case 0x1:
			//do something, implementation defined internally
			break;
		}
		wait();
	}
}
