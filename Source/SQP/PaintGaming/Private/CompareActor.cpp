// Fill out your copyright notice in the Description page of Project Settings.


#include "CompareActor.h"
#include "AISimilarityClient.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACompareActor::ACompareActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACompareActor::BeginPlay()
{
	Super::BeginPlay();

	if (!Original || !CompareA || !CompareB)
	{
		UE_LOG(LogTemp, Error, TEXT("Texture references missing! Assign in Editor"));
		return;
	}

	auto* Client = GetGameInstance()->GetSubsystem<UAISimilarityClient>();
	Client->SendCompareRequest(Original, CompareA, CompareB,
		FAIResultDelegate::CreateLambda([](const FString& Winner)
		{
			UE_LOG(LogTemp, Warning, TEXT("AI 선택 결과: %s"), *Winner);
		})
	);
	
}

// Called every frame
void ACompareActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

