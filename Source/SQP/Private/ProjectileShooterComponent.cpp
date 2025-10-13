// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileShooterComponent.h"

#include "ProjectileBase.h"
#include "ProjectilePoolWorldSubsystem.h"
#include "GameFramework/Character.h"

UProjectileShooterComponent::UProjectileShooterComponent() :
	bIsOnTrigger(false),
	ElapsedTimeAfterLastShoot(0),
	ShootRate(0.2),
	ShootCounter(0),
	BurstLimiter(-1)
{
	//컴포넌트 틱 활성화
	PrimaryComponentTick.bCanEverTick = true;
}

void UProjectileShooterComponent::BeginPlay()
{
	Super::BeginPlay();

	//로컬 플레이어 확인을 위해 소유 캐릭터 획득
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UProjectileShooterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	//서버 측의 오소리티 캐릭터일 때만
	if (OwnerCharacter && OwnerCharacter->HasAuthority())
	{
		//트리거가 활성화되어 있다면
		if (bIsOnTrigger)
		{
			//버스트 리미터보다 적게 발사했다면
			if (BurstLimiter == -1 || ShootCounter < BurstLimiter)
			{
				//시간이 지나가고
				ElapsedTimeAfterLastShoot += DeltaTime;

				//발사 시간이 지나가면
				if (ElapsedTimeAfterLastShoot > ShootRate)
				{
					//발사 카운터를 증가시키고
					ShootCounter++;
			
					//발사 시간을 차감하고
					ElapsedTimeAfterLastShoot -= ShootRate;

					//발사체를 발사한다
					if (const auto Subsystem = GetWorld()->GetSubsystem<UProjectilePoolWorldSubsystem>())
					{
						Subsystem->PopProjectile(ProjectileClass, GetComponentTransform(), 2000);

						// const auto Temp = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, GetComponentTransform());
						// Temp->ActiveProjectile(GetComponentTransform(), 2000);
					}
				}
			}
		}	
	}
}

void UProjectileShooterComponent::PullTrigger()
{
	if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
	{
		Server_StartShoot();
	}
}

void UProjectileShooterComponent::ReleaseTrigger()
{
	if (OwnerCharacter && OwnerCharacter->IsLocallyControlled())
	{
		Server_StopShoot();
	}
}

void UProjectileShooterComponent::Server_StartShoot_Implementation()
{
	bIsOnTrigger = true;
	ElapsedTimeAfterLastShoot = ShootRate;
	ShootCounter = 0;
}

void UProjectileShooterComponent::Server_StopShoot_Implementation()
{
	bIsOnTrigger = false;	
}