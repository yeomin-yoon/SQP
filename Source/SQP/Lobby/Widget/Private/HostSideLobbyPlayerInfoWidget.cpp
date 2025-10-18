// Fill out your copyright notice in the Description page of Project Settings.

#include "HostSideLobbyPlayerInfoWidget.h"

#include "SQP_GM_Lobby.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UHostSideLobbyPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//방출 버튼 바인딩
	KickButton->OnClicked.AddDynamic(this, &UHostSideLobbyPlayerInfoWidget::OnClickKickButton);
}

void UHostSideLobbyPlayerInfoWidget::OnClickKickButton()
{
	Cast<ASQP_GM_Lobby>(GetWorld()->GetAuthGameMode())->KickPlayerByUniqueId(BindingPlayerUniqueId);
}