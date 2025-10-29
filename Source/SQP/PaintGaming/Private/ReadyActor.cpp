// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadyActor.h"

#include "MainUI.h"
#include "MainUIComponent.h"
#include "TankCharacter.h"
#include "Components/RichTextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AReadyActor::AReadyActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AReadyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AReadyActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AReadyActor, ReadyPlayerState)
}

// Called every frame
void AReadyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ReadyPlayerState.Num() > 1)
	{
		// TODO: 렌더타겟2D 초기화
		// ClearRenderTarget2D
		// TODO: 추후 bReady, visibility, ReadyPlayerState 초기화

		if (bReady)
			return;

		bReady = true;

		if (bReady)
		{
			StartReadyTimer();
		}
	}
}

void AReadyActor::StartReadyTimer()
{
	ReadyTime = 3;

	GetWorld()->GetTimerManager().SetTimer(
		ReadyTimer,
		this,
		&AReadyActor::CountDown,
		1.0f,
		true
	);
}

void AReadyActor::CountDown()
{
	if (ReadyTime < 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReadyTimer);
		OnTimerFinished.Execute();
		return;
	}
	CountDownText();
	ReadyTime--;
}

void AReadyActor::CountDownText()
{
	FString RichText = FString::Printf(TEXT("<Timer>%d</>"), ReadyTime);
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && PC->IsLocalController())
	{
		if (ATankCharacter* TankPlayer = Cast<ATankCharacter>(PC->GetPawn()))
		{
			if (UMainUIComponent* MainUIComp = TankPlayer->FindComponentByClass<UMainUIComponent>())
			{
				MainUIComp->MainUI->TimerRichTextBlock->SetText(FText::FromString(RichText));
			}
		}
	}
}
