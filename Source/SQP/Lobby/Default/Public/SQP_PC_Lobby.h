// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQPPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "SQP_PC_Lobby.generated.h"

class ASQP_GM_Lobby;
enum class ELobbyState : uint8;
class ULobbyMenuWidgetBase;

UCLASS()
class SQP_API ASQP_PC_Lobby : public ASQPPlayerController
{
	GENERATED_BODY()

public:
	//특정 클라이언트가 포스트 로그인 단계에 도달했을 떄 그 정보를 다른 이들에게 전달하는 Client RPC
	UFUNCTION(Client, Reliable)
	void Client_ReceiveEnterPlayerInfo(FPlayerInfo NewPlayerInfo);

	//특정 클라이언트가 로그아웃 단계에 도달했을 때 그 정보를 다른 이들에게 전달하는 Client RPC
	UFUNCTION(Client, Reliable)
	void Client_ReceiveExitPlayerInfo(FPlayerInfo OldPlayerInfo);

	//특정 클라이언트의 포스트 로그인 단계에 서버가 현재 로비 플레이어의 정보를 전달하는 Client RPC
	UFUNCTION(Client, Reliable)
	void Client_OnPostLogin(const TArray<FPlayerInfo>& ExistingPlayerInfoArray);

	//서버에게 자신의 로비 상태를 변경해달라고 요청하는 Server RPC
	UFUNCTION(Server, Reliable)
	void Server_SetLobbyState(ELobbyState Value);

	//로비에서 자발적으로 나간다
	UFUNCTION()
	void LeaveLobby();

	//포스트 로그인에 성공한 클라이언트에 전용 로비 위젯을 띄우도록 명령하는 Client RPC
	UFUNCTION(Client, Reliable)
	void Client_CreateLobbyWidget(TSubclassOf<UUserWidget> TargetWidgetClass);

	//서버의 명령을 받아서 생성한 전용 로비 위젯을 가리키는 포인터 
	UPROPERTY()
	TObjectPtr<ULobbyMenuWidgetBase> LobbyMenuWidget;

	//호스트의 게임 모드
	UFUNCTION()
	ASQP_GM_Lobby* GetHostGameMode() const;

	
	virtual void ClientWasKicked_Implementation(const FText& KickReason) override;
	

	
	

};
