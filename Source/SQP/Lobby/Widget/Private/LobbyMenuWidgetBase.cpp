// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyMenuWidgetBase.h"

#include "LobbyPlayerInfoWidgetBase.h"
#include "SQP_GM_Lobby.h"
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

		//플레이어 유니크 아이디 바인딩
		Temp->BindingPlayerUniqueId = PlayerInfo.PlayerUniqueId;

		//기타 디스플레이 설정
		const FString PlayerRole = PlayerInfo.LobbyState == ELobbyState::Host ? TEXT("호스트 :") : TEXT("참가자 :");
		const FSlateColor PlayerColor = PlayerInfo.LobbyState == ELobbyState::Host ? FColor::Purple : (PlayerInfo.LobbyState == ELobbyState::ReadyClient ? FColor::Green : FColor::Red);
		Temp->RoleTextBlock->SetColorAndOpacity(PlayerColor);
		Temp->RoleTextBlock->SetText(FText::FromString(PlayerRole));
		Temp->NameTextBlock->SetColorAndOpacity(PlayerColor);
		Temp->NameTextBlock->SetText(FText::FromString(PlayerInfo.PlayerName));

		//플레이어 정보 버티컬 박스에 추가
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

void ULobbyMenuWidgetBase::UpdatePlayerLobbyState(const FString& PlayerUniqueId, const ELobbyState& Value)
{
	for (const auto Temp : PlayerInfoBox->GetAllChildren())
	{
		if (const auto Target = Cast<ULobbyPlayerInfoWidgetBase>(Temp); Target->BindingPlayerUniqueId == PlayerUniqueId)
		{
			const FColor StateColor = Value == ELobbyState::Host ? FColor::Purple : (Value == ELobbyState::ReadyClient ? FColor::Green : FColor::Red);
			Target->NameTextBlock->SetColorAndOpacity(StateColor);
			Target->RoleTextBlock->SetColorAndOpacity(StateColor);
		}
	}
}