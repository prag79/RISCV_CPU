#include "ExecutionUnit.h"

void ExecutionUnit::ExecutionThread()
{
	while (true)
	{
		if(pReset.read()) {
			pAluOut.write(0);
			pZero.write(0);
			pAluOut.write(0);
		} 
		else 
		{
        pAluOut.write(mAluResult);
		mAluOut = mAluResult;
		switch (pAluOp.read())
		{
		case 0x0:
			if (pAluSrcA.read() == 0x0 && pAluSrcB.read() == 0x1) //PC = PC+4 Calculation
			{
				
					mAluResult = pInPC.read() + 4;
					//pAluOut.write(pInPC.read() + 4);
			}
			else if (pAluSrcA.read() == 0x1 && pAluSrcB.read() == 0x2) //Load/Store address offset or addi calculation
			{
				mAluResult = pSrcRegDataA.read() + pImm.read();
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
		} 
		wait();
	}
}

void ExecutionUnit::pcSourceMethod()
{

	switch(pPCSrc.read())
	{
		case 0x0:
		pPCout.write(mAluResult.read());
		break;
		case 0x1:
		pPCout.write(mAluOut);
		break;
		case 0x2:
		//PC calculation for jump instr
		break;
		
	}
	if(pPCSrc.read() == 0)
	{
		pPCout.write(mAluResult.read());
	} else if (pPCSrc.read()==1)
	{

	}
}