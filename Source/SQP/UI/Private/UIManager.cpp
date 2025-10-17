// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"

#include "MainUI.h"
#include "Blueprint/UserWidget.h"


UUIManager::UUIManager()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("Class'/Game/Splatoon/UI/WBP_MainUI.WBP_MainUI_C'"));
	if (WidgetClassFinder.Succeeded())
	{
		MainUIClass = WidgetClassFinder.Class;
	}
}

void UUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
}

void UUIManager::CreateMainUI()
{
	MainUI = CreateWidget<UMainUI>(GetWorld(), MainUIClass);
	MainUI->AddToViewport();
	UE_LOG(LogTemp, Log, TEXT("Added MainUI"));
}

FLinearColor UUIManager::GetCurrentColor()
{
	return MainUI->GetBorderColor();
}
