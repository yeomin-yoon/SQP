// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQP_SG_Main.h"
#include "Blueprint/UserWidget.h"
#include "PaintRoomSaveInfoWidget.generated.h"

class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDoubleClickedSignature, UPaintRoomSaveInfoWidget*, ClickedItem);

UCLASS()
class SQP_API UPaintRoomSaveInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnDeselected();

	UPROPERTY()
	bool bIsSelect;

	FOnItemDoubleClickedSignature OnItemDoubleClickedDelegate;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SaveNameTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SaveIDTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> SaveDateTextBlock;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> BackgroundImage;

	UPROPERTY()
	FSQP_PainRoomSaveFormat BindingPRS;
};