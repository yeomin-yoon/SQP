// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SQP_PS_Lobby.generated.h"

struct FPlayerInfo;

UENUM(Blueprintable)
enum class ELobbyState : uint8
{
	Host = 0,
	UnreadyClient = 1,
	ReadyClient = 2,
};

UCLASS()
class SQP_API ASQP_PS_Lobby : public APlayerState
{
	GENERATED_BODY()

public:
	ASQP_PS_Lobby();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//로비에서 준비 상태를 획득, 설정하는 프로퍼티
	__declspec(property(get=GetReadyState, put=SetReadyState)) bool READY_STATE;
	void SetReadyState(bool Value);
	bool GetReadyState() const { return bReadyState; }

	//로비 입장 시 스냅샷을 위한 커스텀 구조체 반환
	UFUNCTION()
	FPlayerInfo GetPlayerInfo();

protected:
	//준비 상태 필드
	UPROPERTY(ReplicatedUsing = OnRep_bReadyState)
	bool bReadyState;

	//준비 상태 리플리케이션 콜백
	UFUNCTION()
	void OnRep_bReadyState();
	
	//로비 상태 필드
	UPROPERTY(ReplicatedUsing = OnRep_LobbyState)
	ELobbyState LobbyState;

	//로비 상태 필드
	UFUNCTION()
	void OnRep_LobbyState();
};
