// Fill out your copyright notice in the Description page of Project Settings.

#include "UwsToyCppTP/HexMap/UserWidgets/HexSeleteColor.h"
#include "Components/CheckBox.h"
#include "Components/Slider.h"

void UHexSeleteColor::NativeConstruct()
{
	Super::NativeConstruct();

	CheckBoxRed = Cast<UCheckBox>(GetWidgetFromName(TEXT("CheckBoxRed")));
	if (CheckBoxRed)
	{
		CheckBoxRed->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnCheckBoxRedChanged);
	}

	CheckBoxGreen = Cast<UCheckBox>(GetWidgetFromName(TEXT("CheckBoxGreen")));
	if (CheckBoxGreen)
	{
		CheckBoxGreen->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnCheckBoxGreenChanged);
	}

	CheckBoxBlue = Cast<UCheckBox>(GetWidgetFromName(TEXT("CheckBoxBlue")));
	if (CheckBoxBlue)
	{
		CheckBoxBlue->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnCheckBoxBlueChanged);
	}

	ElevationSlider = Cast<USlider>(GetWidgetFromName(TEXT("ElevationSlider")));
	if (ElevationSlider)
	{
		ElevationSlider->SetOrientation(EOrientation::Orient_Horizontal);
		ElevationSlider->SetMinValue(0.0f);
		ElevationSlider->SetMaxValue(6.0f);
		ElevationSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnEleSliderValueChanged);
	}

	BrushSizeSlider = Cast<USlider>(GetWidgetFromName(TEXT("BrushSizeSlider")));
	if (BrushSizeSlider)
	{
		BrushSizeSlider->SetOrientation(EOrientation::Orient_Horizontal);
		BrushSizeSlider->SetMinValue(0.0f);
		BrushSizeSlider->SetMaxValue(7.0f);
		BrushSizeSlider->OnValueChanged.AddDynamic(this, &ThisClass::OnBrushSizeSliderValueChanged);
	}

	CheckBoxCellLabelsVisible = Cast<UCheckBox>(GetWidgetFromName(TEXT("CheckBoxCellLabelsVisible")));
	if (CheckBoxCellLabelsVisible)
	{
		CheckBoxCellLabelsVisible->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnCheckBoxCellLabelsVisibleChanged);
	}
}

void UHexSeleteColor::OnCheckBoxRedChanged(bool bIsChecked)
{
	if (!CurrentCheckBox) CurrentCheckBox = CheckBoxRed;
	if (CurrentCheckBox == CheckBoxRed) return;

	CurrentCheckBox->SetIsChecked(false);
	CurrentCheckBox = CheckBoxRed;
	DelColorCheckBoxChanged.ExecuteIfBound(FColor::Red);

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Red CheckState Changed"));
}

void UHexSeleteColor::OnCheckBoxGreenChanged(bool bIsChecked)
{
	if (!CurrentCheckBox) CurrentCheckBox = CheckBoxGreen;
	if (CurrentCheckBox == CheckBoxGreen) return;

	CurrentCheckBox->SetIsChecked(false);
	CurrentCheckBox = CheckBoxGreen;
	DelColorCheckBoxChanged.ExecuteIfBound(FColor::Green);

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Green CheckState Changed"));

}

void UHexSeleteColor::OnCheckBoxBlueChanged(bool bIsChecked)
{
	if (!CurrentCheckBox) CurrentCheckBox = CheckBoxBlue;
	if (CurrentCheckBox == CheckBoxBlue) return;

	CurrentCheckBox->SetIsChecked(false);
	CurrentCheckBox = CheckBoxBlue;
	DelColorCheckBoxChanged.ExecuteIfBound(FColor::Blue);

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Blue CheckState Changed"));

}

void UHexSeleteColor::OnEleSliderValueChanged(float Value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Slider Value: %f"), Value));
	DelElevationSliderChanged.ExecuteIfBound(Value);
}

void UHexSeleteColor::OnBrushSizeSliderValueChanged(float Value)
{
	DelBrushSizeSliderChanged.ExecuteIfBound(Value);
}

void UHexSeleteColor::OnCheckBoxCellLabelsVisibleChanged(bool bVisible)
{
	DelCellLabelVisibility.ExecuteIfBound(bVisible);
}
