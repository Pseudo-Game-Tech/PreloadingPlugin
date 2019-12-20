#include "PreloadingBehaviorAssetType.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "PreloadingSubsystem.h"

#define LOCTEXT_NAMESPACE "FPreloadingBehaviorEditorModule"

UPreloadingBehaviorBlueprintFactory::UPreloadingBehaviorBlueprintFactory(const FObjectInitializer& ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UPreloadingBehaviorBlueprint::StaticClass();
	ParentClass = UPreloadingBehavior::StaticClass();
}

UObject* UPreloadingBehaviorBlueprintFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	check(Class->IsChildOf(UBlueprint::StaticClass()));

	if ((ParentClass == nullptr) || !FKismetEditorUtilities::CanCreateBlueprintOfClass(ParentClass))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("ClassName"), (ParentClass != nullptr) ? FText::FromString(ParentClass->GetName()) : LOCTEXT("Null", "(null)"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::Format(LOCTEXT("CannotCreateBlueprintFromClass", "Cannot create a blueprint based on the class '{0}'."), Args));
		return nullptr;
	}
	else
	{
		UClass* BlueprintClass = UPreloadingBehaviorBlueprint::StaticClass();
		UClass* BlueprintGeneratedClass = UPreloadingBehaviorBlueprintGeneratedClass::StaticClass();
		UBlueprint* newBP = FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BPTYPE_Normal, BlueprintClass, BlueprintGeneratedClass, CallingContext);

		auto Template = TSoftObjectPtr<UPreloadingBehaviorBlueprint>(FSoftObjectPath(TEXT("/PreloadingPlugin/BP_PreloadingBehaviorTemplate.BP_PreloadingBehaviorTemplate")));
		UObject* loadedObject = GetMutableDefault<UPreloadingSubsystemSettings>()->PreloadingBehaviorTemplate.IsNull()? Template.LoadSynchronous() : GetMutableDefault<UPreloadingSubsystemSettings>()->PreloadingBehaviorTemplate.LoadSynchronous();
		UBlueprint* templateBP = Cast<UBlueprint>(loadedObject);
		UBlueprint* newBP2 = FKismetEditorUtilities::ReplaceBlueprint(newBP, templateBP);

		return newBP2;
	}
}

FAssetTypeActions_PreloadBehavior::FAssetTypeActions_PreloadBehavior(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

void FAssetTypeActions_PreloadBehavior::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (UObject* Object : InObjects)
	{
		if (UBlueprint* Blueprint = Cast<UBlueprint>(Object))
		{
			bool bLetOpen = true;
			if (!Blueprint->SkeletonGeneratedClass || !Blueprint->GeneratedClass)
			{
				bLetOpen = EAppReturnType::Yes == FMessageDialog::Open(EAppMsgType::YesNo, LOCTEXT("FailedToLoadBlueprintWithContinue", "Blueprint could not be loaded because it derives from an invalid class.  Check to make sure the parent class for this blueprint hasn't been removed! Do you want to continue (it can crash the editor)?"));
			}
			if (bLetOpen)
			{
				TSharedRef< FPreloadingBehaviorEditor > NewEditor(new FPreloadingBehaviorEditor());

				TArray<UBlueprint*> Blueprints;
				Blueprints.Add(Blueprint);

				NewEditor->InitPreloadingBehaviorEditor(Mode, EditWithinLevelEditor, Blueprints, false);
			}
		}
		else
		{
			FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("FailedToLoadBlueprint", "Blueprint could not be loaded because it derives from an invalid class.  Check to make sure the parent class for this blueprint hasn't been removed!"));
		}
	}
}

uint32 FAssetTypeActions_PreloadBehavior::GetCategories()
{
	return MyAssetCategory;
}

UFactory* FAssetTypeActions_PreloadBehavior::GetFactoryForBlueprintType(UBlueprint* InBlueprint) const
{
	UBlueprintFactory* BlueprintFactory = NewObject<UPreloadingBehaviorBlueprintFactory>();
	BlueprintFactory->ParentClass = InBlueprint->GeneratedClass;
	return BlueprintFactory;
}
