#include "InstructionFetchUnit.h"

void InstructionFetchUnit::pcUpdateThread()
{
    while(true)
    {
        sc_uint<32> pc;
        if(pReset.read())
        {
            pAddr.write(0);
            pOutInstr.write(0);
            pOutData.write(0);

        } else {
            if(pIorD.read())
            {
                pc.write(pAluOut.read());
                pAddr.write(pc.read());
            }
             else {
                pc.write(pInPC.read());
                pAddr.write(pc.read());
            }
        }
        pOutPC.write(pc.read());
        wait();
    }
}


void InstructionFetchUnit::getInstrOrDataMethod()
{
        if (pDataValid.read()==true && pIRWrite.read() == true)
        {
            pDataLoaded.write(true);
            pOutInstr.write(pInMemData.read());
            
        }
        else if(pDataValid.read() == true && pIRWrite.read() == false)
        {
            pDataLoaded.write(true);
            pOutData.write(pInMemData.read());
            
        }
        else
        {
            pDataLoaded.write(false);
        }
}
    

