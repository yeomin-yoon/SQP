// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SQPPlayerController.generated.h"

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

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input", meta = (AllowPrivateAccess = "true"))
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

public:
	// UI에서 호출할 함수. 내부적으로 서버 RPC를 호출
	UFUNCTION(BlueprintCallable)
	void RequestSetReadyState(bool bNewReadyState);
    
	// UI 업데이트를 위한 클라이언트 함수
	UFUNCTION(Client, Reliable)
	void Client_UpdateReadyStatusInUI(bool bIsReady);

	UFUNCTION(Client, Reliable)
	void Client_CreateLobbyUI();

protected:
	// 클라이언트에서 서버로 준비 상태 변경을 요청하는 RPC
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetReadyState(bool bNewReadyState);
};
