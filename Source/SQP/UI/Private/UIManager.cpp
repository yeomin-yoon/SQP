// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"

#include "MainUI.h"
#include "TimerUI.h"
#include "Blueprint/UserWidget.h"


UUIManager::UUIManager()
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainUIClassFinder(TEXT("Class'/Game/Splatoon/UI/WBP_MainUI.WBP_MainUI_C'"));
	if (MainUIClassFinder.Succeeded())
	{
		MainUIClass = MainUIClassFinder.Class;
	}

	ConstructorHelpers::FClassFinder<UUserWidget> TimerUIClassFinder(
		TEXT("Class'/Game/Splatoon/UI/WBP_TimerUI.WBP_TimerUI_C'"));
	if (TimerUIClassFinder.Succeeded())
	{
		TimerUIClass = TimerUIClassFinder.Class;
	}
}

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}


UMainUI* UUIManager::CreateMainUI()
{
	MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
	MainUI->AddToViewport(-1);
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

UTimerUI* UUIManager::CreateTimerUI()
{
	UTimerUI* TimerUI = CreateWidget<UTimerUI>(GetWorld(), TimerUIClass);
	TimerUI->AddToViewport();
	TimerUI->SetIsFocusable(false);
	return TimerUI;
}