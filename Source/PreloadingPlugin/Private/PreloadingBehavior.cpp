#include "PreloadingBehavior.h"
#include "Engine/AssetManager.h"
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
			NowPreloading.Add(PreloadingDataMapKey, Request);
		}
	}
}

void UPreloadingBehavior::UnLoading(FName PreloadingDataMapKey)
{
	TSharedPtr<FStreamableHandle> OutRemovedValue;
	if (NowPreloading.RemoveAndCopyValue(PreloadingDataMapKey, OutRemovedValue))
	{
		OutRemovedValue->ReleaseHandle();
	}
}
//__pragma(optimize("", on))