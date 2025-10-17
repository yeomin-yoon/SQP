#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainUIComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQP_API UMainUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMainUIComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TObjectPtr<class UUIManager> UIManager;
};
