#include "SkyViewComponent.h"
#include "SkyViewPawn.h"
#include "Camera/CameraComponent.h"

USkyViewComponent::USkyViewComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USkyViewComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<APawn>(GetOwner());

	Server_SpawnSkyViewPawn();
}


void USkyViewComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkyViewComponent::Server_SpawnSkyViewPawn_Implementation()
{
	FVector Loc = GetOwner()->GetComponentByClass<UCameraComponent>()->GetComponentLocation() +
		InitialLocationOffset;
	FRotator Rot = GetOwner()->GetActorRotation();
	Rot.Pitch = InitialPitchOffset;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	SkyViewPawn = GetWorld()->SpawnActor<ASkyViewPawn>(
		ASkyViewPawn::StaticClass(),
		Loc,
		Rot,
		SpawnParams
	);
}

void USkyViewComponent::Server_IsSkyView_Implementation(APlayerController* PC)
{
	if (bIsSkyView) // Sky View On
	{
		Server_ResetSkyViewLocation(bIsSkyView);
		Server_PossessSkyView(bIsSkyView, PC);
		bIsSkyView = false;
		return;
	}
	// Sky View Off
	Server_ResetSkyViewLocation(bIsSkyView);
	Server_PossessSkyView(bIsSkyView, PC);
	bIsSkyView = true;
}

void USkyViewComponent::Server_ResetSkyViewLocation_Implementation(bool isSkyView)
{
	if (isSkyView) // Change view to Sky View
	{
		FVector Loc = GetOwner()->GetComponentByClass<UCameraComponent>()->GetComponentLocation() +
			InitialLocationOffset;
		FRotator Rot = GetOwner()->GetActorRotation();
		Rot.Pitch = InitialPitchOffset;

		SkyViewPawn->SetActorTransform(FTransform(Rot, Loc));
	}
}

void USkyViewComponent::Server_PossessSkyView_Implementation(bool isSkyView, APlayerController* PC)
{
	if (isSkyView)
	{
		PC->Possess(SkyViewPawn);
		return;
	}
	if (isSkyView == false) // Change view to Character View
	{
		PC->Possess(OwnerPawn);
	}
}
