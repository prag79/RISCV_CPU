#include "RegisterFile.h"

void RegisterFile::RegFileThread()
{
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
	wait();
}
