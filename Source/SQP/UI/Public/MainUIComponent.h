#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "MainUIComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQP_API UMainUIComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UMainUIComponent();

protected:
	virtual void BeginPlay() override;

	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	void BillboardLikeUI();

	UPROPERTY()
	TObjectPtr<class UUIManager> UIManager;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWidgetComponent> LikeUIComp;

	bool bCursorEnabled = true;
	UPROPERTY()
	TObjectPtr<APlayerController> PC;
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> IMC;
	UPROPERTY()
	TObjectPtr<class UInputAction> ToggleMouseAction;
	UFUNCTION()
	void OnToggleMouse(const FInputActionValue& InputActionValue);
};
