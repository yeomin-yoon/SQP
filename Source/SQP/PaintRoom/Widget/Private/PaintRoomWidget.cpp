// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintRoomWidget.h"

#include "SQPPaintWorldSubsystem.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"

void UPaintRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//세이브 판넬 오픈 버튼 바인딩
	OpenSavePanelButton->OnClicked.AddDynamic(this, &UPaintRoomWidget::OnOpenSaveButtonClicked);

	//세이브 버튼 바인딩
	SaveButton->OnClicked.AddDynamic(this, &UPaintRoomWidget::OnSaveButtonClicked);

	//페인트 룸 귀환 버튼 바인딩
	BackPaintRoomButton1->OnClicked.AddDynamic(this, &UPaintRoomWidget::OnBackPaintRoomButtonClicked);
	BackPaintRoomButton2->OnClicked.AddDynamic(this, &UPaintRoomWidget::OnBackPaintRoomButtonClicked);
	BackPaintRoomButton3->OnClicked.AddDynamic(this, &UPaintRoomWidget::OnBackPaintRoomButtonClicked);

	//메인 메뉴 귀환 버튼 바인딩
	BackMainMenuButton1->OnClicked.AddDynamic(this, &UPaintRoomWidget::OnBackMainMenuButtonClicked);
	BackMainMenuButton2->OnClicked.AddDynamic(this, &UPaintRoomWidget::OnBackMainMenuButtonClicked);
}

void UPaintRoomWidget::OnOpenSaveButtonClicked()
{
	//두 번째 캔버스 판넬로 전환
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void UPaintRoomWidget::OnSaveButtonClicked()
{
	//페인트 룸 세이브 이름
	const FString PaintRoomSaveName = SaveNameTextBox->GetText().ToString();
	
	//리슨 서버 혹은 스탠드 얼론만 저장할 수 있다
	if (GetWorld()->GetAuthGameMode() && PaintRoomSaveName.Len() > 0)
	{
		//요청
		GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>()->SavePaintOfWorld(PaintRoomSaveName);

		//세 번째 캔버스 판넬로 전환
		WidgetSwitcher->SetActiveWidgetIndex(2);
	}
	else
	{
		//네 번째 캔버스 판넬로 전환
		WidgetSwitcher->SetActiveWidgetIndex(3);
	}
}

void UPaintRoomWidget::OnBackMainMenuButtonClicked()
{
	
}

void UPaintRoomWidget::OnBackPaintRoomButtonClicked()
{
	//첫 번째 캔버스 판넬로 전환
	WidgetSwitcher->SetActiveWidgetIndex(0);
}