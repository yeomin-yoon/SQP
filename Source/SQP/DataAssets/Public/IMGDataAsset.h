// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "IMGDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SQP_API UIMGDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Images")
	TArray<TObjectPtr<UTexture2D>> ImageData;
};
