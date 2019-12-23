#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Commandlets/Commandlet.h"
#include "PreloadingSubsystemCommandlet.generated.h"

UCLASS()
class PRELOADINGBEHAVIOREDITOR_API UPreloadingSubsystemCommandlet : public UCommandlet
{
	GENERATED_BODY()
public:
	virtual int32 Main(const FString& Params) override;
};