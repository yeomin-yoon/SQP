// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SQPPlayerController.generated.h"

struct FPlayerInfo;
class ULobbyMenuWidgetBase;
class UInputMappingContext;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class ASQPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

	virtual void SetupInputComponent() override;

public:
	// UI에서 호출할 함수. 내부적으로 서버 RPC를 호출
	UFUNCTION(BlueprintCallable)
	void RequestSetReadyState(bool bNewReadyState);
    
	// UI 업데이트를 위한 클라이언트 함수
	UFUNCTION(Client, Reliable)
	void Client_UpdateReadyStatusInUI(bool bIsReady);

	
	
	UFUNCTION(Client, Reliable)
	void Client_ReceiveEnterPlayerInfo(FPlayerInfo NewPlayerInfo);

	UFUNCTION(Client, Reliable)
	void Client_ReceiveExitPlayerInfo(FPlayerInfo OldPlayerInfo);

	UFUNCTION(Client, Reliable)
	void Client_OnPostLogin(const TArray<FPlayerInfo>& ExistingPlayerInfoArray);

	UFUNCTION()
	void LeaveLobby();


	
	UPROPERTY()
	TObjectPtr<ULobbyMenuWidgetBase> LobbyMenuWidget;
	
	UFUNCTION(Client, Reliable)
	void Client_CreateLobbyWidget(TSubclassOf<UUserWidget> WidgetToShow);
	
	UFUNCTION(Server, Reliable)
	void Server_SetReadyState(bool Value);
};
