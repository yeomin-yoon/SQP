// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PaintRoomSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPaintedTextureData
{
	GENERATED_BODY()

	UPROPERTY()
	FString ObjectUniqueID;

	UPROPERTY()
	TArray<FLinearColor> Pixels;

	UPROPERTY()
	int32 TextureWidth = 0;

	UPROPERTY()
	int32 TextureHeight = 0;
};

UCLASS()
class SQP_API UPaintRoomSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	TMap<FString, FPaintedTextureData> PaintedTextures;
};
