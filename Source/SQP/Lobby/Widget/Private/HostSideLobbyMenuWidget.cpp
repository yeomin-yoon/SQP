// Fill out your copyright notice in the Description page of Project Settings.

#include "HostSideLobbyMenuWidget.h"

#include "ActiveButton.h"
#include "HostSideLobbyPlayerInfoWidget.h"
#include "SQP_GM_Lobby.h"
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

void UHostSideLobbyMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//시작 버튼 바인딩
	StartButton->OnClicked.AddDynamic(this, &UHostSideLobbyMenuWidget::OnStartButtonClicked);
}

void UHostSideLobbyMenuWidget::OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo)
{
	//플레이어 정보 위젯 추가
	const auto Temp = CreateWidget<ULobbyPlayerInfoWidgetBase>(this, PlayerInfoWidgetClass);

	//플레이어의 유니크 아이디 바인딩
	Temp->BindingPlayerUniqueId = NewPlayerInfo.PlayerUniqueId;

	//기타 디스플레이 설정
	Temp->NameTextBlock->SetText(FText::FromString(NewPlayerInfo.PlayerName));

	//플레이어 정보 버티컬 박스에 추가
	PlayerInfoBox->AddChildToVerticalBox(Temp);

	//맵에 추가
	UniqueIdToWidgetMap.Add(NewPlayerInfo.PlayerUniqueId, Temp);
}

void UHostSideLobbyMenuWidget::OnStartButtonClicked()
{
	Cast<ASQP_GM_Lobby>(GetWorld()->GetAuthGameMode())->MoveToGameMap();
}