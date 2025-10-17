// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyMenuWidgetBase.h"

#include "LobbyPlayerInfoWidgetBase.h"
#include "SQPGameState.h"
#include "SQP_GM_Lobby.h"
#include "SQPPlayerController.h"
#include "SQP_PC_Lobby.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"

class ULobbyPlayerInfoWidgetBase;

void ULobbyMenuWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	//퇴장하기 버튼 바인딩
	LeaveLobbyButton->OnClicked.AddDynamic(this, &ULobbyMenuWidgetBase::OnLeaveButtonClicked);

	//플레이어 숫자를 지속적으로 업데이트
	GetWorld()->GetTimerManager().SetTimer(UpdateNumOfPlayerTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		NumOfPlayers->SetText(FText::FromString(FString::Printf(TEXT("%d"), GetWorld()->GetGameState()->PlayerArray.Num())));
	}), 0.1, true);
}

void ULobbyMenuWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();

	//플레이어 숫자를 업데이트하던 타이머를 무효화
	GetWorld()->GetTimerManager().ClearTimer(UpdateNumOfPlayerTimerHandle);
}

void ULobbyMenuWidgetBase::OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo)
{
	//플레이어 정보 위젯 추가
	const auto Temp = CreateWidget<ULobbyPlayerInfoWidgetBase>(this, PlayerInfoWidgetClass);
	Temp->NameTextBlock->SetText(FText::FromString(NewPlayerInfo.PlayerName));
	Temp->BindingPlayerUniqueId = NewPlayerInfo.PlayerUniqueId;
	PlayerInfoBox->AddChildToVerticalBox(Temp);

	//맵에 추가
	UniqueIdToWidgetMap.Add(NewPlayerInfo.PlayerUniqueId, Temp);
}

void ULobbyMenuWidgetBase::OnOtherPlayerExit(FPlayerInfo& OldPlayerInfo)
{
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

void ULobbyMenuWidgetBase::OnLeaveButtonClicked()
{
	if (const auto PC = Cast<ASQP_PC_Lobby>(GetWorld()->GetFirstPlayerController()))
	{
		//플레이어 컨트롤러가 서버-클라이언트에 따라서 알아서 처리해준다
		PC->LeaveLobby();
	}
}

void ULobbyMenuWidgetBase::UpdatePlayerInfoReady(const FString& PlayerUniqueId, const bool& Value)
{
	for (const auto Temp : PlayerInfoBox->GetAllChildren())
	{
		if (const auto Target = Cast<ULobbyPlayerInfoWidgetBase>(Temp); Target->BindingPlayerUniqueId == PlayerUniqueId)
		{
			Target->NameTextBlock->SetColorAndOpacity(Value ? FColor::Green: FColor::Red);
		}
	}
}