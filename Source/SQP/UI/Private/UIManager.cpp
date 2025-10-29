// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"

#include "MainUI.h"
#include "Blueprint/UserWidget.h"


UUIManager::UUIManager()
{
	ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("Class'/Game/Splatoon/UI/WBP_MainUI.WBP_MainUI_C'"));
	if (WidgetClassFinder.Succeeded())
	{
		MainUIClass = WidgetClassFinder.Class;
	}
}

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}


UMainUI* UUIManager::CreateMainUI()
{
	MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
	MainUI->AddToViewport();
	MainUI->SetIsFocusable(false);

	return MainUI;
}

USlider* UUIManager::GetSliderUI()
{
	return MainUI->BrushSlider.Get();
}

FLinearColor UUIManager::GetCurrentColor()
{
	if (!MainUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("MainUI is nullptr"));
		return FLinearColor::White;
	}
	return MainUI->GetBorderColor();
}
