// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQPPlayerController.h"
#include "SQP_PC_PaintRoom.generated.h"

class UCatchMindWidget;
class ASQP_PS_Master;

UCLASS()
class SQP_API ASQP_PC_PaintRoom : public ASQPPlayerController
{
	GENERATED_BODY()

protected:
	ASQP_PC_PaintRoom();
	
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynMat;
	UTexture2D* LoadTextureByIndex(int32 Index);
	
public:	
	virtual void OnPossess(APawn* InPawn) override;
	UPROPERTY()
	TObjectPtr<APawn> CurrentPawn;
	UPROPERTY()
	TObjectPtr<APawn> PreviousPawn;

	UFUNCTION(Server, Reliable)
	void Server_PaintColorChange(FLinearColor Value);

	UFUNCTION(Server, Reliable)
	void Server_ChangeBrushSize(float Value);

	UFUNCTION(Server, Reliable)
	void Server_UpdateLikes(int32 LikeNum);

	void SpawnSkyViewPawn();
	UPROPERTY(EditDefaultsOnly, Category="Sky View")
	FVector InitialLocationOffset = FVector(0.f, 0.f, 100);
	UPROPERTY(EditDefaultsOnly, Category="Sky View")
	float InitialPitchOffset = -15.f;
	UPROPERTY()
	TObjectPtr<class ASkyViewPawn> SkyViewPawn;
	UFUNCTION(Server, Reliable)
	void Server_PossessSkyView();
	UFUNCTION(Server, Reliable)
	void Server_PossessPreviousPawn();
	void OnSkyView();

	UFUNCTION(Server, Reliable)
	void Server_CountLike(ASQP_PS_Master* TargetPS);

	//서버가 캐치마인드 제시어를 보내주는 Client RPC
	UFUNCTION(Client, Reliable)
	void Client_ReceiveCatchMindSuggestion(const FString& Suggestion, const FString& Hint);

	//서버에게 캐치마인드 정답을 보내주는 Server RPC
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
};
