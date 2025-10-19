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
		if (OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed > 500.f)
		{
			ElapsedTime += DeltaTime;
			Server_ChangeMaxSpeed(
				UEaseFunctionLibrary::LerpFloatEase(1000.f, 500.f, ElapsedTime / SpeedUpTime, EEaseType::EaseOutQuart));
		}
		Server_CheckOnPaint();
	}
}


void USwimComponent::Server_ChangeMaxSpeed_Implementation(float Speed)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed;
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

	// 1. Hit된 Mesh가 캐시에 있는지 확인
	UTextureRenderTarget2D** CachedRT = PaintRTCache.Find(HitComp);

	UTextureRenderTarget2D* PaintRT = nullptr;

	// 2. 없으면 새로 찾고 캐시에 저장
	if (!CachedRT)
	{
		UMaterialInterface* Mat = HitComp->GetMaterial(0);
		UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(Mat);

		if (DynMat)
		{
			UTexture* Tex = nullptr;
			if (DynMat->GetTextureParameterValue(FName("ColorRenderTarget"), Tex))
			{
				PaintRT = Cast<UTextureRenderTarget2D>(Tex);
				if (PaintRT)
				{
					PaintRTCache.Add(HitComp, PaintRT); // 캐싱
				}
			}
		}
	}
	else
	{
		PaintRT = *CachedRT; // 캐시된 RenderTarget 재사용
	}

	// RenderTarget 없으면 페인트 아님
	if (!PaintRT)
		return false;

	// UV 구하기
	FVector2D UV;
	if (!UGameplayStatics::FindCollisionUV(Hit, 0, UV))
		return false;

	// RenderTarget에서 색 읽기
	FColor Color = UKismetRenderingLibrary::ReadRenderTargetUV(this, PaintRT, UV.X, UV.Y);

	// 흰색이면 페인트 X
	return !(Color.R == 255 && Color.G == 255 && Color.B == 255);
}
