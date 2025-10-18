// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSideLobbyPlayerInfoWidget.h"

#include "Components/Button.h"

void UClientSideLobbyPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//프로필 버튼 바인딩
	ProfileButton->OnClicked.AddDynamic(this, &UClientSideLobbyPlayerInfoWidget::OnProfileButtonClicked);
}

void UClientSideLobbyPlayerInfoWidget::OnProfileButtonClicked()
{
	
}
