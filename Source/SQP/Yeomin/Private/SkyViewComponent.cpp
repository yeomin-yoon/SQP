#include "SkyViewComponent.h"

#include "SkyViewPawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

USkyViewComponent::USkyViewComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void USkyViewComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetWorld()->GetFirstPlayerController();

	OwnerPawn = Cast<APawn>(GetOwner());
}


void USkyViewComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkyViewComponent::IsSkyView()
{
	if (bIsSkyView) // Sky View On
	{
		ChangeView(bIsSkyView);
		bIsSkyView = false;
		return;
	}
	// Sky View Off
	ChangeView(bIsSkyView);
	bIsSkyView = true;
}

void USkyViewComponent::ChangeView(bool isSkyView)
{
	if (isSkyView) // Change view to Sky View
	{
		FRotator Rot = GetOwner()->GetActorRotation();
		Rot.Pitch = -50.f;

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		SkyViewPawn = GetWorld()->SpawnActor<ASkyViewPawn>(
			ASkyViewPawn::StaticClass(),
			GetOwner()->GetActorLocation() + FVector(0.f, 0.f, 1000.f),
			Rot,
			SpawnParams
		);

		if (PlayerController && SkyViewPawn)
		{
			PlayerController->Possess(SkyViewPawn);
		}
		return;
	}
	if (isSkyView == false) // Change view to Character View
	{
		if (PlayerController && SkyViewPawn)
		{
			PlayerController->Possess(OwnerPawn);
			SkyViewPawn->Destroy();
		}
	}
}
