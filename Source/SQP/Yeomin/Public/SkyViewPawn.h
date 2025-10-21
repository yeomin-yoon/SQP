#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "SkyViewPawn.generated.h"

UCLASS()
class SQP_API ASkyViewPawn : public ASpectatorPawn
{
	GENERATED_BODY()
	
public:
	ASkyViewPawn();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<class UInputAction> SkyViewAction;

	UPROPERTY()
	TObjectPtr<class ASQP_PC_PaintRoom> PC;
	
	void TriggerSkyView();

	void QuitSkyView();

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;



};
