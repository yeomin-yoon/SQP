// Fill out your copyright notice in the Description page of Project Settings.


#include "SwimComponent.h"

#include "Features/UEaseFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Shared/FEaseHelper.h"


USwimComponent::USwimComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USwimComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void USwimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
	{
		if (OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed > 550.f)
		{
			ElapsedTime += DeltaTime;
			Server_BrakingFriction(0.07);
			Server_ChangeMaxSpeed(
				UEaseFunctionLibrary::LerpFloatEase(950.f, 550.f, ElapsedTime / SpeedUpTime, EEaseType::EaseOutQuart));
		}
		else
		{
			Server_BrakingFriction(1.5);
		}
		Server_CheckOnPaint();
	}
}


void USwimComponent::Server_ChangeMaxSpeed_Implementation(float Speed)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void USwimComponent::Server_BrakingFriction_Implementation(float Factor)
{
	OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = Factor;
}

void USwimComponent::Server_CheckOnPaint_Implementation()
{
	if (IsOnPaint())
	{
		Server_ChangeMaxSpeed(1000.f);
		ElapsedTime = 0.f;
	}
}


bool USwimComponent::IsOnPaint()
{
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start + FVector(0, 0, -150);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.bReturnFaceIndex = true;
	Params.AddIgnoredActor(GetOwner());

	if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
		return false;

	UPrimitiveComponent* HitComp = Hit.GetComponent();
	if (!HitComp) return false;

	// 1. Find if HitComp is Cached
	UTextureRenderTarget2D** CachedRT = PaintRTCache.Find(HitComp);

	UTextureRenderTarget2D* PaintRT = nullptr;

	// Cache or Recycle RenderTarget
	if (!CachedRT)
	{
		UMaterialInterface* Mat = HitComp->GetMaterial(0);
		UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(Mat);

		if (DynMat)
		{
			UTexture* Texture = nullptr;
			if (DynMat->GetTextureParameterValue(FName("ColorRenderTarget"), Texture))
			{
				PaintRT = Cast<UTextureRenderTarget2D>(Texture);
				if (PaintRT)
				{
					PaintRTCache.Add(HitComp, PaintRT);	// Cache RenderTarget
				}
			}
		}
	}
	else	// Recycle Cached RenderTarget
	{
		PaintRT = *CachedRT;
	}

	// Return False if there is no RenderTarget
	if (!PaintRT)
		return false;

	// Find Collision UV
	FVector2D UV;
	if (!UGameplayStatics::FindCollisionUV(Hit, 0, UV))
		return false;

	// Read RenderTarget at UV
	FColor Color = UKismetRenderingLibrary::ReadRenderTargetUV(this, PaintRT, UV.X, UV.Y);

	// Return False if White, else Return True
	return !(Color.R == 255 && Color.G == 255 && Color.B == 255);
}
