// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQPPlayerController.h"
#include "SQP_PC_PaintRoom.generated.h"

class USQP_GI;
class UPlaygroundScoreWidget;
class UCatchMindWidget;
class ASQP_PS_Master;

UCLASS()
class SQP_API ASQP_PC_PaintRoom : public ASQPPlayerController
{
	GENERATED_BODY()

protected:
	ASQP_PC_PaintRoom();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY()
	TObjectPtr<USQP_GI> GI;
	UPROPERTY()
	TObjectPtr<class ASQP_GM_PaintRoom> GM;
	UPROPERTY()
	TObjectPtr<class ASQP_GS_PaintRoom> GS;
	UPROPERTY()
	TObjectPtr<ASQP_PS_Master> PS;
	UPROPERTY()
	TObjectPtr<class UUIManager> UIManager;
\
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynMat;
	UTexture2D* LoadTextureByIndex(int32 Index);
	
public:	
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION(Server, Reliable)
	void Server_PaintColorChange(FLinearColor Value);

	UFUNCTION(Server, Reliable)
	void Server_ChangeBrushSize(float Value);

	UFUNCTION(Server, Reliable)
	void Server_UpdateLikes(int32 LikeNum);

	UFUNCTION(Server, Reliable)
	void Server_CountLike(ASQP_PS_Master* TargetPS);

	//서버가 캐치마인드 제시어를 보내주는 Client RPC
	UFUNCTION(Client, Reliable)
	void Client_ReceiveCatchMindSuggestion(const FString& Suggestion, const FString& Hint);

	//서버에게 캐치마인드 정답을 제출하는 Server RPC
	UFUNCTION(Server, Reliable)
	void Server_ReceiveCatchMindAnswer(const FString& Answer);

	//클라이언트에 오답 사실을 보내주는 Client RPC
	UFUNCTION(Client, Reliable)
	void Client_NotifyAnswerIsWrong();

	//캐치 마인드 위젯 블루프린트 클래스
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> CatchMindWidgetClass;

	//캐치 마인드 위젯
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCatchMindWidget> CatchMindWidget;

	//플레이그라운드 점수 위젯 블루프린트 클래스
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> PlaygroundScoreWidgetClass;

	//플레이그라운드 위젯
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPlaygroundScoreWidget> PlaygroundScoreWidget;

	UPROPERTY()
	TObjectPtr<class UTimerUI> TimerUI;
	float Elapsed;
	float Remaining;
	int32 RemainingTime;
	int32 LastRemainingTime = -1;
	void ReplicatedCountDown();
	void UpdateCountdownUI(int RemainingSeconds, class UTimerUI* UI);
};
