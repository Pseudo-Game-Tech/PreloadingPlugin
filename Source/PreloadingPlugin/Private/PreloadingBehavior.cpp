#include "PreloadingBehavior.h"
#include "Engine/AssetManager.h"

DEFINE_LOG_CATEGORY(PreloadingBehaviorLOG);

UPreloadingBehavior::~UPreloadingBehavior()
{
	if (PermanentlyLoadAssetsHandle.IsValid())
	{
		PermanentlyLoadAssetsHandle->ReleaseHandle();
	}

	for (auto& Pair : NowPreloading)
	{
			if (Pair.Value.IsValid())
			{
				Pair.Value->ReleaseHandle();
			}
	}
}
//__pragma(optimize("", off))
void UPreloadingBehavior::Loading(FName PreloadingDataMapKey)
{
	if (NowPreloading.Contains(PreloadingDataMapKey)) return;

	static const FString ContextString(TEXT("UPreloadingBehavior::Loading"));
	if (const auto* Data = PreloadingDataMap.Find(PreloadingDataMapKey))
	{
		TSharedPtr<FStreamableHandle> Request = UAssetManager::GetStreamableManager().RequestAsyncLoad(Data->Assets, FStreamableDelegate(), Priority, true, false, ContextString);
		if (Request.IsValid())
		{
			UE_LOG(PreloadingBehaviorLOG, Log, TEXT("%s::Loading(%s)"), *GetName(), *PreloadingDataMapKey.ToString());
			NowPreloading.Add(PreloadingDataMapKey, Request);
		}
	}
}

void UPreloadingBehavior::UnLoading(FName PreloadingDataMapKey)
{
	TSharedPtr<FStreamableHandle> OutRemovedValue;
	if (NowPreloading.RemoveAndCopyValue(PreloadingDataMapKey, OutRemovedValue))
	{
		UE_LOG(PreloadingBehaviorLOG, Log, TEXT("%s::UnLoading(%s)"), *GetName(), *PreloadingDataMapKey.ToString());
		OutRemovedValue->ReleaseHandle();
	}
}
//__pragma(optimize("", on))