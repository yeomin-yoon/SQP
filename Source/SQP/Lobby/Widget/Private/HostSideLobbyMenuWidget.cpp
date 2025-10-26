// Fill out your copyright notice in the Description page of Project Settings.

#include "HostSideLobbyMenuWidget.h"

#include "SQP_GM_Lobby.h"
#include "SQP_SG_Main.h"
#include "ActiveButton.h"
#include "HostSideLobbyPlayerInfoWidget.h"
#include "PaintRoomSaveInfoWidget.h"
#include "SelectedPRSInfoWidget.h"
#include "SQPGameInstance.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

UHostSideLobbyMenuWidget::UHostSideLobbyMenuWidget()
{
	if (static ConstructorHelpers::FClassFinder<UUserWidget>
		Finder(TEXT("/Game/Splatoon/Blueprint/LobbyLevel/WBP_HostSideLobbyPlayerInfoWidget.WBP_HostSideLobbyPlayerInfoWidget_C"));
		Finder.Succeeded())
	{
		PlayerInfoWidgetClass = Finder.Class;	
	}

	if (static ConstructorHelpers::FClassFinder<UUserWidget>
		Finder(TEXT("/Game/Splatoon/Blueprint/LobbyLevel/WBP_PaintRoomSaveInfoWidget.WBP_PaintRoomSaveInfoWidget_C"));
		Finder.Succeeded())
	{
		PaintRoomSaveInfoWidgetClass = Finder.Class;
	}
}

void UHostSideLobbyMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//시작 버튼 바인딩
	StartButton->OnClicked.AddDynamic(this, &UHostSideLobbyMenuWidget::OnStartButtonClicked);

	//취소 버튼 바인딩
	CancelSaveDataSelectionButton->OnClicked.AddDynamic(this, &UHostSideLobbyMenuWidget::OnCancelSaveDataSelectionButtonClicked);

	//메인 세이브 게임에 접근
	if (const auto MainSaveGame = Cast<USQP_SG_Main>(USQPGameInstance::LoadMainSaveGame()))
	{
		//페인트 룸 세이브 게임 하나마다
		for (auto PainRoomSave : MainSaveGame->PaintRoomSaveArray)
		{
			//위젯 생성 후 초기화
			const auto Created = Cast<UPaintRoomSaveInfoWidget>(CreateWidget(this, PaintRoomSaveInfoWidgetClass));
			Created->SaveNameTextBlock->SetText(FText::FromString(PainRoomSave.Name));
			Created->SaveDateTextBlock->SetText(FText::FromString(PainRoomSave.Date));
			Created->SaveIDTextBlock->SetText(FText::FromString(PainRoomSave.ID));
			Created->BindingPRS = PainRoomSave;

			//위젯에 델리게이트 등록
			Created->OnItemDoubleClickedDelegate.AddDynamic(this, &UHostSideLobbyMenuWidget::OnSaveDataSlotDoubleClicked);

			//위젯을 스크롤 박스에 추가
			SaveDataScrollBox->AddChild(Created);
		}
	}
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

// ReSharper disable once CppMemberFunctionMayBeConst
void UHostSideLobbyMenuWidget::OnCancelSaveDataSelectionButtonClicked()
{
	//게임 인스턴스에 지정된 페인트 룸 세이브 ID를 초기화한다
	Cast<USQPGameInstance>(GetWorld()->GetGameInstance())->SetTargetPaintRoomSave(FSQP_PainRoomSaveFormat());
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UHostSideLobbyMenuWidget::OnSaveDataSlotDoubleClicked(UPaintRoomSaveInfoWidget* Target)
{
	//이미 선택된 대상을 다시 더블클릭했다면 아무런 행동도 하지 않는다
	if (CurrentSelectedPaintRoomSaveInfoWidget == Target)
	{
		return;
	}
	
	//게임 인스턴스에 지정된 페인트 룸 세이브 ID를 재설정한다
	Cast<USQPGameInstance>(GetWorld()->GetGameInstance())->SetTargetPaintRoomSave(Target->BindingPRS);

	//이전에 선택된 위젯이 있었다면
	if (CurrentSelectedPaintRoomSaveInfoWidget)
	{
		//비선택 상태로 전환한다
		CurrentSelectedPaintRoomSaveInfoWidget->OnDeselected();	
	}
	
	//최신 선택 위젯 업데이트
	CurrentSelectedPaintRoomSaveInfoWidget = Target;

	//최선 선택 페인트 룸 세이브에 대한 정보를 디스플레이하도록 업데이트
	SelectedPRSInfoWidget->UpdatePRSInfo(Target->BindingPRS);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UHostSideLobbyMenuWidget::OnStartButtonClicked()
{
	Cast<ASQP_GM_Lobby>(GetWorld()->GetAuthGameMode())->MoveToGameMap();
}