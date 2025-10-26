// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintRoomSaveInfoWidget.h"

#include "Components/Image.h"

void UPaintRoomSaveInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsSelect = false;
	BackgroundImage->SetColorAndOpacity(FLinearColor(0.2, 0.2, 0.2, 0.0));
}

FReply UPaintRoomSaveInfoWidget::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//좌클릭이라면 처리
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		//델리게이트 브로드캐스트
		OnItemDoubleClickedDelegate.Broadcast(this);

		BackgroundImage->SetColorAndOpacity(FLinearColor(0.2, 0.2, 0.2, 0.5));
		bIsSelect = true;
		
		return FReply::Handled();
	}

	//우클릭이라면 건너뛰기
	return FReply::Unhandled();
}

void UPaintRoomSaveInfoWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (bIsSelect == false)
	{
		BackgroundImage->SetColorAndOpacity(FLinearColor(0.2, 0.2, 0.2, 0.2));
	}
}

void UPaintRoomSaveInfoWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (bIsSelect == false)
	{
		BackgroundImage->SetColorAndOpacity(FLinearColor(0.2, 0.2, 0.2, 0.0));	
	}
}

void UPaintRoomSaveInfoWidget::OnDeselected()
{
	BackgroundImage->SetColorAndOpacity(FLinearColor(0.2, 0.2, 0.2, 0.0));
	bIsSelect = false;
}