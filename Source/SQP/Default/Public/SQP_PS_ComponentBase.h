// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SQP_PS_ComponentBase.generated.h"

struct FUserInfo;
class USQP_GI;
class ASQP_PS_Master;
class ASQP_PC_Lobby;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQP_API USQP_PS_ComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	//현 시스템의 게임 인스턴스 획득
	UFUNCTION()
	USQP_GI* GetSQPGameInstance() const;
	
	//현 시스템의 플레이어 컨트롤러 획득
	UFUNCTION()
	ASQP_PC_Lobby* GetWorldFirstPCMaster() const;

	//바인딩되어 있는 플레이어 스테이트 획득
	UFUNCTION()
	ASQP_PS_Master* GetBindingPSMaster() const;

	//서버에 플레이어 스테이트가 준비됐다는 걸 알린다
	UFUNCTION(Server, Reliable)
	virtual void Server_NotifyUserInfo(FUserInfo UserInfo);
};