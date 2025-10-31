// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintRoom/Default/Public/SQP_PC_PaintRoom.h"

#include "CatchMindWidget.h"
#include "SkyViewPawn.h"
#include "SQP.h"
#include "SQP_GM_PaintRoom.h"
#include "SQP_GS_PaintRoom.h"
#include "SQP_PS_Master.h"
#include "SQP_PS_PaintRoomComponent.h"
#include "TankCharacter.h"
#include "TimerUI.h"
#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASQP_PC_PaintRoom::ASQP_PC_PaintRoom()
{
	//캐치 마인드 위젯 블루프린트 클래스 획득
	if (static ConstructorHelpers::FClassFinder<UUserWidget>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintRoomLevel/WBP_CatchMind.WBP_CatchMind_C"));
		Finder.Succeeded())
	{
		CatchMindWidgetClass = Finder.Class;
	}
}

void ASQP_PC_PaintRoom::Server_PaintColorChange_Implementation(const FLinearColor Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("PaintColorChange!"));
	GetPlayerState<ASQP_PS_Master>()->PaintRoom->SelectedColor = Value;
}

void ASQP_PC_PaintRoom::Server_ChangeBrushSize_Implementation(const float Value)
{
	GetPlayerState<ASQP_PS_Master>()->PaintRoom->SelectedBrushSize = Value;
}

void ASQP_PC_PaintRoom::Server_UpdateLikes_Implementation(const int32 LikeNum)
{
	GetPlayerState<ASQP_PS_Master>()->PaintRoom->LikeCounter = LikeNum;
}

void ASQP_PC_PaintRoom::BeginPlay()
{
	Super::BeginPlay();

	GM = Cast<ASQP_GM_PaintRoom>(UGameplayStatics::GetGameMode(GetWorld()));
	GS = Cast<ASQP_GS_PaintRoom>(UGameplayStatics::GetGameState(this));
	PS = Cast<ASQP_PS_Master>(PlayerState);
	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();

	if (IsLocalController())
	{
		//캐치 마인드 위젯 블루프린트 생성
		if (const auto Created = CreateWidget(this, CatchMindWidgetClass))
		{
			if (const auto Casted = Cast<UCatchMindWidget>(Created))
			{
				CatchMindWidget = Casted;
				CatchMindWidget->HideAll();
				CatchMindWidget->AddToViewport();
			}
		}
		TimerUI = UIManager->CreateTimerUI();
	}
}

void ASQP_PC_PaintRoom::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GS)
	{
		if (IsLocalController())
		{
			ReplicatedCountDown();
		}
	}
}


void ASQP_PC_PaintRoom::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ATankCharacter* player = Cast<ATankCharacter>(InPawn))
	{
		DynMat = player->GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	}
}

UTexture2D* ASQP_PC_PaintRoom::LoadTextureByIndex(int32 Index)
{
	FString Path = FString::Printf(
		TEXT("'/Game/Assets/Tanks/red_tank/Textures/TankTexture/tank_texture_%d.tank_texture_%d'"), Index, Index);
	return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));
}

void ASQP_PC_PaintRoom::Client_ReceiveCatchMindSuggestion_Implementation(const FString& Suggestion)
{
	CatchMindWidget->SetSuggestionText(Suggestion);
}

void ASQP_PC_PaintRoom::Server_CountLike_Implementation(ASQP_PS_Master* TargetPS)
{
	TargetPS->PaintRoom->IncreaseLikeCounter();
}

void ASQP_PC_PaintRoom::ReplicatedCountDown()
{
	if (GS->bOnCountdown)
	{
		Elapsed = GS->GetServerWorldTimeSeconds() - GS->CountdownStartTime;
		Remaining = GS->CountdownTotalTime - Elapsed;

		RemainingTime = FMath::CeilToInt(Remaining);
		if (RemainingTime != LastRemainingTime)
		{
			UpdateCountdownUI(RemainingTime, TimerUI);
			LastRemainingTime = RemainingTime;
		}
		if (Remaining < 0.f)
		{
			Remaining = 0.f;
			GS->bOnCountdown = false;
			LastRemainingTime = -1;
			if (HasAuthority())
			{
				GM->EndCatchMindMiniGame();
			}
		}
	}
}

void ASQP_PC_PaintRoom::UpdateCountdownUI(int RemainingSeconds, UTimerUI* UI)
{
	FString RichText = FString::Printf(TEXT("<Timer>%d</>"), RemainingSeconds);

	if (UI)
	{
		UI->TimerRichTextBlock->SetText(FText::FromString(RichText));
	}
}
