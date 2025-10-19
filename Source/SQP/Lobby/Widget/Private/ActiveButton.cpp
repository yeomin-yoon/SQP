// Fill out your copyright notice in the Description page of Project Settings.

#include "ActiveButton.h"

#include "Components/TextBlock.h"

UActiveButton::UActiveButton()
{
	//기본값 설정
	bActive = false;

	//토글 색상
	ActiveOnColor = FLinearColor(0.05, 1, 0.05, 1.0);  
	ActiveOffColor = FLinearColor(1, 0.05, 0.05, 1.0);

	//토글 텍스트
	ActiveOnText = FText::FromString(FString(TEXT("Let's Start!")));
	ActiveOffText = FText::FromString(FString(TEXT("Not Ready...")));

	//활성화 버튼 바인딩
	OnClicked.AddDynamic(this, &UActiveButton::HandleClick);
}

void UActiveButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	//자식이 없는 경우에만 기본 TextBlock을 생성
	if (GetChildrenCount() == 0)
	{
		//텍스트 블록 위젯 생성
		ButtonTextBlock = NewObject<UTextBlock>(this);
		
		//UButton은 하나의 자식만 가질 수 있음
		AddChild(ButtonTextBlock);
	}
	else
	{
		//이미 자식이 있다면, 그 자식이 TextBlock인지 확인하고 캐스팅
		ButtonTextBlock = Cast<UTextBlock>(GetChildAt(0));
	}

	//가운데 정렬
	ButtonTextBlock->SetJustification(ETextJustify::Center);
}

void UActiveButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	UpdateButtonAppearance();
}

void UActiveButton::PostLoad()
{
	Super::PostLoad();
	
	UpdateButtonAppearance();
}

void UActiveButton::SetActive(const bool Value)
{
	//새로운 상태
	bActive = Value;

	//색상 변경
	UpdateButtonAppearance();
		
	//델리게이트 브로드캐스트
	OnActiveStateChanged.Broadcast(bActive);
}

void UActiveButton::HandleClick()
{
	//활성화되어 있을 때만 델리게이트 실행
	if (bActive)
	{
		OnActiveButtonClicked.Broadcast();
	}
}

void UActiveButton::UpdateButtonAppearance()
{
	//버튼 스타일을 변경
	FButtonStyle NewStyle = GetStyle();
	const FSlateColor NewColor = bActive ? ActiveOnColor : ActiveOffColor;
	const FSlateColor NewHalfColor = bActive ? ActiveOnColor / 2 : ActiveOffColor / 2; 
	NewStyle.Normal.TintColor = NewHalfColor;
	NewStyle.Hovered.TintColor = NewColor;
	NewStyle.Pressed.TintColor = NewHalfColor;
	SetStyle(NewStyle);

	//텍스트 변경
	const FText NewText = bActive ? ActiveOnText : ActiveOffText;
	if (ButtonTextBlock)
	{
		ButtonTextBlock->SetText(NewText);	
	}
}