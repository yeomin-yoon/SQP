// Fill out your copyright notice in the Description page of Project Settings.


#include "CompareActor.h"

#include "CompetitorName.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Engine/Texture2D.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ACompareActor::ACompareActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(RootComp);
	
	NameUIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameUI"));
	NameUIComp->SetupAttachment(GetRootComponent());
	NameUIComp->SetRelativeLocation(FVector(0.f, 0.f, 75.f));

	ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(
		TEXT("Class'/Game/Splatoon/Blueprint/PaintGaming/WBP_Name.WBP_Name_C'"));
	if (WidgetClassFinder.Succeeded())
	{
		NameUIComp->SetWidgetClass(WidgetClassFinder.Class);
	}

	bReplicates = true;
}



// Called when the game starts or when spawned
void ACompareActor::BeginPlay()
{
	Super::BeginPlay();

	DynMat = FindComponentByClass<UStaticMeshComponent>()->CreateAndSetMaterialInstanceDynamic(0);

	SetPlayerNameText();
}

void ACompareActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACompareActor, CompetitionPlayerName)
}

// Called every frame
void ACompareActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACompareActor::SetPlayerNameText()
{
	UCompetitorName* NameUI = Cast<UCompetitorName>(NameUIComp->GetWidget());
	NameUI->CompetitorName->SetText(FText(FText::FromString(CompetitionPlayerName)));
	float R = FMath::FRandRange(0.3f, 1.0f);
	float G = FMath::FRandRange(0.3f, 1.0f);
	float B = FMath::FRandRange(0.3f, 1.0f);
	FLinearColor RandomColor(R, G, B, 1.0f);
	NameUI->CompetitorName->SetColorAndOpacity(FSlateColor(RandomColor));
}


