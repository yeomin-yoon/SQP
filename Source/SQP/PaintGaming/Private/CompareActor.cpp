// Fill out your copyright notice in the Description page of Project Settings.


#include "CompareActor.h"
#include "AISimilarityClient.h"
#include "PaintGameActor.h"
#include "SQP_GS_PaintRoom.h"
#include "Engine/Texture2D.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACompareActor::ACompareActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (Tags.Contains("0"))
	{
		Tags.Add(FName("1"));
	}
	Tags.Add(FName("0"));
}



// Called when the game starts or when spawned
void ACompareActor::BeginPlay()
{
	Super::BeginPlay();

	APaintGameActor* PaintGameActor = Cast<APaintGameActor>(
		UGameplayStatics::GetActorOfClass(GetWorld(), APaintGameActor::StaticClass()));

	PaintGameActor->OnTimerFinished.BindUObject(this, &ACompareActor::FinishGame);

	if (PaintGamePlayer == EPaintGamePlayer::PlayerA)
	{
		Tags.AddUnique("PlayerA");
	}
	else if (PaintGamePlayer == EPaintGamePlayer::PlayerB)
	{
		Tags.AddUnique("PlayerB");
	}

	Client = GetGameInstance()->GetSubsystem<UAISimilarityClient>();

	GS = Cast<ASQP_GS_PaintRoom>(UGameplayStatics::GetGameState(GetWorld()));

	DynMat = FindComponentByClass<UStaticMeshComponent>()->CreateAndSetMaterialInstanceDynamic(0);
}

// Called every frame
void ACompareActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACompareActor::SetCompareImage(UTexture2D* Image)
{
	
}

void ACompareActor::EvaluateWinner()
{

}

void ACompareActor::FinishGame()
{
	UTexture* ColorTexture;
	DynMat->GetTextureParameterValue(FName("ColorRenderTarget"), ColorTexture);
	UTexture2D* Texture2D = Cast<UTexture2D>(ColorTexture);
	SetCompareImage(Texture2D);
	EvaluateWinner();
}