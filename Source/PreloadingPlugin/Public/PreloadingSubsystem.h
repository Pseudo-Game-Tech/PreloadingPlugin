// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PreloadingBehavior.h"
#include "PreloadingSubsystem.generated.h"
/**
 * 
 */
UCLASS()
class PRELOADINGPLUGIN_API UPreloadingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// 获取PreloadingBehavior
	UFUNCTION(BlueprintCallable, Category = "PreloadingBehavior")
	UPreloadingBehavior* GetPreloadingBehavior(TSubclassOf<UPreloadingBehavior> Class);

	// Subsystem 接口
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
private:
	UPROPERTY()
	TMap<TSubclassOf<UPreloadingBehavior>, UPreloadingBehavior*> BehaviorMap;
};
