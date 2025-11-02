// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlaygroundScoreWidget.generated.h"

class UPlaygroundPlayerScoreWidget;
class ASQP_PS_Master;
class UVerticalBox;

UCLASS()
class SQP_API UPlaygroundScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> PlayerScoreBox;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PlayerScoreWidgetClass;

	UPROPERTY()
	TMap<FString, UPlaygroundPlayerScoreWidget*> UniqueIdToWidgetMap;

	FTimerHandle UpdateTimerHandle;

	UFUNCTION()
	void UpdatePlaygroundScore();
};
