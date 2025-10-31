// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "ProjectileShooterComponent.generated.h"

class AProjectileBase;

UENUM()
enum class EShooterMode : uint8
{
	SemiAuto,
	FullAuto,
	Burst
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQP_API UProjectileShooterComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	//각종 프로퍼티를 초기화하는 생성자
	UProjectileShooterComponent();

	//소유 캐릭터 획득
	virtual void BeginPlay() override;

	//매 틱마다 트리거 및 사격 상태를 확인하고 발사체를 생성-발사
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//페인트 볼을 발사한다
	UFUNCTION(BlueprintCallable)
	void FirePaintBall();
	
	//외부에서 호출해 발사 신호를 주는 메서드
	UFUNCTION(BlueprintCallable)
	void PullTrigger();

	//외부에서 호출해 발사 신호를 끊는 메서드
	UFUNCTION(BlueprintCallable)
	void ReleaseTrigger();

	//유저의 발사 입력 개시 신호를 전달하는 Server RPC
	UFUNCTION(Server, Reliable)
	void Server_StartShoot();

	//유자의 발사 입력 중단 신호를 전달하는 Server RPC
	UFUNCTION(Server, Reliable)
	void Server_StopShoot();

	//발사할 블루프린트 클래스를 에디터에서 설정
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectileBase> ProjectileClass;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APawn> OwnerCharacter;
	
	//사격 모드
	UPROPERTY()
	EShooterMode ShooterMode;

	//트리거 여부
	UPROPERTY(VisibleAnywhere)
	bool bIsOnTrigger;

	//지난 발사로부터 지난 시간
	UPROPERTY(VisibleAnywhere)
	float ElapsedTimeAfterLastShoot;

	//다음 발사까지 걸리는 시간
	UPROPERTY(VisibleAnywhere)
	float ShootRate;

	//지난 트리거 이후로 발사된 발사체의 개수
	UPROPERTY(VisibleAnywhere)
	int32 ShootCounter;

	//트리거 당 발사할 수 있는 최대 개수
	UPROPERTY(VisibleAnywhere)
	int32 BurstLimiter;
};
