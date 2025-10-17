#include "SkyViewPawn.h"
#include "EnhancedInputComponent.h"
#include "SkyViewComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"


ASkyViewPawn::ASkyViewPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UInputAction> SkyViewActionObj(
		TEXT("'/Game/Splatoon/Input/IA_SkyView.IA_SkyView'"));
	if (SkyViewActionObj.Succeeded())
	{
		SkyViewAction = SkyViewActionObj.Object;
	}

	bReplicates = true;
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
		EnhancedInputComponent->BindAction(SkyViewAction, ETriggerEvent::Started, this,
		                                   &ASkyViewPawn::TriggerSkyView);
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
	Server_QuitSkyView(GetWorld()->GetFirstPlayerController());
}

void ASkyViewPawn::Server_QuitSkyView_Implementation(APlayerController* PC)
{
	GetOwner()->GetComponentByClass<USkyViewComponent>()->Server_IsSkyView(PC);
}
