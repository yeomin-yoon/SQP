// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintGaming/Public/PaintGameActor.h"

#include "IMGManager.h"
#include "MainUI.h"
#include "MainUIComponent.h"
#include "ReadyActor.h"
#include "TankCharacter.h"
#include "Components/RichTextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
APaintGameActor::APaintGameActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APaintGameActor::BeginPlay()
{
	Super::BeginPlay();

	AReadyActor* ReadyActor = Cast<AReadyActor>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AReadyActor::StaticClass()));

	ReadyActor->OnTimerFinished.BindUObject(this, &APaintGameActor::StartGameTimer);

	IMGManager = GetGameInstance()->GetSubsystem<UIMGManager>();
	if (!IMGManager) return;

	DynMat = FindComponentByClass<UStaticMeshComponent>()->CreateAndSetMaterialInstanceDynamic(0);
}

void APaintGameActor::Multicast_ShowRandomImage_Implementation(UTexture2D* Image)
{
	if (DynMat)
	{
		DynMat->SetTextureParameterValue(TEXT("ReferenceImage"), Image);
	}
}

void APaintGameActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called every frame
void APaintGameActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APaintGameActor::StartGameTimer()
{
	StartTime = 30;
	if (HasAuthority())
	{
		Multicast_ShowRandomImage(IMGManager->GetRandomImage());
	}

	GetWorld()->GetTimerManager().SetTimer(
		StartTimer,
		this,
		&APaintGameActor::CountDown,
		1.0f,
		true
	);
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
				MainUIComp->MainUI->TimerRichTextBlock->SetText(FText::FromString(RichText));
			}
		}
	}
}

void APaintGameActor::StartGameTimerFinished()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("[SERVER] : Finish"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[CLIENT] : Finish"));
	}
}
