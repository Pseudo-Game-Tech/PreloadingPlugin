// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PreloadingBehaviorEditor/Public/PreloadingBehaviorEditor.h"
#include "PreloadingBehaviorEditor/Public/PreloadingBehaviorAssetType.h"
#include "AssetTypeCategories.h"

#define LOCTEXT_NAMESPACE "FPreloadingBehaviorEditorModule"

void FPreloadingBehaviorEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type Category = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Preloading")), LOCTEXT("PreloadingAssetCategory", "Preloading"));
	TSharedRef<IAssetTypeActions> AssetTypeActions_PreloadBehavior = MakeShareable(new FAssetTypeActions_PreloadBehavior(Category));
	AssetTools.RegisterAssetTypeActions(AssetTypeActions_PreloadBehavior);
}

void FPreloadingBehaviorEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPreloadingBehaviorEditorModule, PreloadingBehaviorEditor)