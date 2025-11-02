// Fill out your copyright notice in the Description page of Project Settings.

#include "PlaygroundScoreWidget.h"

#include "PlaygroundPlayerScoreWidget.h"
#include "SQP.h"
#include "SQP_PS_Master.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"

void UPlaygroundScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		UpdatePlaygroundScore();
	}), 1, true);
}

void UPlaygroundScoreWidget::NativeDestruct()
{
	Super::NativeDestruct();

	GetWorld()->GetTimerManager().ClearTimer(UpdateTimerHandle);
}

void UPlaygroundScoreWidget::UpdatePlaygroundScore()
{
	TArray<ASQP_PS_Master*> PSMasterArray;
	for (const auto PS : GetWorld()->GetGameState()->PlayerArray)
	{
		PSMasterArray.Emplace(Cast<ASQP_PS_Master>(PS));	
	}
	
	for (const auto PSMaster : PSMasterArray)
	{
		const FString ID = PSMaster->GetUniqueId()->ToString();
		
		if (const auto MappingWidget = UniqueIdToWidgetMap.Find(ID))
		{
			(*MappingWidget)->SetScore(PSMaster->SCORE);
			
			PRINTLOG(TEXT("Successfully U %s Score Widget"), *PSMaster->GetPlayerName());	
		}
		else
		{
			const auto Created = CreateWidget(this, PlayerScoreWidgetClass);
			if (Created == nullptr)
			{
				return;
			}

			const auto Casted = Cast<UPlaygroundPlayerScoreWidget>(Created);
			if (Casted == nullptr)
			{
				return;
			}

			const FString Name = PSMaster->GetPlayerName();
			const int32 Score = PSMaster->SCORE;

			const bool Mine = PSMaster == GetWorld()->GetFirstPlayerController()->GetPlayerState<ASQP_PS_Master>();
			const FColor Color = Mine ? FColor::Purple : FColor::Emerald;
			Casted->Init(Name, ID, Color, Score);
			PlayerScoreBox->AddChild(Casted);
			UniqueIdToWidgetMap.Emplace(ID, Casted);

			PRINTLOG(TEXT("Successfully Create %s Score Widget"), *PSMaster->GetPlayerName());	
		}
	}
}
