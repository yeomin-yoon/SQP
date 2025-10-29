// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IMGManager.generated.h"

/**
 * 
 */
UCLASS()
class SQP_API UIMGManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TSoftObjectPtr<class UIMGDataAsset> IMGDataPtr;
	UPROPERTY()
	TObjectPtr<class UIMGDataAsset> IMGData;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	UTexture2D* GetRandomImage();
};
