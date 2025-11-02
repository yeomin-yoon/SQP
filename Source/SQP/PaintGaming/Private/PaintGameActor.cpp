// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintGaming/Public/PaintGameActor.h"

#include "MainUIComponent.h"
#include "SQP_GS_PaintRoom.h"
#include "TankCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
APaintGameActor::APaintGameActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

// Called when the game starts or when spawned
void APaintGameActor::BeginPlay()
{
	Super::BeginPlay();

	DynMat = FindComponentByClass<UStaticMeshComponent>()->CreateAndSetMaterialInstanceDynamic(0);

	GS = Cast<ASQP_GS_PaintRoom>(UGameplayStatics::GetGameState(GetWorld()));
}

void APaintGameActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APaintGameActor, ElapsedRot)
}


void APaintGameActor::ShowRandomImage(UTexture2D* Image)
{
	if (DynMat)
	{
		DynMat->SetTextureParameterValue(TEXT("ReferenceImage"), Image);
	}
}

// Called every frame
void APaintGameActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		ElapsedRot += 0.8f;
		if (ElapsedRot >= 360.0f)
		{
			ElapsedRot = 0.f;
		}
	}
	SetActorRotation(FRotator(0, ElapsedRot, 0));
}

void APaintGameActor::CountDown()
{
	if (StartTime < 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(StartTimer);
		StartGameTimerFinished();
		return;
	}
	CountDownText();
	StartTime--;
}

void APaintGameActor::CountDownText()
{
	FString RichText = FString::Printf(TEXT("<Timer>%d</>"), StartTime);
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && PC->IsLocalController())
	{
		if (ATankCharacter* TankPlayer = Cast<ATankCharacter>(PC->GetPawn()))
		{
			if (UMainUIComponent* MainUIComp = TankPlayer->FindComponentByClass<UMainUIComponent>())
			{
				//MainUIComp->MainUI->TimerRichTextBlock->SetText(FText::FromString(RichText));
			}
		}
	}
}

void APaintGameActor::StartGameTimerFinished()
{
	OnTimerFinished.Execute();
}
