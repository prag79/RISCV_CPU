#include "ÏnstructionFetchUnit.h"

// Fetch logic
void InstructionFetchUnit::fetchThread() {
    while (1)
    {
        if (pIorD.read() == 0)
        {
            pAddr.write(pInPC.read());

        }
        else {
            pAddr.write(pAluOut.read());
        }

        if (pDataValid.read()==true && pIRWrite.read() == true)
        {
            pDataLoaded.write(1);
            pOutInstr.write(pInMemData.read());
            pOutPC.write(pInPC.read());
        }
        else if(pDataValid.read() == true && pIRWrite.read() == false)
        {
            pDataLoaded.write(1);
            pOutData.write(pInMemData.read());
            pOutPC.write(pInPC.read());
        }
        else
        {
            pDataLoaded.write(0);
        }

        wait();
    }

}