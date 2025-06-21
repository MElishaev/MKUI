// MAAKU Studio all rights reserved


#include "Widgets/MKUI_W_ActivatableBase.h"
#include "Controllers/MKUI_PlayerController.h"

AMKUI_PlayerController* UMKUI_W_ActivatableBase::getOwningPlayerController()
{
    if (!mCachedOwningPC.IsValid()) {
        mCachedOwningPC = GetOwningPlayer<AMKUI_PlayerController>();
    }

    return mCachedOwningPC.IsValid() ? mCachedOwningPC.Get() : nullptr;
}
