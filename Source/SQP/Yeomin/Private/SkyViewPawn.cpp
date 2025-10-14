#include "SkyViewPawn.h"
#include "EnhancedInputComponent.h"
#include "SkyViewComponent.h"
#include "Camera/CameraComponent.h"


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
	
	OwnerSkyViewComp = GetOwner()->GetComponentByClass<USkyViewComponent>();
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
		EnhancedInputComponent->BindAction(SkyViewAction, ETriggerEvent::Started, this, &ASkyViewPawn::QuitSkyView);
	}
	else
	{
		UE_LOG(LogTemp, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component!"
		       ), *GetNameSafe(this));
	}
}

void ASkyViewPawn::QuitSkyView()
{
	OwnerSkyViewComp->IsSkyView();
}
