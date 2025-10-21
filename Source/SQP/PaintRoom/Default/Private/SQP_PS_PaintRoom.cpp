// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PS_PaintRoom.h"
#include "LikeUI.h"
#include "MainUIComponent.h"
#include "SQP.h"
#include "Components/RichTextBlock.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

void ASQP_PS_PaintRoom::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASQP_PS_PaintRoom, LikeCounter);
}

void ASQP_PS_PaintRoom::IncreaseLikeCounter()
{
	LikeCounter++;
	OnRep_LikeCounter();
}

void ASQP_PS_PaintRoom::OnRep_LikeCounter()
{
	FString RichText = FString::Printf(TEXT("<Impact>%d</>"), LikeCounter);

	if (UMainUIComponent* MainUIComp = GetPawn()->FindComponentByClass<UMainUIComponent>())
	{
		PRINTLOGNET(TEXT("%s"), *GetPawn()->GetActorNameOrLabel());

		ULikeUI* LikeComp = Cast<ULikeUI>(MainUIComp->LikeUIComp->GetWidget());
		if (LikeComp)
		{
			LikeComp->LikeNumberText->SetText(FText::FromString(RichText));
			PRINTLOGNET(TEXT("SetLikeText"));
		}
	}
}
