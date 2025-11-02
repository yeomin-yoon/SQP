// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQP_SG_Main.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SQP_GI.generated.h"

struct FSQP_PainRoomSaveFormat;
class USQP_SG_PaintRoom;
class USaveGame;

USTRUCT()
struct FUserInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString UserNickname;

	UPROPERTY()
	FGuid UserID;

	UPROPERTY()
	FString UserIDString;

	FUserInfo() : UserNickname(TEXT(""))
	{
		UserID = FGuid::NewGuid();
		UserIDString = UserID.ToString();
	}
	FUserInfo(const FString& InNickname) : UserNickname(InNickname)
	{
		UserID = FGuid::NewGuid();
		UserIDString = UserID.ToString();
	}
};

UENUM(BlueprintType)
enum class EProgramState : uint8
{
	None       UMETA(DisplayName = "None"),
	Main	   UMETA(DisplayName = "Main"),
	Lobby      UMETA(DisplayName = "Lobby"),
	PaintRoom     UMETA(DisplayName = "PaintRoom"),
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFindCompleteSignature, const TArray<FOnlineSessionSearchResult>&);

static const FName SESSION_NAME = FName(TEXT("DP_NAME"));
static const FString MAIN_SAVE = TEXT("MAIN_SAVE");

UCLASS()
class SQP_API USQP_GI : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

#pragma region 유저 스테이트

	UPROPERTY()
	FUserInfo UserInfo;

	UFUNCTION()
	FORCEINLINE void AssignNewUser(const FString& NewNickname) { UserInfo = FUserInfo(NewNickname); }

	UFUNCTION()
	FORCEINLINE bool CheckNameEmpty() const { return UserInfo.UserNickname.IsEmpty(); }

#pragma endregion

#pragma region 프로그램 스테이트

protected:
	//프로그램 스테이트 필드
	EProgramState CurrentProgramState;
	
	//네트워크 오류를 처리하는 콜백
	UFUNCTION()
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

public:
	//프로그램 스테이트를 저장하는 프로퍼티
	__declspec(property(get=GetProgramState, put=SetProgramState)) EProgramState PROGRAM_STATE;
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetProgramState(const EProgramState Value) { CurrentProgramState = Value; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EProgramState GetProgramState() const { return CurrentProgramState; }

#pragma endregion 

#pragma region 세션 핵심

	//온라인 서브시스템을 통해 얻은 온라인 세션 인터페이스
	TWeakPtr<IOnlineSession> OnlineSessionInterface;

	//생성한 세션의 이름을 저장하는 필드 - 세션 관리는 세션 이름으로 이루어진다
	UPROPERTY()
	FName CurrentSessionName;

#pragma endregion 

#pragma region 세션 생성 관련

	//세션 생성
	UFUNCTION(BlueprintCallable)
	void CreateMySession(FString DisplayName, int32 PlayerCount) const;

	//세션 생성 완료 콜백
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	
#pragma endregion

#pragma region 세션 검색 관련

	//세션 검색
	UFUNCTION(BlueprintCallable)
	void FindOtherSession();

	//세션 생성 완료 시 외부에서 콜백을 받을 수 있도록 하는 델리게이트
	FOnFindCompleteSignature OnFindCompleteDelegate;
	
	//세션 검색용 객체 포인터
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	//세션 검색 완료 콜백
	void OnFindSessionCompleted(bool bWasSuccessful) const;
	
#pragma endregion

#pragma region 세션 합류 관련

	//세션 합류
	UFUNCTION(BlueprintCallable)
	void JoinOtherSession(int32 SessionIdx) const;
	
	//세션 합류 완료 콜백
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

#pragma endregion

#pragma region 세션 파괴-종료 관련

	UFUNCTION()
	void TerminateMySession() const;
	
	//세션 파괴
	UFUNCTION()
	void DestroyMySession() const;

	//세션 파괴 완료 콜백
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	//세션 종료
	UFUNCTION()
	void EndMySession() const;

	//세션 종료 완료 콜백
	virtual void OnEndSessionCompleted(FName SessionName, bool bWasSuccessful);

#pragma endregion

#pragma region 페인트 룸 저장 관련

public:
	UFUNCTION()
	static USaveGame* LoadMainSaveGame();
	
	UFUNCTION()
	void SavePaintRoomData(const FString& PaintRoomSaveName, const FGuid PaintRoomSaveGameID, USaveGame* PaintRoomSaveGame) const;
	
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

public:
	//호스트는 관찰자로서 참여하는가?
	UPROPERTY()
	bool bHostAsSpectator;

	//플레이 그라운드 모드인가?
	UPROPERTY()
	bool bPlayground;
};