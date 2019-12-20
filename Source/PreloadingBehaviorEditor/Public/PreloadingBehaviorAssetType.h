#pragma once

#include "Core.h"
#include "PreloadingBehavior.h"
#include "Factories/BlueprintFactory.h"
#include "AssetTypeActions/AssetTypeActions_Blueprint.h"
#include "BlueprintEditor.h"
#include "PreloadingBehaviorAssetType.generated.h"

class PRELOADINGBEHAVIOREDITOR_API FAssetTypeActions_PreloadBehavior : public FAssetTypeActions_Blueprint
{
public:
	FAssetTypeActions_PreloadBehavior(EAssetTypeCategories::Type InAssetCategory);

	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions_PreloadBehavior", "AssetTypeActions_PreloadBehavior", "PreloadingBehavior Class"); }
	virtual FColor GetTypeColor() const override { return FColor::Magenta; }
	virtual UClass* GetSupportedClass() const override { return UPreloadingBehaviorBlueprint::StaticClass(); }
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;
protected:
	// FAssetTypeActions_Blueprint Implementation
	virtual UFactory* GetFactoryForBlueprintType(UBlueprint* InBlueprint) const;
private:
	EAssetTypeCategories::Type MyAssetCategory;
};

UCLASS(hidecategories = Object)
class PRELOADINGBEHAVIOREDITOR_API UPreloadingBehaviorBlueprintFactory : public UBlueprintFactory
{
	GENERATED_BODY()

public:
	UPreloadingBehaviorBlueprintFactory(const FObjectInitializer& ObjectInitializer);

	// UFactory Interface
	virtual bool ConfigureProperties() override { return true; };
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
};

class PRELOADINGBEHAVIOREDITOR_API FPreloadingBehaviorEditor : public FBlueprintEditor
{
public:
	void InitPreloadingBehaviorEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, const TArray<class UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode)
	{
		InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, false);
	};
};