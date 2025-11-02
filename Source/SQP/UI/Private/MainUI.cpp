// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "LikeUI.h"
#include "SQP_PC_PaintRoom.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "UI/Synth2DSlider.h"
#include "Kismet/KismetMathLibrary.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	ColorWheelSlider->OnValueChangedX.AddDynamic(this, &UMainUI::OnSliderXChanged);
	SetColorRedBtn->OnClicked.AddDynamic(this, &UMainUI::OnClickRedBtn);
	SetColorGreenBtn->OnClicked.AddDynamic(this, &UMainUI::OnClickGreenBtn);
	SetColorBlueBtn->OnClicked.AddDynamic(this, &UMainUI::OnClickBlueBtn);
	SetColorWhiteBtn->OnClicked.AddDynamic(this, &UMainUI::OnClickWhiteBtn);
	SetColorBlackBtn->OnClicked.AddDynamic(this, &UMainUI::OnClickBlackBtn);
	BrushSlider->OnValueChanged.AddDynamic(this, &UMainUI::OnBrushSizeChanged);

	WBP_LikeUI->LikeBtn->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UMainUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float ValueAX = ColorWheelSlider->GetValue().X;
	float ValueBX = UKismetMathLibrary::DegCos(GetAngle()) * 0.5f + 0.5f;
	float ValueX = UKismetMathLibrary::SelectFloat(ValueAX, ValueBX, IsInRadius());

	float ValueAY = ColorWheelSlider->GetValue().Y;
	float ValueBY = UKismetMathLibrary::DegSin(GetAngle()) * 0.5f + 0.5f;
	float ValueY = UKismetMathLibrary::SelectFloat(ValueAY, ValueBY, IsInRadius());

	ColorWheelSlider->SetValue(FVector2D(ValueX, ValueY));
}

FLinearColor UMainUI::GetBorderColor()
{
	return CurrentColor->GetBrushColor();
}

float UMainUI::GetAngle()
{
	FVector2D Hue = ColorWheelSlider->GetValue() - FVector2D(0.5f, 0.5f);
	return UKismetMathLibrary::ClampAxis(UKismetMathLibrary::DegAtan2(Hue.Y, Hue.X));
}

bool UMainUI::IsInRadius()
{
	return UKismetMathLibrary::Distance2D(FVector2D(0.5f, 0.5f), ColorWheelSlider->GetValue()) < 0.5f;
}

FLinearColor UMainUI::UpdateColorWheel()
{
	float Sat = UKismetMathLibrary::FClamp(
		UKismetMathLibrary::Distance2D(FVector2D(0.5f, 0.5f), ColorWheelSlider->GetValue()) * 2.f, 0, 1);
	return UKismetMathLibrary::HSVToRGB(GetAngle(), Sat, 1, 1);
}

void UMainUI::OnClickRedBtn()
{
	SetColor(SetColorRedBtn->GetBackgroundColor());

	UpdatePlayerState();
}

void UMainUI::OnClickGreenBtn()
{
	SetColor(SetColorGreenBtn->GetBackgroundColor());

	UpdatePlayerState();
}

void UMainUI::OnClickBlueBtn()
{
	SetColor(SetColorBlueBtn->GetBackgroundColor());

	UpdatePlayerState();
}

void UMainUI::OnClickWhiteBtn()
{
	SetColor(SetColorWhiteBtn->GetBackgroundColor());

	UpdatePlayerState();
}

void UMainUI::OnClickBlackBtn()
{
	SetColor(SetColorBlackBtn->GetBackgroundColor());

	UpdatePlayerState();
}

void UMainUI::SetColor(FLinearColor Color)
{
	float Hue, Sat, Val, Alpha;
	UKismetMathLibrary::RGBToHSV(Color ,Hue, Sat, Val, Alpha);
	float ValueX = UKismetMathLibrary::DegCos(Hue) * Sat + 0.5f;
	float ValueY = UKismetMathLibrary::DegSin(Hue) * Sat + 0.5f;
	ColorWheelSlider->SetValue(FVector2D(ValueX, ValueY));

	CurrentColor->SetBrushColor(Color);
	ColorWheelSlider->SetSliderHandleColor(Color);
}

void UMainUI::OnSliderXChanged(float Value)
{
	CurrentColor->SetBrushColor(UpdateColorWheel()); 
	ColorWheelSlider->SetSliderHandleColor(UpdateColorWheel());

	UpdatePlayerState();
}

void UMainUI::OnBrushSizeChanged(float Value)
{
	Cast<ASQP_PC_PaintRoom>(GetWorld()->GetFirstPlayerController())->Server_ChangeBrushSize(Value);
}

void UMainUI::UpdatePlayerState()
{
	Cast<ASQP_PC_PaintRoom>(GetWorld()->GetFirstPlayerController())->Server_PaintColorChange(GetBorderColor());
}
