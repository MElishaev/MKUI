#pragma once

namespace MKUI_Debug
{
    static void print(const FString& msg, int32 key = -1, const FColor& color = FColor::MakeRandomColor())
    {
        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(key, 7.f, color, msg);
            UE_LOG(LogTemp, Warning, TEXT("%s"), *msg);
        }
    }
}
