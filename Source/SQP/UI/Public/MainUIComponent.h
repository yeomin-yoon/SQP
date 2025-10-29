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
	UPROPERTY()
	TObjectPtr<class UMainUI> MainUI;
	
protected:
	UFUNCTION()
	void OnClick();
	
	virtual void BeginPlay() override;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UWidgetComponent> LikeUIComp;
protected:
	void BillboardLikeUI();

	UPROPERTY()
	TObjectPtr<class UUIManager> UIManager;
	UPROPERTY()
	TObjectPtr<class USlider> Slider;
	
	bool bCursorEnabled = true;
	UPROPERTY()
	TObjectPtr<APlayerController> PC;
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> IMC;
	UPROPERTY()
	TObjectPtr<class UInputAction> ToggleMouseAction;
	UPROPERTY()
	TObjectPtr<class UInputAction> IA_WheelAction;
	UFUNCTION()
	void OnToggleMouse(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void OffToggleMouse();
	void OnMouseWheel(const FInputActionValue& InputActionValue);

	void SetMainUIScale(float Scale);
	bool bScaleUp = false;
	float ElapsedTime = 0.f;
	float TimeSpeed = 0.5f;
};
