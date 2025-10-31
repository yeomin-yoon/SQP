// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SQP_GM_PaintRoom.generated.h"

USTRUCT(BlueprintType)
struct FCatchMind : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Hint;

	UPROPERTY(EditAnywhere)
	FString Suggestion;

	FCatchMind() { }
	FCatchMind(const FString& InHint, const FString& InSuggestion) : Hint(InHint), Suggestion(InSuggestion) { }
};

UCLASS()
class SQP_API ASQP_GM_PaintRoom : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASQP_GM_PaintRoom();
	
	virtual void BeginPlay() override;

	//페인트 룸에서 사용하는 위젯 블루프린트 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PaintRoomWidgetClass;

	//제시어로 사용할 수 있는 단어를 저장한 문자열 배열
	UPROPERTY(EditDefaultsOnly)
	TArray<FString> SuggestionArray;

	//캐치 마인드 미니 게임을 시작하는 메서드
	UFUNCTION(BlueprintCallable)
	void StartCatchMindMiniGame();

	//캐치 마인드 미니 게임을 종료하는 메서드
	UFUNCTION()
	void EndCatchMindMiniGame();

	//캐치 마인드 미니 게임을 종료하는 타이머
	FTimerHandle CatchMindMiniGameTimerHandle;

	//캐치 마인드 데이터 테이블
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> CatchMindMiniGameDataTable;
};