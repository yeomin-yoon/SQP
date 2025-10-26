// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectedPRSInfoWidget.h"

#include "Components/TextBlock.h"

void USelectedPRSInfoWidget::UpdatePRSInfo(const FSQP_PainRoomSaveFormat& PainRoomSave) const
{
	SaveNameTextBlock->SetText(FText::FromString(PainRoomSave.Name));
	SaveIDTextBlock->SetText(FText::FromString(PainRoomSave.ID));
	SaveDateTextBlock->SetText(FText::FromString(PainRoomSave.Date));
}