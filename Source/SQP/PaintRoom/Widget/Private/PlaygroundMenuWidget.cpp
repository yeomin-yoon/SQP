// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaygroundMenuWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"	

void UPlaygroundMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuButton->OnClicked.AddDynamic(this, &UPlaygroundMenuWidget::OnMenuButtonClicked);
	BackMainButton->OnClicked.AddDynamic(this, &UPlaygroundMenuWidget::OnBackMainButtonClicked);
	BackPaintButton->OnClicked.AddDynamic(this, &UPlaygroundMenuWidget::OnBackPaintButtonClicked);
}

void UPlaygroundMenuWidget::OnMenuButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void UPlaygroundMenuWidget::OnBackMainButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("Main"), true);
}

void UPlaygroundMenuWidget::OnBackPaintButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}