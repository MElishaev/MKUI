// MAAKU Studio all rights reserved

#include "Widgets/MKUI_W_ActivatableBase.h"

APlayerController* UMKUI_W_ActivatableBase::getOwningPlayerController()
{
    if (!mCachedOwningPC.IsValid()) {
        mCachedOwningPC = GetOwningPlayer();
    }

    return mCachedOwningPC.IsValid() ? mCachedOwningPC.Get() : nullptr;
}
