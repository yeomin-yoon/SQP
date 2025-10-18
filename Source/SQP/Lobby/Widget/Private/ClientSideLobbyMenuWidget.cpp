// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientSideLobbyMenuWidget.h"

#include "ClientSideLobbyPlayerInfoWidget.h"
#include "SQP_GM_Lobby.h"
#include "SQP_PC_Lobby.h"
#include "ToggleButton.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

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
	//플레이어 정보 위젯 추가
	const auto Temp = CreateWidget<ULobbyPlayerInfoWidgetBase>(this, PlayerInfoWidgetClass);

	//플레이어의 유니크 아이디 바인딩
	Temp->BindingPlayerUniqueId = NewPlayerInfo.PlayerUniqueId;

	//기타 디스플레이 설정
	const FString PlayerRole = NewPlayerInfo.LobbyState == ELobbyState::Host ? TEXT("호스트 :") : TEXT("참가자 :");
	const FSlateColor PlayerColor = NewPlayerInfo.LobbyState == ELobbyState::Host ? FColor::Purple : (NewPlayerInfo.LobbyState == ELobbyState::ReadyClient ? FColor::Green : FColor::Red);
	Temp->RoleTextBlock->SetColorAndOpacity(PlayerColor);
	Temp->RoleTextBlock->SetText(FText::FromString(PlayerRole));
	Temp->NameTextBlock->SetColorAndOpacity(PlayerColor);
	Temp->NameTextBlock->SetText(FText::FromString(NewPlayerInfo.PlayerName));

	//플레이어 정보 버티컬 박스에 추가
	PlayerInfoBox->AddChildToVerticalBox(Temp);

	//맵에 추가
	UniqueIdToWidgetMap.Add(NewPlayerInfo.PlayerUniqueId, Temp);
}

void UClientSideLobbyMenuWidget::OnReadyButtonClicked(const bool Value)
{
	const auto PC = Cast<ASQP_PC_Lobby>(GetWorld()->GetFirstPlayerController());
	PC->Server_SetLobbyState(Value ? ELobbyState::ReadyClient : ELobbyState::UnreadyClient);
}