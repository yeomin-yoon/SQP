// Fill out your copyright notice in the Description page of Project Settings.

#include "Main/Default/Public/SQP_GM_Main.h"

#include "Blueprint/UserWidget.h"

ASQP_GM_Main::ASQP_GM_Main()
{
	if (ConstructorHelpers::FClassFinder<UUserWidget>
		Finder(TEXT("/Game/Splatoon/Blueprint/MainLevel/WBP_MainMenu.WBP_MainMenu_C"));
		Finder.Succeeded())
	{
		MainMenuWidgetClass = Finder.Class;
	}
}

void ASQP_GM_Main::BeginPlay()
{
	Super::BeginPlay();

	//메인 메뉴의 위젯 블루프린트 생성
	const auto CreatedWidget = CreateWidget(GetWorld(), MainMenuWidgetClass);
	CreatedWidget->AddToViewport();
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;	
}
