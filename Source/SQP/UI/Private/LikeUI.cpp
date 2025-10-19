// Fill out your copyright notice in the Description page of Project Settings.


#include "LikeUI.h"

#include "Components/Button.h"
#include "Components/RichTextBlock.h"


void ULikeUI::NativeConstruct()
{
	Super::NativeConstruct();

	LikeBtn->OnPressed.AddDynamic(this, &ULikeUI::OnLike);
}

void ULikeUI::CountLikes()
{
	LikeNum++;
	FString RichText = FString::Printf(TEXT("<Impact>%d</>"), LikeNum);
	LikeNumberText->SetText(FText::FromString(RichText));
}

void ULikeUI::OnLike()
{
	CountLikes();
}

