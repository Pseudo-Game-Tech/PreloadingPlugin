// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PreloadingBehavior.h"
#include "PreloadingSubsystem.generated.h"
/**
 * 
 */
UCLASS(config = PreloadingSubsystemSettings)
class PRELOADINGPLUGIN_API UPreloadingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// 获取PreloadingBehavior
	UFUNCTION(BlueprintCallable, Category = "PreloadingBehavior")
	UPreloadingBehavior* GetPreloadingBehavior(TSubclassOf<UPreloadingBehavior> Class);

	// Subsystem 接口
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
public:
	// 存放UPreloadingBehavior类的路径,建议不要直接输入/Game,否则资源很多的话可能会扫描很久,导致启动很久
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TArray< FString > ContentPaths;
private:
	UPROPERTY()
	TMap<TSubclassOf<UPreloadingBehavior>, UPreloadingBehavior*> BehaviorMap;
};
