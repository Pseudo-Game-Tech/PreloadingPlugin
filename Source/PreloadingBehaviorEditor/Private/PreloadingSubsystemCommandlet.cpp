#include "PreloadingSubsystemCommandlet.h"
#include "PreloadingSubsystem.h"
#include "FileHelpers.h"

//__pragma(optimize("", off))
int32 UPreloadingSubsystemCommandlet::Main(const FString& Params)
{
	UPreloadingSubsystem::LoadBlueprintClass();

	// 调用所有UPreloadingBehavior::GenerateData()
	{
		TArray<UClass*> Classes;
		TArray<UPackage*> Packages;
		GetDerivedClasses(UPreloadingBehavior::StaticClass(), Classes, true);
		for (UClass* BehaviorClass : Classes)
		{
			if (!BehaviorClass->GetName().StartsWith(TEXT("SKEL_")) && !BehaviorClass->GetName().StartsWith(TEXT("REINST_")) && !BehaviorClass->HasAllClassFlags(CLASS_Abstract))
			{
				UPreloadingBehavior* DefaultBehavior = Cast<UPreloadingBehavior>(BehaviorClass->GetDefaultObject());
				DefaultBehavior->GenerateData();
				Packages.AddUnique((UPackage*)DefaultBehavior->GetOuter());
			}
		}
		UEditorLoadingAndSavingUtils::SavePackages(Packages, false);
	}
	return int32();
}
//__pragma(optimize("", on))