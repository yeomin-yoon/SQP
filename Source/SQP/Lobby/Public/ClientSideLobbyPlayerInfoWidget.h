// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyPlayerInfoWidgetBase.h"
#include "Blueprint/UserWidget.h"
#include "ClientSideLobbyPlayerInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class SQP_API UClientSideLobbyPlayerInfoWidget : public ULobbyPlayerInfoWidgetBase

{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
};
