// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SQPPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SQP_API ASQPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ASQPPlayerState();

	// 변수가 리플리케이트될 때 호출될 함수 지정
	UFUNCTION()
	void OnRep_bIsReady();

	// 리플리케이트될 프로퍼티를 등록하는 함수
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetReadyState(bool bReady);

	bool IsReady() const { return bIsReady; }

protected:
	// 플레이어의 준비 상태. ReplicatedUsing으로 지정된 함수는 클라이언트에서 값이 변경될 때마다 호출됨
	UPROPERTY(ReplicatedUsing = OnRep_bIsReady)
	bool bIsReady;
};
