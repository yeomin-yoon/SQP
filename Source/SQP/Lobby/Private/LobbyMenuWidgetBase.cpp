// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyMenuWidgetBase.h"

#include "LobbyPlayerInfoWidgetBase.h"
#include "SQPGameState.h"
#include "SQPLobbyGameMode.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"

class ULobbyPlayerInfoWidgetBase;

void ULobbyMenuWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		NumOfPlayers->SetText(FText::FromString(FString::Printf(TEXT("%d"), GetWorld()->GetGameState()->PlayerArray.Num())));
	}), 0.1, true);
}

void ULobbyMenuWidgetBase::OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo)
{
	
}

void ULobbyMenuWidgetBase::OnOtherPlayerExit(FPlayerInfo& OldPlayerInfo)
{
	
}

void ULobbyMenuWidgetBase::OnPostLogin(const TArray<FPlayerInfo>& ExistingPlayerInfoArray)
{
	for (auto PlayerInfo : ExistingPlayerInfoArray)
	{
		//플레이어 정보 위젯 추가
		const auto Temp = CreateWidget<ULobbyPlayerInfoWidgetBase>(this, PlayerInfoWidgetClass);
		Temp->NameTextBlock->SetText(FText::FromString(PlayerInfo.PlayerName));
		PlayerInfoBox->AddChildToVerticalBox(Temp);

		//맵에 추가
		UniqueIdToWidgetMap.Add(PlayerInfo.PlayerUniqueId, Temp);	
	}
}