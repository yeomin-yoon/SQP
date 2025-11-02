// Fill out your copyright notice in the Description page of Project Settings.


#include "IMGManager.h"
#include "IMGDataAsset.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

void UIMGManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const FSoftObjectPath AssetPath(TEXT("/Game/DataAssets/DA_RefIMGs.DA_RefIMGs"));
	IMGDataPtr = TSoftObjectPtr<UIMGDataAsset>(AssetPath);

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	IMGData = Cast<UIMGDataAsset>(
		Streamable.LoadSynchronous(IMGDataPtr.ToSoftObjectPath(), true)
	);

	if (!IMGData)
	{
		UE_LOG(LogTemp, Error, TEXT("[IMGManager] Failed to load DA_RefIMGs! Check asset path or packaging."));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[IMGManager] Successfully loaded IMGData: %s"), *IMGData->GetName());
	}
}

UTexture2D* UIMGManager::GetRandomImage()
{
	if (!IMGData || IMGData->ImageData.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[IMGManager] No image data available."));
		return nullptr;
	}

	const int32 Index = FMath::RandRange(0, IMGData->ImageData.Num() - 1);
	UTexture2D* SelectedImage = IMGData->ImageData[Index];

	if (!SelectedImage)
	{
		UE_LOG(LogTemp, Warning, TEXT("[IMGManager] Image at index %d is null."), Index);
		return nullptr;
	}

	return SelectedImage;
}
