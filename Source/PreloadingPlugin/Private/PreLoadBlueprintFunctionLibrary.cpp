// Fill out your copyright notice in the Description page of Project Settings.


#include "PreLoadBlueprintFunctionLibrary.h"
#include "PreloadingBehavior.h"

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
