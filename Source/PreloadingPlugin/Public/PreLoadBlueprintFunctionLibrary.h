// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetData.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PreLoadBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PRELOADINGPLUGIN_API UPreLoadBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "PreloadingTool")
	static void GeneratePreloadingBehaviorData(TArray<UObject*> PreloadingBehaviorList);

	UFUNCTION(BlueprintCallable, Category = "PreloadingTool")
	static void AddToPreloadDataTable(FName RowName, TArray<UObject*> AssetList, bool IsClear);

	// Depth深度参数-1为无限深度
	UFUNCTION(BlueprintCallable, Category = "PreloadingTool")
	static TArray<FSoftObjectPath> GatherShareDependenciesRecursively(const TArray<FSoftObjectPath> Assets, const float SharingRate, const int32 FindSoftReferenceDepth = 0, const int32 FindDepth = -1);
};
