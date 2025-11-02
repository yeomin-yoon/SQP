// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQPCharacter.h"
#include "TankCharacter.generated.h"

UCLASS()
class SQP_API ATankCharacter : public ASQPCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATankCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UCapsuleComponent> CapsuleComp;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UMainUIComponent> MainUIComp;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UProjectileShooterComponent> ProjectileShooter;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USceneComponent> BoomOffset;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USpringArmComponent> InteractionBoom;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UUIInteractionComponent> InteractionComp;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USwimComponent> SwimComp;
	UPROPERTY()
	TObjectPtr<class USkyViewComponent> SkyViewComp;
	
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> IMC;
	UPROPERTY()
	TObjectPtr<class UInputAction> FireAction;
	
	void StartFire();
	void CompleteFire();
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool bIsInactivePawn = false;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DynBody;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetTankColor(FLinearColor Color);
	UPROPERTY(Replicated)
	FLinearColor RandomColor;
};
