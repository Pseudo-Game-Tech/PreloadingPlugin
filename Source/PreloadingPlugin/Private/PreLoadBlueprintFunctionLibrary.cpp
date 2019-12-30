// Fill out your copyright notice in the Description page of Project Settings.


#include "PreLoadBlueprintFunctionLibrary.h"
#include "PreloadingBehavior.h"
#include "PreloadingSubsystem.h"
#include "AssetRegistryModule.h"
#include "Engine/AssetManager.h"

__pragma(optimize("", off))
void UPreLoadBlueprintFunctionLibrary::GeneratePreloadingBehaviorData(TArray<UObject*> PreloadingBehaviorList)
{
	for (auto Object : PreloadingBehaviorList)
	{
		UPreloadingBehavior* PreloadingBehavior = Cast<UPreloadingBehavior>(Cast<UBlueprint>(Object)->GeneratedClass->GetDefaultObject());
		if (PreloadingBehavior)
		{
			PreloadingBehavior->GenerateData();
			PreloadingBehavior->Modify();
		}
	}
}

void UPreLoadBlueprintFunctionLibrary::AddToPreloadDataTable(FName RowName, TArray<UObject*> AssetList, bool IsClear)
{
	UDataTable* PreloadDataTable = GetMutableDefault<UPreloadingSubsystemSettings>()->PreloadDataTable.LoadSynchronous();
	if (PreloadDataTable->GetRowStruct()->IsChildOf(FPreloadingData::StaticStruct()) && RowName != NAME_None && IsValid(PreloadDataTable))
	{
		FPreloadingData* PreloadingData = PreloadDataTable->FindRow<FPreloadingData>(RowName, TEXT(""), false);

		if (IsClear || PreloadingData == nullptr)
		{
			PreloadDataTable->AddRow(RowName, FPreloadingData());
		}

		PreloadingData = PreloadDataTable->FindRow<FPreloadingData>(RowName, TEXT("UPreLoadBlueprintFunctionLibrary::AddToPreloadDataTable PreloadDataTable->FindRow Error!!"));
		for (UObject* Asset : AssetList)
		{
			PreloadingData->Assets.AddUnique(Asset);
		}

		PreloadDataTable->Modify();
		if (PreloadDataTable->OnDataTableChanged().IsBound())
		{
			PreloadDataTable->OnDataTableChanged().Broadcast();
		}
	}
}

TArray<FSoftObjectPath> UPreLoadBlueprintFunctionLibrary::GatherShareDependenciesRecursively(const TArray<FSoftObjectPath> Assets, const float SharingRate)
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FSoftObjectPath> OutShareDependencies;

	TMap<FName, int32> AllShareDependencies;

	for (auto& Asset : Assets)
	{
		TSharedPtr<FStreamableHandle> Handle = UAssetManager::GetStreamableManager().RequestSyncLoad(Asset);

		TSet<FName> AllDependencies;
		TSet<FName> NowLayerAssets;
		NowLayerAssets.Add(FName(*Asset.GetLongPackageName()));

		while (NowLayerAssets.Num() > 0)
		{
			TSet<FName> NextLayerAssets;
			for (auto& NowAsset : NowLayerAssets)
			{
				TArray<FName> OutDependencies;
				AssetRegistry.GetDependencies(NowAsset, OutDependencies, EAssetRegistryDependencyType::Hard);

				auto RemoveNativePackage = [](const FAssetIdentifier& InAsset) { return InAsset.PackageName.ToString().StartsWith(TEXT("/Script")) && !InAsset.IsValue(); };
				OutDependencies.RemoveAll(RemoveNativePackage);

				for (auto& RefAsset : OutDependencies)
				{
					if (AllDependencies.Contains(RefAsset)) continue;

					AllDependencies.Add(RefAsset);
					NextLayerAssets.Add(RefAsset);
				}
			}

			NowLayerAssets.Reset();
			NowLayerAssets.Append(NextLayerAssets);
		}

		for (auto& Dependencies : AllDependencies)
		{
			AllShareDependencies.FindOrAdd(Dependencies) += 1;
		}

		Handle->ReleaseHandle();
	}

	float SharingRate2 = FMath::Clamp(SharingRate, 0.f, 1.f);

	for (auto Pair : AllShareDependencies)
	{
		int32 Count = Pair.Value;
		FName RefAsset = Pair.Key;
		if ((float)Count / Assets.Num() >= SharingRate2)
		{
			FString Path_L;
			FString Path_R;
			RefAsset.ToString().Split(TEXT("/"), &Path_L, &Path_R, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			FString Path = Path_L + TEXT("/") + Path_R + TEXT(".") + Path_R;
			OutShareDependencies.AddUnique(Path);
		}
	}

	return OutShareDependencies;
}
__pragma(optimize("", on))