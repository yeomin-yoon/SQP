#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkyViewComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQP_API USkyViewComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkyViewComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	void OnSkyView();

	UPROPERTY()
	TObjectPtr<class ASQP_PC_PaintRoom> PlayerController;
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> IMC;
	UPROPERTY()
	TObjectPtr<class UInputAction> SkyViewAction;

	UPROPERTY()
	TObjectPtr<APawn> OwnerPawn;
};
