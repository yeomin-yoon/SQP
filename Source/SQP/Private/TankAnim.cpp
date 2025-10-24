// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAnim.h"

#include "TankCharacter.h"
#include "UIInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


void UTankAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ATankCharacter>(TryGetPawnOwner());
	if (Owner)
	{
		FollowCam = Owner->GetFollowCamera();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't Cast Owner at AnimInstance."))
	}
}

void UTankAnim::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTankAnim, TurretRotation);
}

void UTankAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Owner)
		return;
	if (!FollowCam)
		return;
	if (Owner->GetComponentByClass<UUIInteractionComponent>())
	{
		FVector TargetLoc = Owner->GetComponentByClass<UUIInteractionComponent>()->HitLoc;
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(Owner->GetMesh()->GetSocketLocation(FName("TurretBarrel")),
															  TargetLoc);
		TurretRotation = FRotator(0.f, Rot.Yaw - Owner->GetActorRotation().Yaw, -1 * Rot.Pitch);
	}
}
