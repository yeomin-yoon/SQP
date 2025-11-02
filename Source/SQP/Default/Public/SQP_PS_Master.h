// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SQP_PS_Master.generated.h"

class USQP_PS_PaintRoomComponent;
class USQP_PS_LobbyRoomComponent;

UCLASS()
class SQP_API ASQP_PS_Master : public APlayerState
{
	GENERATED_BODY()

public:
	ASQP_PS_Master();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Destroyed() override;

	virtual void CopyProperties(APlayerState* PlayerState) override;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USQP_PS_LobbyRoomComponent> LobbyRoom;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USQP_PS_PaintRoomComponent> PaintRoom;

	__declspec(property(get=GetScore, put=SetScoreV)) int32 SCORE;
	UFUNCTION()
	FORCEINLINE void SetScoreV(const int32 NewScore) { SetScore(NewScore); OnRep_Score(); }
	virtual void OnRep_Score() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
