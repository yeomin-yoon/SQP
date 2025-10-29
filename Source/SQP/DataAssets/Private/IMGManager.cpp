// Fill out your copyright notice in the Description page of Project Settings.


#include "IMGManager.h"

#include "IMGDataAsset.h"

void UIMGManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	IMGDataPtr = TSoftObjectPtr<UIMGDataAsset>(FSoftObjectPath(TEXT("'/Game/DataAssets/DA_RefIMGs.DA_RefIMGs'")));
	IMGData = IMGDataPtr.LoadSynchronous();
}

UTexture2D* UIMGManager::GetRandomImage()
{
	if (!IMGData) return nullptr;

	int32 Index = FMath::RandRange(0, IMGData->ImageData.Num() - 1);
	return IMGData->ImageData[Index];
}
