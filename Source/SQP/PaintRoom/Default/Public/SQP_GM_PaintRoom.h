// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SQP_GM_PaintRoom.generated.h"

class ASQP_PaintableActor;

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
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//페인트 룸에서 사용하는 위젯 블루프린트 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PaintRoomWidgetClass;

	//제시어로 사용할 수 있는 단어를 저장한 문자열 배열
	UPROPERTY(EditDefaultsOnly)
	TArray<FString> SuggestionArray;

	//캐치 마인드 미니 게임을 시작하는 메서드
	UFUNCTION(BlueprintCallable)
	void StartCatchMindMiniGame();

	//캐치 마인드 미니 게임 제한 시간 오버 메서드
	UFUNCTION()
	void TimeUpCatchMindMiniGame();

	//캐치 마인드 미니 게임을 종료하는 메서드
	UFUNCTION()
	void EndCatchMindMiniGame();

	//캐치 마인드 미니 게임의 캔버스 액터 블루프린트 클래스
	UPROPERTY()
	TSubclassOf<ASQP_PaintableActor> CatchMindCanvasActorClass;

	//캐치 마인드 미니 게임의 캔버스 액터
	UPROPERTY(EditAnywhere)
	TObjectPtr<ASQP_PaintableActor> CatchMindCanvasActor;

	//캐치 마인드 미니 게임을 종료하는 타이머
	FTimerHandle CatchMindMiniGameTimerHandle;

	void StartTimer(class ASQP_GS_PaintRoom* GS, float Time) const;

	//경쟁 미니 게임을 시작하는 메서드
	UFUNCTION(BlueprintCallable)
	void StartCompetitionMiniGame();

	//경쟁 미니 게임을 종료하는 메서드
	UFUNCTION()
	void EndCompetitionMiniGame();

	bool bIsCompetition = false;

	UPROPERTY()
	TObjectPtr<class UAISimilarityClient> SimilarityClient;

	
	//캐치 마인드 데이터 테이블
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> CatchMindMiniGameDataTable;
	
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class ACompareActor> CompareActorClass;
	
	UPROPERTY()
	TArray<APlayerState*> CompetitionPSs;
	
	UPROPERTY()
	TArray<class ACompareActor*> PaintableCompareActors;
	TArray<FString> PlayerNames;
	UPROPERTY()
	TArray<UTexture2D*> CompareTextures;

	void SpawnActorsInCircle(TSubclassOf<ACompareActor> ActorClass, int32 NumActors, float Radius, const FVector& Center);

	void InitCompetition();
	
	UTexture2D* ConvertRenderTargetToTexture2D(UTextureRenderTarget2D* RenderTarget);

	UPROPERTY(EditDefaultsOnly, Category="Spectator")
	TSubclassOf<APawn> SpectatorPawnClass;
};
