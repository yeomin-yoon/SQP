// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SQPGameInstance.generated.h"

UCLASS()
class SQP_API USQPGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(blueprintCallable)
	void HostGame();

protected:
	IOnlineSessionPtr SessionInterface;

	UFUNCTION(blueprintCallable)
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	virtual void Init() override;

#pragma region 클라이언트

public:
	//UI에서 호출하여 세션 검색을 시작하는 함수
	UFUNCTION(BlueprintCallable)
	void FindAndJoinGame();

protected:
	//세션 검색 완료 시 호출될 델리게이트 함수
	void OnFindSessionsComplete(bool bWasSuccessful);

	//세션 참가 완료 시 호출될 델리게이트 함수
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//검색 결과를 저장할 공유 포인터
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

#pragma endregion

};