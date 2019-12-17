#include "PreloadingSubsystem.h"
#include "UObject/UObjectHash.h"
#include "Engine/AssetManager.h"
#include "Engine/Blueprint.h"
#include "Engine/GameEngine.h"
#include "Engine/World.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "AssetRegistryModule.h"
#include "ARFilter.h"
#include "Engine/ObjectLibrary.h"

//__pragma(optimize("", off))

UPreloadingBehavior* UPreloadingSubsystem::GetPreloadingBehavior(TSubclassOf<UPreloadingBehavior> Class)
{
	UPreloadingBehavior** Find = BehaviorMap.Find(Class);
	return Find ? *Find : nullptr;
}

void UPreloadingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked< FAssetRegistryModule >(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// ������ԴĿ¼
	AssetRegistry.ScanPathsSynchronous(ContentPaths);
	
	// ����PreloadingBehaviorBlueprint����
	{
		FName BaseClassName = UPreloadingBehavior::StaticClass()->GetFName();

		FARFilter Filter;
		Filter.ClassNames.Add(UPreloadingBehaviorBlueprint::StaticClass()->GetFName());
		for (auto& ContentPath : ContentPaths)
		{
			Filter.PackagePaths.Add(FName(*ContentPath));
		}
		Filter.bRecursiveClasses = true;
		Filter.bRecursivePaths = true;

		TArray< FAssetData > AssetList;
		AssetRegistry.GetAssets(Filter, AssetList);

		// ��������PreloadingBehaviorBlueprint�����ʲ�
		for (auto const& Asset : AssetList)
		{
			// ��ȡͨ����ͼ���ɳ��������·��
			const FAssetDataTagMapSharedView::FFindTagResult GeneratedClassValue = Asset.TagsAndValues.FindTag(TEXT("GeneratedClass"));
			if (GeneratedClassValue.IsSet())
			{
				// ������ͼ����
				TSoftClassPtr< UObject >(FStringAssetReference(FPackageName::ExportTextPathToObjectPath(GeneratedClassValue.GetValue()))).LoadSynchronous();
			}
		}
	}

	// �Զ�ע������UPreloadingBehavior����
	{
		TArray<UClass*> Classes;
		GetDerivedClasses(UPreloadingBehavior::StaticClass(), Classes, true);
		for (UClass* BehaviorClass : Classes)
		{
			if (!BehaviorClass->GetName().StartsWith(TEXT("SKEL_")) && !BehaviorClass->GetName().StartsWith(TEXT("REINST_")) && !BehaviorClass->HasAllClassFlags(CLASS_Abstract))
			{
				UPreloadingBehavior*& PreloadingBehavior = BehaviorMap.Add(BehaviorClass);
				PreloadingBehavior = NewObject<UPreloadingBehavior>(this, BehaviorClass);
			}
		}
	}
}
//__pragma(optimize("", on))