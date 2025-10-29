// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PS_PaintRoomComponent.h"

#include "LikeUI.h"
#include "MainUIComponent.h"
#include "SQP_PS_Master.h"
#include "Components/RichTextBlock.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

class ULikeUI;

void USQP_PS_PaintRoomComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USQP_PS_PaintRoomComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//프로퍼티 리플리케이션 활성화
	DOREPLIFETIME(USQP_PS_PaintRoomComponent, LikeCounter);
}

void USQP_PS_PaintRoomComponent::IncreaseLikeCounter()
{
	//카운터 증가
	LikeCounter++;

	//수동 호출
	OnRep_LikeCounter();
}

void USQP_PS_PaintRoomComponent::OnRep_LikeCounter() const
{
	const FString RichText = FString::Printf(TEXT("<Impact>%d</>"), LikeCounter);

	if (const UMainUIComponent* MainUIComp = GetBindingPSMaster()->GetPawn()->FindComponentByClass<UMainUIComponent>())
	{
		if (const ULikeUI* LikeUI = Cast<ULikeUI>(MainUIComp->LikeUIComp->GetWidget()))
		{
			LikeUI->LikeNumberText->SetText(FText::FromString(RichText));
		}
	}
}