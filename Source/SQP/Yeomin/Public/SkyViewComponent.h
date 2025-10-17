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
	UFUNCTION(Server, Reliable)
	void Server_SpawnSkyViewPawn();
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_IsSkyView(APlayerController* PC);
	UFUNCTION(Server, Reliable)
	void Server_ResetSkyViewLocation(bool isSkyView);
	UFUNCTION(Server, Reliable)
	void Server_PossessSkyView(bool isSkyView, APlayerController* PC);

protected:
	UPROPERTY()
	TObjectPtr<APawn> OwnerPawn;
	UPROPERTY()
	TObjectPtr<class ASkyViewPawn> SkyViewPawn;

	bool bIsSkyView = true;

	UPROPERTY(EditDefaultsOnly, Category="Sky View")
	FVector InitialLocationOffset = FVector(0.f, 0.f, 100);
	UPROPERTY(EditDefaultsOnly, Category="Sky View")
	float InitialPitchOffset = -15.f;
};
