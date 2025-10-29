// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionInfoWidget.h"

#include "SQP_GI.h"
#include "Components/Button.h"

void USessionInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	JoinButton->OnClicked.AddDynamic(this, &USessionInfoWidget::OnJoinButtonClicked);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void USessionInfoWidget::OnJoinButtonClicked()
{
	if (const auto GI = Cast<USQP_GI>(GetGameInstance()))
	{
		GI->JoinOtherSession(BindingSessionIdx);
	}
}