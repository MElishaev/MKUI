// MAAKU Studio all rights reserved


#include "Settings/MKUI_LoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UMKUI_LoadingScreenSettings::getLoadingScreenWidgetClassChecked() const
{
    checkf(!mSoftLoadingScreenWidgetClass.IsNull(), TEXT("No widget was assigned to the loading screen in project settings"));

    TSubclassOf<UUserWidget> loadedWidget = mSoftLoadingScreenWidgetClass.LoadSynchronous();
    return loadedWidget;
}

UDataTable* UMKUI_LoadingScreenSettings::getLoadingConditionsDataTable() const
{
    checkf(!mLoadingConditionsTable.IsNull(), TEXT("No loading conditions data table was set in loading screen project settings"));
    const auto dt = mLoadingConditionsTable.LoadSynchronous();
    return dt;    
}
