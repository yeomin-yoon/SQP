// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SQPPaintWorldSubsystem.generated.h"

UCLASS()
class SQP_API USQPPaintWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	//필요한 텍스처와 머터리얼 베이스를 로드하는 생성자
	USQPPaintWorldSubsystem();

	UFUNCTION()
	void TryPaint(
		const FVector& Start,
		const FVector& End,
		const uint8& BrushIndex,
		const float& BrushSize);

	UFUNCTION()
	void TryPaintColor(
		const FVector& Start,
		const FVector& End,
		const TArray<AActor*>& ActorsToIgnore,
		const uint8& BrushIndex,
		const float& BrushSize, const FLinearColor& BrushColor);
	
	//충돌 구조체에서 컬러 렌더 타겟 텍스처과 노말 렌더 타겟 텍스처를 획득한다
	UFUNCTION(BlueprintCallable)
	void GetRenderTargetFromHit(
		const FHitResult& Hit,
		UTextureRenderTarget2D*& OutColorRenderTarget,
		UTextureRenderTarget2D*& OutNormalRenderTarget) const;

	//컬러 렌더 타겟 텍스처와 노말 렌더 타겟 텍스처를 대상으로 드로잉을 시도한다
	UFUNCTION(BlueprintCallable)
	void PaintRenderTarget(
		uint8 BrushIndex,
		const float BrushSize,
		const FLinearColor& BrushColor,
		const FVector2D& DrawLocation,
		UTextureRenderTarget2D* ColorRenderTarget, UTextureRenderTarget2D* NormalRenderTarget);
	
	//지정 텍스처로 컬러 렌더 타겟 텍스처에 드로잉을 시도한다
	UFUNCTION(BlueprintCallable)
	void PaintColorRenderTarget(
		UTexture2D* BrushTexture,
		const float BrushSize,
		const FLinearColor& BrushColor,
		const FVector2D& DrawLocation, UTextureRenderTarget2D* ColorRenderTarget);

	//지정 텍스처로 노말 렌더 타겟 텍스처에 드로잉을 시도한다
	UFUNCTION(BlueprintCallable)
	void PaintNormalRenderTarget(
		UTexture2D* BrushTexture,
		UTexture2D* BrushAlphaTexture,
		const float BrushSize,
		const FVector2D& DrawLocation,
		UTextureRenderTarget2D* NormalRenderTarget);

protected:
	//컬러 페인트 텍스처 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UTexture2D*> ColorTextureArray;

	//노말 페인트 텍스처 배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UTexture2D*> NormalTextureArray;

	//명중한 머터리얼과 비교하기 위한 캔버스 머터리얼 베이스
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterial> CanvasMaterialBase;

	//컬러용 브러시 다이나믹 머터리얼 인스턴스 생성용 머터리얼 베이스
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterial> ColorBrushMaterialBase;

	//노말용 브러시 다이나믹 머터리얼 인스턴스 생성용 머터리얼 베이스
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterial> NormalBrushMaterialBase;

	//컬러용 브러시 다이나믹 머터리얼 인스턴스 포인터
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMaterialInstanceDynamic> ColorBrushMaterialDynamicInstance;

	//노말용 브러시 다이나믹 머터리얼 인스턴스 포인터
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMaterialInstanceDynamic> NormalBrushMaterialDynamicInstance;
};