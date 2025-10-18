// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SQPGameInstance.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	None       UMETA(DisplayName = "None"),
	Main	   UMETA(DisplayName = "Main"),
	Lobby      UMETA(DisplayName = "Lobby"),
	InGame     UMETA(DisplayName = "In Game"),
};

UCLASS()
class SQP_API USQPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	//게임 스테이트를 저장하는 프로퍼티
	__declspec(property(get=GetGameState, put=SetGameState)) EGameState GAME_STATE;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetGameState(const EGameState Value) { CurrentGameState = Value; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGameState GetGameState() const { return CurrentGameState; }

protected:
	//게임 스테이트 필드
	EGameState CurrentGameState;

	//네트워크 오류를 처리하는 콜백
	UFUNCTION()
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

};