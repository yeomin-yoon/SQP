// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQP_SG_Main.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SQPGameInstance.generated.h"

struct FSQP_PainRoomSaveFormat;
class USQP_SG_PaintRoom;
class USaveGame;

UENUM(BlueprintType)
enum class EProgramState : uint8
{
	None       UMETA(DisplayName = "None"),
	Main	   UMETA(DisplayName = "Main"),
	Lobby      UMETA(DisplayName = "Lobby"),
	InGame     UMETA(DisplayName = "In Game"),
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFindCompleteSignature, const TArray<FOnlineSessionSearchResult>&);

static const FName SESSION_NAME = FName(TEXT("DP_NAME"));
static const FString MAIN_SAVE = TEXT("MAIN_SAVE");

UCLASS()
class SQP_API USQPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

#pragma region 프로그램 스테이트

protected:
	//프로그램 스테이트 필드
	EProgramState CurrentProgramState;

public:
	//프로그램 스테이트를 저장하는 프로퍼티
	__declspec(property(get=GetProgramState, put=SetProgramState)) EProgramState PROGRAM_STATE;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetProgramState(const EProgramState Value) { CurrentProgramState = Value; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EProgramState GetProgramState() const { return CurrentProgramState; }

#pragma endregion 

#pragma region 세션 생성-검색-합류 관련
	
	//온라인 서브시스템을 통해 얻은 온라인 세션 인터페이스
	TWeakPtr<IOnlineSession> OnlineSessionInterface;

	//생성한 세션의 이름을 저장하는 필드
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

#pragma region 페인트 룸 저장 관련

public:
	UFUNCTION()
	static USaveGame* LoadMainSaveGame();
	
	UFUNCTION()
	void SavePaintRoomData(const FString& PaintRoomSaveName, const FGuid PaintRoomSaveGameID, USaveGame* PaintRoomSaveGame);
	
	UFUNCTION()
	USaveGame* LoadSelectedPaintRoomData() const;

	UFUNCTION()
	FORCEINLINE FSQP_PainRoomSaveFormat GetTargetPaintRoomSave() const { return TargetPaintRoomSave; } 

	UFUNCTION()
	FORCEINLINE void SetTargetPaintRoomSave(const FSQP_PainRoomSaveFormat& Value) { TargetPaintRoomSave = Value;}

private:
	UPROPERTY()
	FSQP_PainRoomSaveFormat TargetPaintRoomSave;

#pragma endregion 
	
protected:
	//네트워크 오류를 처리하는 콜백
	UFUNCTION()
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
};