#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PreloadingBehavior.h"
#include "PreloadingSubsystem.generated.h"

UCLASS(config = PreloadingSubsystemSettings)
class PRELOADINGPLUGIN_API UPreloadingSubsystemSettings : public UObject
{
	GENERATED_BODY()
public:
	UPreloadingSubsystemSettings(const FObjectInitializer& ObjectInitializer);

	// 存放UPreloadingBehavior类的路径,建议不要直接输入/Game,否则资源很多的话可能会扫描很久,导致启动很久
	UPROPERTY(Config, EditAnywhere, Category = Settings)
	TArray<FString> ContentPaths;

	// 创建PreloadingBehavior资源时的模版
	UPROPERTY(Config, EditAnywhere, Category = EditorSettings)
	TSoftObjectPtr<UPreloadingBehaviorBlueprint> PreloadingBehaviorTemplate;
};

UCLASS()
class PRELOADINGPLUGIN_API UPreloadingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:	
	// 获取PreloadingBehavior
	UFUNCTION(BlueprintCallable, Category = "PreloadingBehavior")
	UPreloadingBehavior* GetPreloadingBehavior(TSubclassOf<UPreloadingBehavior> Class);
	
	template<class T = UPreloadingBehavior>
	T* GetPreloadingBehavior()
	{
		return (T*)GetPreloadingBehavior(T::StaticClass());
	};

	// 获取所有PreloadingBehavior
	UFUNCTION(BlueprintCallable, Category = "PreloadingBehavior")
	TArray<UPreloadingBehavior*> GetAllPreloadingBehavior();

	// Subsystem 接口
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 确保加载蓝图类型
	static void LoadBlueprintClass();
private:
	UPROPERTY()
	TMap<TSubclassOf<UPreloadingBehavior>, UPreloadingBehavior*> BehaviorMap;
};