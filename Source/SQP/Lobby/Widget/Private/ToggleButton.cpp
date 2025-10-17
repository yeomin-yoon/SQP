// Fill out your copyright notice in the Description page of Project Settings.

#include "ToggleButton.h"

#include "Components/TextBlock.h"

UToggleButton::UToggleButton()
{
	//기본값 설정
	bToggle = false;

	//토글 색상
	ToggledOnColor = FLinearColor(0.2f, 0.8f, 0.2f, 1.0f);  
	ToggledOffColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);

	//토글 텍스트
	ToggleOnText = FText::FromString(FString(TEXT("Cancel...")));
	ToggleOffText = FText::FromString(FString(TEXT("Ready!")));

	//델리게이트 바인딩
	OnClicked.AddDynamic(this, &UToggleButton::HandleClick);
}

void UToggleButton::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();

	// UMG 디자이너에서 사용자가 수동으로 자식을 추가했을 수도 있으므로,
	// 자식이 없는 경우에만 기본 TextBlock을 생성합니다.
	if (GetChildrenCount() == 0)
	{
		// TextBlock 위젯을 생성합니다. 'this'는 이 버튼이 소유자임을 의미합니다.
		ButtonTextBlock = NewObject<UTextBlock>(this);
		// 생성된 TextBlock을 이 버튼의 자식으로 추가합니다.
		// UButton은 하나의 자식만 가질 수 있는 UContentWidget입니다.
		AddChild(ButtonTextBlock);
	}
	else
	{
		// 이미 자식이 있다면, 그 자식이 TextBlock인지 확인하고 캐스팅합니다.
		ButtonTextBlock = Cast<UTextBlock>(GetChildAt(0));
	}

	// ButtonText가 유효하다면 내용을 업데이트합니다.
	if (ButtonTextBlock)
	{
		ButtonTextBlock->SetJustification(ETextJustify::Center); // 가운데 정렬
	}
}

void UToggleButton::SetToggle(const bool Value)
{
	//새로운 상태
	bToggle = Value;

	//색상 변경
	UpdateButtonAppearance();
		
	//델리게이트 브로드캐스트
	OnToggleStateChanged.Broadcast(bToggle);
}

void UToggleButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	UpdateButtonAppearance();
}

void UToggleButton::PostLoad()
{
	Super::PostLoad();
	
	//로드 시에도 색상이 제대로 적용되도록
	UpdateButtonAppearance();
}

void UToggleButton::HandleClick()
{
	//클릭될 때마다 활성화 상태를 반전
	SetToggle(!bToggle);
}

void UToggleButton::UpdateButtonAppearance()
{
	//버튼 스타일을 변경
	FButtonStyle NewStyle = GetStyle();
	const FSlateColor NewColor = bToggle ? ToggledOnColor : ToggledOffColor;
	NewStyle.Normal.TintColor = NewColor;
	NewStyle.Hovered.TintColor = NewColor;
	NewStyle.Pressed.TintColor = NewColor;
	SetStyle(NewStyle);

	//텍스트 변경
	const FText NewText = bToggle ? ToggleOnText : ToggleOffText;
	if (ButtonTextBlock)
	{
		ButtonTextBlock->SetText(NewText);	
	}
}