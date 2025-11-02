#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AISimilarityClient.generated.h"

UCLASS()
class SQP_API UAISimilarityClient : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void CompareTextures(UTexture2D* Original, const TArray<UTexture2D*>& ComparisonTextures, const TArray<FString>& PlayerNames);
};