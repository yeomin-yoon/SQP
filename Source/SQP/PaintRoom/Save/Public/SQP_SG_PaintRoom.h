// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SQP_SG_PaintRoom.generated.h"

//페인트 데이터를 저장하기 위한 구조체 
USTRUCT(BlueprintType)
struct FPaintExecutionData
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid UniqueID;

	UPROPERTY()
	int32 FaceIndex;

	UPROPERTY()
	uint8 BrushIndex;

	UPROPERTY()
	FLinearColor BrushColor;

	UPROPERTY()
	FVector2D BrushLocation;

	UPROPERTY()
	float BrushSize;

	FPaintExecutionData() : FaceIndex(0), BrushIndex(0), BrushColor(), BrushLocation(), BrushSize(0) { }
	FPaintExecutionData(
		const FGuid InPersistantUniqueID,
		const int32 InFaceIndex,
		const uint8 InBrushIndex,
		const FLinearColor InBrushColor,
		const FVector2D InBrushLocation,
		const float InBrushSize) :
		UniqueID(InPersistantUniqueID),
		FaceIndex(InFaceIndex),
		BrushIndex(InBrushIndex),
		BrushColor(InBrushColor),
		BrushLocation(InBrushLocation),
		BrushSize(InBrushSize) { }
};

//페인트 데이터를 배열로 저장하기 위한 래핑 구조체
USTRUCT(BlueprintType)
struct FPaintExecutionDataWrapper
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FPaintExecutionData> PEDArray;
};

UCLASS()
class SQP_API USQP_SG_PaintRoom : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FGuid, FPaintExecutionDataWrapper> PEDContainer;

	UFUNCTION()
	TArray<FPaintExecutionData> ConstructFullPEDArray() const;
};