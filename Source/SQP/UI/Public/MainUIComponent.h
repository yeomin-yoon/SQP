#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "MainUIComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnLikeChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQP_API UMainUIComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UMainUIComponent();

	FOnLikeChanged OnLikeChanged;

protected:
	UFUNCTION()
	void OnClick();
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<class UUIInteractionComponent> InteractionComp;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWidgetComponent> LikeUIComp;
protected:
	void BillboardLikeUI();

	UPROPERTY()
	TObjectPtr<class UUIManager> UIManager;
	
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
