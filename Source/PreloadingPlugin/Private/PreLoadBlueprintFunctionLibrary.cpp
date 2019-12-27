// Fill out your copyright notice in the Description page of Project Settings.


#include "PreLoadBlueprintFunctionLibrary.h"
#include "PreloadingBehavior.h"
#include "PreloadingSubsystem.h"
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
__pragma(optimize("", on))