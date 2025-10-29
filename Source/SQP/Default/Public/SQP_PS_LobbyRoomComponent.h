// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQP_PS_ComponentBase.h"
#include "Components/ActorComponent.h"
#include "SQP_PS_LobbyRoomComponent.generated.h"

enum class ELobbyState : uint8;
struct FUserInfo;
struct FPlayerInfo;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQP_API USQP_PS_LobbyRoomComponent : public USQP_PS_ComponentBase
{
	GENERATED_BODY()

public:
	USQP_PS_LobbyRoomComponent();
	
	virtual void BeginPlay() override;

	virtual void InitializeComponents();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//BeginPlay 이후 유저의 정보를 보내주는 Server RPC의 오버라이드
	virtual void Server_NotifyUserInfo_Implementation(FUserInfo UserInfo) override;

	//로비에서 준비 상태를 획득, 설정하는 프로퍼티
	__declspec(property(get=GetLobbyState, put=SetLobbyState)) ELobbyState LOBBY_STATE;
	UFUNCTION(BlueprintCallable)
	ELobbyState GetLobbyState() const;
	UFUNCTION(BlueprintCallable)
	void SetLobbyState(const ELobbyState Value);

	//로비 입장 시 스냅샷을 위한 커스텀 구조체 반환
	UFUNCTION()
	FPlayerInfo GetPlayerInfo() const;

protected:
	//로비 상태 필드
	UPROPERTY(ReplicatedUsing = OnRep_LobbyState)
	ELobbyState LobbyState;

	//로비 상태 필드
	UFUNCTION()
	void OnRep_LobbyState() const;
};