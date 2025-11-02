// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class SQP_API AProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	AProjectileBase();

	//서버-클라이언트에 맞춰 초기화
	virtual void BeginPlay() override;

	//활성화
	UFUNCTION()
	void ActiveProjectile(const FTransform& FireTransform, const float InitSpeed);

	//비활성화
	UFUNCTION(BlueprintCallable)
	void InactivateProjectile();

	//프로피티 리플리케이션 설정
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//리플리케이션 여부 설정
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;

protected:
	//활성화 여부
	UPROPERTY(Replicated)
	bool bIsActive;

	//풀링 여부
	UPROPERTY(EditDefaultsOnly)
	bool bIsPoolable;

	//풀링 타이머 핸들
	FTimerHandle PoolingTimerHandle;

	//풀링 델리게이트
	FTimerDelegate PoolingDelegate;

	//충돌체 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComp;

	//발사체 무브 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMoveComp;
};