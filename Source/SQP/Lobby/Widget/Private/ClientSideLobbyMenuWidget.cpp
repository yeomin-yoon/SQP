// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientSideLobbyMenuWidget.h"

#include "ClientSideLobbyPlayerInfoWidget.h"
#include "SQP_PC_Lobby.h"
#include "ToggleButton.h"

UClientSideLobbyMenuWidget::UClientSideLobbyMenuWidget()
{
	//클라이언트 측 로비 플레이어 정보 위젯 블루프린트 클래스 로드
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

	//준비 버튼 바인딩
	ReadyButton->OnToggleStateChanged.AddDynamic(this, &UClientSideLobbyMenuWidget::OnReadyButtonClicked);
}

void UClientSideLobbyMenuWidget::OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo)
{
	Super::OnOtherPlayerEnter(NewPlayerInfo);
}

void UClientSideLobbyMenuWidget::OnOtherPlayerExit(FPlayerInfo& OldPlayerInfo)
{
	Super::OnOtherPlayerExit(OldPlayerInfo);
}

void UClientSideLobbyMenuWidget::OnReadyButtonClicked(const bool Value)
{
	const auto PC = Cast<ASQP_PC_Lobby>(GetWorld()->GetFirstPlayerController());
	PC->Server_SetReadyState(Value);
}