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
	UFUNCTION(BlueprintCallable)
	void IsSkyView();
	UFUNCTION(BlueprintCallable)
	void ChangeView(bool isSkyView);
	
protected:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;
	UPROPERTY()
	TObjectPtr<APawn> OwnerPawn;
	UPROPERTY()
	TObjectPtr<class ASkyViewPawn> SkyViewPawn;
	
	bool bIsSkyView = false;
};
