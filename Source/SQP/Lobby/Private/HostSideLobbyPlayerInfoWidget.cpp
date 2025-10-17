// Fill out your copyright notice in the Description page of Project Settings.

#include "HostSideLobbyPlayerInfoWidget.h"

#include "SQPLobbyGameMode.h"
#include "Components/Button.h"

void UHostSideLobbyPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KickButton->OnClicked.AddDynamic(this, &UHostSideLobbyPlayerInfoWidget::OnClickKickButton);
}

void UHostSideLobbyPlayerInfoWidget::OnClickKickButton()
{
	Cast<ASQPLobbyGameMode>(GetWorld()->GetAuthGameMode())->KickPlayerByUniqueId(BindingPlayerUniqueId);
}
