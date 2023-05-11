#include "InstructionFetchUnit.h"

void InstructionFetchUnit::pcUpdateThread()
{
    while(true)
    {
        if(pReset.read())
        {
            pc.write(0);
            
        } else {
            if(pIorD.read())
            {
                pc.write(pAluOut.read());
            }
             else {
            pc.write(pInPC.read());
            }
        }
        pOutPC.write(pc);
        wait();
    }
}
// Fetch logic
void InstructionFetchUnit::accessMemMethod() {
    
        
         pAddr.write(pc.read());
}

void InstructionFetchUnit::getInstrOrDataMethod()
{
        if (pDataValid.read()==true && pIRWrite.read() == true)
        {
            pDataLoaded.write(1);
            pOutInstr.write(pInMemData.read());
            
        }
        else if(pDataValid.read() == true && pIRWrite.read() == false)
        {
            pDataLoaded.write(1);
            pOutData.write(pInMemData.read());
            
        }
        else
        {
            pDataLoaded.write(0);
        }
}
    

}