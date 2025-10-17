// Fill out your copyright notice in the Description page of Project Settings.

#include "HostSideLobbyMenuWidget.h"

#include "HostSideLobbyPlayerInfoWidget.h"
#include "SQPLobbyGameMode.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

UHostSideLobbyMenuWidget::UHostSideLobbyMenuWidget()
{
	if (static ConstructorHelpers::FClassFinder<UUserWidget>
	Finder(TEXT("/Game/Splatoon/Blueprint/LobbyLevel/WBP_HostSideLobbyPlayerInfoWidget.WBP_HostSideLobbyPlayerInfoWidget_C"));
	Finder.Succeeded())
	{
		PlayerInfoWidgetClass = Finder.Class;	
	}
}

void UHostSideLobbyMenuWidget::OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo)
{
	Super::OnOtherPlayerEnter(NewPlayerInfo);

	//플레이어 정보 위젯 추가
	const auto Temp = CreateWidget<ULobbyPlayerInfoWidgetBase>(this, PlayerInfoWidgetClass);
	Temp->NameTextBlock->SetText(FText::FromString(NewPlayerInfo.PlayerName));
	Temp->BindingPlayerUniqueId = NewPlayerInfo.PlayerUniqueId;
	PlayerInfoBox->AddChildToVerticalBox(Temp);

	//맵에 추가
	UniqueIdToWidgetMap.Add(NewPlayerInfo.PlayerUniqueId, Temp);
}

void UHostSideLobbyMenuWidget::OnOtherPlayerExit(FPlayerInfo& OldPlayerInfo)
{
	Super::OnOtherPlayerExit(OldPlayerInfo);

	//플레이어 정보 위젯 삭제
	if (UUserWidget** Find = UniqueIdToWidgetMap.Find(OldPlayerInfo.PlayerUniqueId))
	{
		if (UWidget* Widget = *Find)
		{
			Widget->RemoveFromParent();
		}
	}

	//맵에서 제거
	UniqueIdToWidgetMap.Remove(OldPlayerInfo.PlayerUniqueId);
}