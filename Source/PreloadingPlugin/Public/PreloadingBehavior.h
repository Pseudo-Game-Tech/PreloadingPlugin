// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/Blueprint.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "PreloadingBehavior.generated.h"

// 因为蓝图不支持嵌套的容器,用结构体套一层,使间接容器嵌套
USTRUCT(BlueprintType)
struct PRELOADINGPLUGIN_API FPreloadingData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PreloadingData")
	TArray<FSoftObjectPath> Assets;
};

/**
 * UPreloadingBehavior类主要由蓝图继承实现
 */
UCLASS(Blueprintable, BlueprintType)
class PRELOADINGPLUGIN_API UPreloadingBehavior : public UObject
{
	GENERATED_BODY()
	friend class UPreloadingSubsystem;

public:
	~UPreloadingBehavior();

protected:
	// 通过GenerateData()生成的数据
	// 由Loading()/UnLoading()使用
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PreloadingData")
	TMap<FName, FPreloadingData> PreloadingDataMap;

	// 永久加载的资源,在UPreloadingBehavior构造的时候就会加载
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PreloadingData")
	TArray<FSoftObjectPath> PermanentlyLoadAssets;

	// 异步加载时的优先级 ( 0-100 )
	UPROPERTY(EditDefaultsOnly, Category = "PreloadingData")
	int32 Priority = 0;

public:
	// 用于自动生成数据的,通过蓝图编辑器脚本调用( 右键当前资源>脚本化操作>生成PreloadingBehavior数据 )
	UFUNCTION(BlueprintImplementableEvent, Category = "PreloadingBehavior")
	void GenerateData();

private:
	TMap<FName, TSharedPtr<struct FStreamableHandle>> NowPreloading;

	TSharedPtr<struct FStreamableHandle> PermanentlyLoadAssetsHandle;

	// 开始异步加载PreloadingDataMap中指定Key关联的所有资源
	UFUNCTION(BlueprintCallable, Category = "PreloadingBehavior")
	void Loading(FName PreloadingDataMapKey);

	// 释放PreloadingDataMap中指定Key关联的所有资源
	UFUNCTION(BlueprintCallable, Category = "PreloadingBehavior")
	void UnLoading(FName PreloadingDataMapKey);
};

UCLASS()
class PRELOADINGPLUGIN_API UPreloadingBehaviorBlueprintGeneratedClass : public UBlueprintGeneratedClass
{
	GENERATED_BODY()
};

UCLASS(BlueprintType)
class PRELOADINGPLUGIN_API UPreloadingBehaviorBlueprint : public UBlueprint
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	// UBlueprint Implementation
	virtual UClass* GetBlueprintClass() const override { return UPreloadingBehaviorBlueprintGeneratedClass::StaticClass(); };
	virtual void GetReparentingRules(TSet< const UClass* >& AllowedChildrenOfClasses, TSet< const UClass* >& DisallowedChildrenOfClasses) const override
	{
		AllowedChildrenOfClasses.Add(UPreloadingBehavior::StaticClass());
	};
#endif
};