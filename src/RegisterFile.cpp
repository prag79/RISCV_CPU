#include "RegisterFile.h"

void RegisterFile::RegFileThread()
{
	if(pReset.read())
	{
		for (uint8_t i = 0; i < 32; i++)
			registers[i] = 0x0;
	}
	else {
	if (pRegWrite.read() == true)
	{
		if (pMemToReg.read() == true)
		{
			regData[pSrcRegAddr3.read()] = pInData.read();
		}
		else {
			regData[pSrcRegAddr3.read()] = pAluOut.read();
		}
	}
	else {
		pSrcRegData1.write(regData[pSrcRegAddr1.read()]);
		pSrcRegData2.write(regData[pSrcRegAddr2.read()]);
	}
	}
	
	wait();
}
