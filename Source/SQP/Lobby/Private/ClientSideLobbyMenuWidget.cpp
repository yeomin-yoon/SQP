// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientSideLobbyMenuWidget.h"

#include "ClientSideLobbyPlayerInfoWidget.h"
#include "SQPLobbyGameMode.h"
#include "SQPPlayerController.h"
#include "SQPPlayerState.h"
#include "ToggleButton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"

UClientSideLobbyMenuWidget::UClientSideLobbyMenuWidget()
{
	if (static ConstructorHelpers::FClassFinder<UUserWidget>
	Finder(TEXT("/Game/Splatoon/Blueprint/LobbyLevel/WBP_ClientSideLobbyPlayerInfoWidget.WBP_ClientSideLobbyPlayerInfoWidget_C"));
	Finder.Succeeded())
	{
		PlayerInfoWidgetClass = Finder.Class;	
	}
}

void UClientSideLobbyMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ReadyButton->OnToggleStateChanged.AddDynamic(this, &UClientSideLobbyMenuWidget::OnReadyButtonClicked);
}

void UClientSideLobbyMenuWidget::OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo)
{
	Super::OnOtherPlayerEnter(NewPlayerInfo);

	//플레이어 정보 위젯 추가
	const auto Temp = CreateWidget<UClientSideLobbyPlayerInfoWidget>(this, PlayerInfoWidgetClass);
	Temp->NameTextBlock->SetText(FText::FromString(NewPlayerInfo.PlayerName));
	PlayerInfoBox->AddChildToVerticalBox(Temp);

	//맵에 추가
	UniqueIdToWidgetMap.Add(NewPlayerInfo.PlayerUniqueId, Temp);
}

void UClientSideLobbyMenuWidget::OnOtherPlayerExit(FPlayerInfo& OldPlayerInfo)
{
	Super::OnOtherPlayerExit(OldPlayerInfo);
	
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UClientSideLobbyMenuWidget::OnReadyButtonClicked(const bool Value)
{
	Cast<ASQPPlayerController>(GetWorld()->GetFirstPlayerController())->Server_SetReadyState(Value);
	
}