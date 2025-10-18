// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SQP_PS_Lobby.generated.h"

enum class ELobbyState : uint8;
class ASQP_PC_Lobby;
struct FPlayerInfo;

UCLASS()
class SQP_API ASQP_PS_Lobby : public APlayerState
{
	GENERATED_BODY()

public:
	ASQP_PS_Lobby();
	
	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//로비에서 준비 상태를 획득, 설정하는 프로퍼티
	__declspec(property(get=GetLobbyState, put=SetLobbyState)) ELobbyState LOBBY_STATE;
	UFUNCTION(BlueprintCallable)
	ELobbyState GetLobbyState() const { return LobbyState; }
	UFUNCTION(BlueprintCallable)
	void SetLobbyState(const ELobbyState Value)
	{
		if (HasAuthority())
		{
			LobbyState = Value;

			OnRep_LobbyState();
		}
	}

	//로비 입장 시 스냅샷을 위한 커스텀 구조체 반환
	UFUNCTION()
	FPlayerInfo GetPlayerInfo();

protected:
	//현 시스템의 플레이어 컨트롤러 획득
	UFUNCTION()
	ASQP_PC_Lobby* GetLobbyPC() const; 
	
	//로비 상태 필드
	UPROPERTY(ReplicatedUsing = OnRep_LobbyState)
	ELobbyState LobbyState;

	//로비 상태 필드
	UFUNCTION()
	void OnRep_LobbyState();
};