#include "SkyViewPawn.h"
#include "EnhancedInputComponent.h"
#include "SQP_PC_PaintRoom.h"


ASkyViewPawn::ASkyViewPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UInputAction> SkyViewActionObj(
		TEXT("'/Game/Splatoon/Input/IA_SkyView.IA_SkyView'"));
	if (SkyViewActionObj.Succeeded())
	{
		SkyViewAction = SkyViewActionObj.Object;
	}
}

void ASkyViewPawn::BeginPlay()
{
	Super::BeginPlay();
}


void ASkyViewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkyViewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(SkyViewAction, ETriggerEvent::Started, this, &ASkyViewPawn::TriggerSkyView);
	}
	else
	{
		UE_LOG(LogTemp, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component!"
		       ), *GetNameSafe(this));
	}
}

void ASkyViewPawn::TriggerSkyView()
{
	if (IsLocallyControlled())
	{
		PC = Cast<ASQP_PC_PaintRoom>(GetController());
		if (PC)
		{
			QuitSkyView();
			UE_LOG(LogTemp, Warning, TEXT("PlayerController 찾음: %s"), *PC->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PlayerController 캐스팅 실패"));
		}
	}
}

void ASkyViewPawn::QuitSkyView()
{
	if (PC)
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant Get ASQP_PC_PaintRoom"))
	}
}
