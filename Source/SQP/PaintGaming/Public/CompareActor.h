// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQP_PaintableActor.h"
#include "CompareActor.generated.h"

UCLASS()
class SQP_API ACompareActor : public ASQP_PaintableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACompareActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<class USceneComponent> RootComp;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UWidgetComponent> NameUIComp;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> DynMat;

	UPROPERTY(Replicated)
	FString CompetitionPlayerName;

	void SetPlayerNameText();
};
