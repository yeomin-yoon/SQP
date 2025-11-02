// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PS_PaintRoomComponent.h"

#include "CatchMindWidget.h"
#include "LikeUI.h"
#include "MainUI.h"
#include "MainUIComponent.h"
#include "PlaygroundScoreWidget.h"
#include "SQP.h"
#include "SQP_PC_PaintRoom.h"
#include "SQP_PS_Master.h"
#include "Components/RichTextBlock.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"

USQP_PS_PaintRoomComponent::USQP_PS_PaintRoomComponent() : bCanFirePaintBall(true)
{
}

void USQP_PS_PaintRoomComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USQP_PS_PaintRoomComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//프로퍼티 리플리케이션 활성화
	DOREPLIFETIME(USQP_PS_PaintRoomComponent, LikeCounter);
	DOREPLIFETIME(USQP_PS_PaintRoomComponent, PaintRoomRole);
	DOREPLIFETIME(USQP_PS_PaintRoomComponent, bCanFirePaintBall);
}

void USQP_PS_PaintRoomComponent::OnRep_bCanFirePaintBall()
{
	//MUST플레이어가 페인트 볼 발사 가능 여부를 알 수 있도록 UI 업데이트
	PRINTLOGNET(TEXT("CanFirePaintBall : %s!"), bCanFirePaintBall ? TEXT("True") : TEXT("False"));
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
		if (MainUIComp)
		{
			if (MainUIComp->MainUI)
			{
				if (MainUIComp->MainUI->WBP_LikeUI)
				{
					if (MainUIComp->MainUI->WBP_LikeUI->LikeNumberText)
					{
						MainUIComp->MainUI->WBP_LikeUI->LikeNumberText->SetText(FText::FromString(RichText));
					}
				}
			}
		}
		if (const ULikeUI* LikeUI = Cast<ULikeUI>(MainUIComp->LikeUIComp->GetWidget()))
		{
			LikeUI->LikeNumberText->SetText(FText::FromString(RichText));
		}
	}
}

void USQP_PS_PaintRoomComponent::OnRep_PaintRoomRole()
{
	if (const ASQP_PC_PaintRoom* PCPaint = Cast<ASQP_PC_PaintRoom>(GetBindingPSMaster()->GetPlayerController()); PCPaint
		== GetWorld()->GetFirstPlayerController())
	{
		//역할에 따라서 UI 등이 업데이트 된다
		switch (PAINT_ROOM_ROLE)
		{
		case EPaintRoomRole::None:
			{
				//MUST 그림경쟁 UI 업데이트
				break;
			}
		case EPaintRoomRole::CatchMindPainter:
			{
				PCPaint->CatchMindWidget->ShowPainter();
				break;
			}
		case EPaintRoomRole::CatchMindParticipant:
			{
				PCPaint->CatchMindWidget->ShowParticipant();
				break;
			}
		case EPaintRoomRole::CompetitionPainter:
			{
				//MUST 그림경쟁 UI 업데이트
				break;
			}
		default:
			{
				PCPaint->CatchMindWidget->HideAll();
				//MUST 그림경쟁 UI 업데이트
				break;
			}
		}
	}
}
