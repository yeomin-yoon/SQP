// Fill out your copyright notice in the Description page of Project Settings.


#include "LikeUI.h"

#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Net/UnrealNetwork.h"


void ULikeUI::NativeConstruct()
{
	Super::NativeConstruct();

	LikePlayers.Empty();
	
	LikeBtn->OnPressed.AddDynamic(this, &ULikeUI::OnClick);
}

void ULikeUI::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULikeUI, LikeNum);
}

void ULikeUI::OnRep_UpdateLikes(int32 Num)
{
	FString RichText = FString::Printf(TEXT("<Impact>%d</>"), Num);
	LikeNumberText->SetText(FText::FromString(RichText));
}

void ULikeUI::OnClick()
{
	Server_OnLike_Implementation(ClickingActor);
}


void ULikeUI::Server_OnLike_Implementation(AActor* Actor)
{
	LikePlayers.Add(Actor);
	LikeNum = LikePlayers.Num();
	OnRep_UpdateLikes(LikeNum);
}

