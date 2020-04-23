#include "chainparams.h"
#include "utilioprio.h"
#include "validation.h"
#include "validationinterface.h"
#include "net.h"
#include "config.h" // For GetConfig();

std::atomic<bool> fActivateChain(true);

void CConnman::ThreadValidation()
{
    {
    IOPRIO_IDLER(true);

    LogPrintf("%s: Starting\n", __func__);
    int nSleep = 0;
    while (!flagInterruptMsgProc) {
        if (fActivateChain && !fActivatingChain) {
            fActivateChain = false;
            FormBestChain(GetConfig());
        }
        if (!pindexBestHeader || pindexBestHeader->nChainWork == chainActive.Tip()->nChainWork)
            nSleep = 1000;
        else
            nSleep = 100;
        if (!fActivateChain)
            MilliSleep(nSleep);
        else
            nSleep = 0;
    }

    } // end IOPRIO_IDLER scope
}

