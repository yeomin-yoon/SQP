// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SQPGameInstance.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	None       UMETA(DisplayName = "None"),
	Main	   UMETA(DisplayName = "Main"),
	Lobby      UMETA(DisplayName = "Lobby"),
	InGame     UMETA(DisplayName = "In Game"),
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFindCompleteSignature, const TArray<FOnlineSessionSearchResult>&);

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

#pragma region 세션 관련
	
	//생성한 세션을 가리키는 포인터
	TWeakPtr<IOnlineSession> OnlineSessionInterface;

	UPROPERTY()
	FName CurrentSessionName;

	//세션 생성
	UFUNCTION(BlueprintCallable)
	void CreateMySession(FString DisplayName, int32 PlayerCount);

	//세션 생성 완료 콜백
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	
	//세션 탐색용 포인터
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	//세션 검색
	UFUNCTION(BlueprintCallable)
	void FindOtherSession();

	//세션 생성 완료 시 외부에서 콜백을 받을 수 있도록 하는 델리게이트
	FOnFindCompleteSignature OnFindCompleteDelegate;

	//세션 검색 완료 콜백
	void OnFindSessionCompleted(bool bWasSuccessful);

	//세션 합류
	UFUNCTION(BlueprintCallable)
	void JoinOtherSession(int32 SessionIdx);

	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
#pragma endregion

protected:
	//게임 스테이트 필드
	EGameState CurrentGameState;

	//네트워크 오류를 처리하는 콜백
	UFUNCTION()
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

};