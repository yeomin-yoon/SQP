// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AISimilarityClient.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FAIResultDelegate, const FString& /*Winner*/);

UCLASS()
class SQP_API UAISimilarityClient : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// ✅ AI 서버 URL (기본은 로컬)
	FString ServerURL = TEXT("http://127.0.0.1:8000/compare");

	// ✅ AI 비교 요청 함수
	void SendCompareRequest(
		UTexture2D* Original,
		UTexture2D* A,
		UTexture2D* B,
		FAIResultDelegate ResultCallback
	);

private:
	bool ConvertTextureToPNG(UTexture2D* Texture, TArray<uint8>& OutData);
};
