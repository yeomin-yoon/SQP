// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileBase.h"
#include "ProjectilePoolWorldSubsystem.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBase::AProjectileBase()
{
	//액터 틱 비활성화
	PrimaryActorTick.bCanEverTick = false;

	//충돌체 부착
	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
	SphereComp->SetCollisionProfileName(FName("Projectile"), true);
	SetRootComponent(SphereComp);

	//발사체 이동 컴포넌트 부착
	ProjectileMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMoveComp"));

	//발사체 이동 컴포넌트 설정
	ProjectileMoveComp->InitialSpeed = 2000;
	ProjectileMoveComp->bRotationFollowsVelocity = true;
	ProjectileMoveComp->bInterpMovement = true;

	//네트워크 설정
	bReplicates = true;
	SetNetUpdateFrequency(25);
	AActor::SetReplicateMovement(true);
}

void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() == false)
	{
		ProjectileMoveComp->Deactivate();
	}
}

void AProjectileBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//활성화 여부 리플리케이션
	DOREPLIFETIME_CONDITION(AProjectileBase, bIsActive, COND_None);
}

void AProjectileBase::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	//활성화 여부에 따라서 리플리케이션 여부 결정
	DOREPLIFETIME_ACTIVE_OVERRIDE(AProjectileBase, bIsActive, bIsActive);
}

void AProjectileBase::ActiveProjectile(const FTransform& FireTransform, const float InitSpeed)
{
	if (HasAuthority())
	{
		//위치와 회전 설정
		TeleportTo(FireTransform.GetLocation(), FireTransform.GetRotation().Rotator());

		//발사체 컴포넌트 활성화
		ProjectileMoveComp->Velocity = InitSpeed * GetActorForwardVector();
		ProjectileMoveComp->Activate();
		
		//활성화
		bIsActive = true;
		SetActorHiddenInGame(false);
		SetActorEnableCollision(true);

		//풀링 여부에 따라
		if (bIsPoolable)
		{
			//풀링 델리게이트 바인드
			PoolingDelegate.BindUFunction(this, FName("InactivateProjectile"));

			//5초 후에 풀에 반환
			if (const UWorld* World = GetWorld())
			{
				World->GetTimerManager().SetTimer(PoolingTimerHandle, PoolingDelegate, 5, false);
			}	
		}
		else
		{
			//5초 후에 액터 파괴
			SetLifeSpan(5);	
		}
	}
}

void AProjectileBase::InactivateProjectile()
{
	if (HasAuthority())
	{
		//풀링이 활성화 되어 있다면
		if (bIsPoolable)
		{
			//풀링 타이머 핸들 비활성
			if (PoolingTimerHandle.IsValid())
			{
				GetWorldTimerManager().ClearTimer(PoolingTimerHandle);
			}

			//발사체 컴포넌트 비활성화
			ProjectileMoveComp->StopMovementImmediately();
			ProjectileMoveComp->Velocity = FVector::ZeroVector;
			ProjectileMoveComp->Deactivate();

			//비활성화
			bIsActive = false;
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);

			//풀에 반환한다
			if (const UWorld* World = GetWorld())
			{
				if (const auto Subsystem = World->GetSubsystem<UProjectilePoolWorldSubsystem>())
				{
					Subsystem->PushProjectile(this);
				}
			}	
		}
	}
}