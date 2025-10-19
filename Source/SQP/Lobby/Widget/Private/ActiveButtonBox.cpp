// Fill out your copyright notice in the Description page of Project Settings.


#include "ActiveButtonBox.h"

#include "ActiveButton.h"
#include "Components/TextBlock.h"

UActiveButtonBox::UActiveButtonBox()
{
	//기본값 설정
	bActive = false;

	//토글 색상
	ActiveOnColor = FLinearColor(0.05, 1, 0.05, 1.0);  
	ActiveOffColor = FLinearColor(1, 0.05, 0.05, 1.0);

	//토글 텍스트
	ActiveOnText = FText::FromString(FString(TEXT("Not Ready...")));
	ActiveOffText = FText::FromString(FString(TEXT("Let's Start!")));

	//조건 텍스트
	OnConditionText = FText::FromString(FString(TEXT("")));
	OffConditionText = FText::FromString(FString(TEXT("")));
}

void UActiveButtonBox::SetActive(const bool Value)
{
	//새로운 상태
	bActive = Value;

	//색상 변경
	UpdateBoxAppearance();
		
	//델리게이트 브로드캐스트
	OnActiveStateChanged.Broadcast(bActive);
}

void UActiveButtonBox::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	//활성화 버튼과 조건 텍스트 블록만 가지도록 강제한다
	if (GetChildrenCount() != 2)
	{
		//먼저 전부 버린다
		ClearChildren();
		
		//조건 텍스트 블록 추가
		ConditionTextBlock = NewObject<UTextBlock>(this);
		AddChild(ConditionTextBlock);

		//활성화 버튼 추가
		ActiveButton = NewObject<UButton>(this);
		AddChild(ActiveButton);
	}
	else
	{
		ConditionTextBlock = Cast<UTextBlock>(GetChildAt(0));
		ActiveButton = Cast<UButton>(GetChildAt(1));
	}

	if (ActiveButton)
	{
		//활성화 버튼 바인딩
		ActiveButton->OnClicked.AddDynamic(this, &UActiveButtonBox::HandleClick);
		
		//자식이 없는 경우에만 기본 TextBlock을 생성
		if (ActiveButton->GetChildrenCount() == 0)
		{
			//텍스트 블록 위젯 생성
			ActiveButtonTextBlock = NewObject<UTextBlock>(this);
		
			//UButton은 하나의 자식만 가질 수 있음
			AddChild(ActiveButtonTextBlock);
		}
		else
		{
			//이미 자식이 있다면, 그 자식이 TextBlock인지 확인하고 캐스팅
			ActiveButtonTextBlock = Cast<UTextBlock>(GetChildAt(0));
		}

		//가운데 정렬
		ActiveButtonTextBlock->SetJustification(ETextJustify::Center);
	}
}

void UActiveButtonBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	
	UpdateBoxAppearance();
}

void UActiveButtonBox::PostLoad()
{
	Super::PostLoad();

	UpdateBoxAppearance();
}

void UActiveButtonBox::HandleClick()
{
	//활성화되어 있을 때만 델리게이트 실행
	if (bActive)
	{
		OnActiveButtonClicked.Broadcast();
	}
}

void UActiveButtonBox::UpdateBoxAppearance()
{
	//버튼 스타일을 변경
	if (ActiveButton)
	{
		FButtonStyle NewStyle = ActiveButton->GetStyle();
		const FSlateColor NewColor = bActive ? ActiveOnColor : ActiveOffColor;
		const FSlateColor NewHalfColor = bActive ? ActiveOnColor / 2 : ActiveOffColor / 2; 
		NewStyle.Normal.TintColor = NewHalfColor;
		NewStyle.Hovered.TintColor = NewColor;
		NewStyle.Pressed.TintColor = NewHalfColor;
		ActiveButton->SetStyle(NewStyle);	
	}

	//텍스트 변경
	const FText NewButtonText = bActive ? ActiveOnText : ActiveOffText;
	if (ActiveButtonTextBlock)
	{
		ActiveButtonTextBlock->SetText(NewButtonText);	
	}
	
	//조건 텍스트 변경
	const FText NewConditionText = bActive ? OnConditionText : OffConditionText;
	if (ConditionTextBlock)
	{
		ConditionTextBlock->SetText(NewConditionText);	
	}
}