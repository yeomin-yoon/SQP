// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Kismet/KismetMathLibrary.h"

void UMainUI::NativeConstruct()
{
	Super::NativeConstruct();

	ColorWheelSlider->OnValueChangedX.AddDynamic(this, &UMainUI::OnSliderXChanged);
	SetColorRedBtn->OnClicked.AddDynamic(this, &UMainUI::OnClickRedBtn);
	SetColorGreenBtn->OnClicked.AddDynamic(this, &UMainUI::OnClickGreenBtn);
	SetColorBlueBtn->OnClicked.AddDynamic(this, &UMainUI::OnClickBlueBtn);
	SetColorWhiteBtn->OnClicked.AddDynamic(this, &UMainUI::OnClickWhiteBtn);
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

}

void UMainUI::OnClickGreenBtn()
{
	SetColor(SetColorGreenBtn->GetBackgroundColor());
}

void UMainUI::OnClickBlueBtn()
{
	SetColor(SetColorBlueBtn->GetBackgroundColor());
}

void UMainUI::OnClickWhiteBtn()
{
	SetColor(SetColorWhiteBtn->GetBackgroundColor());
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

FLinearColor UMainUI::GetColor()
{
	float Sat = UKismetMathLibrary::FClamp(
		UKismetMathLibrary::Distance2D(FVector2D(0.5f, 0.5f), ColorWheelSlider->GetValue()) * 2.f, 0, 1);
	return UKismetMathLibrary::HSVToRGB(GetAngle(), Sat, 1, 1);
}

void UMainUI::OnSliderXChanged(float Value)
{
	CurrentColor->SetBrushColor(UpdateColorWheel()); 
	ColorWheelSlider->SetSliderHandleColor(UpdateColorWheel());
}
