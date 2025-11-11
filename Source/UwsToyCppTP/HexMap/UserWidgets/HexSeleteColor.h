// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HexSeleteColor.generated.h"

DECLARE_DELEGATE_OneParam(FColorCheckBoxChanged, FColor);
DECLARE_DELEGATE_OneParam(FElevationSliderChanged, float);
DECLARE_DELEGATE_OneParam(FBrushSizeSliderChanged, float);
DECLARE_DELEGATE_OneParam(FCellLabelVisibility, bool);

class UCheckBox;
class USlider;

// have a thing not right now
UCLASS()
class UWSTOYCPPTP_API UHexSeleteColor : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBoxRed;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBoxGreen;
	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBoxBlue;
	UPROPERTY()
	UCheckBox* CurrentCheckBox;

	UPROPERTY(meta = (BindWidget))
	USlider* ElevationSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* BrushSizeSlider;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* CheckBoxCellLabelsVisible;

public:
	FColorCheckBoxChanged DelColorCheckBoxChanged;
	FElevationSliderChanged DelElevationSliderChanged;
	FBrushSizeSliderChanged DelBrushSizeSliderChanged;
	FCellLabelVisibility DelCellLabelVisibility;
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCheckBoxRedChanged(bool bIsChecked);

	UFUNCTION()
	void OnCheckBoxGreenChanged(bool bIsChecked);

	UFUNCTION()
	void OnCheckBoxBlueChanged(bool bIsChecked);

	UFUNCTION()
	void OnEleSliderValueChanged(float Value);

	UFUNCTION()
	void OnBrushSizeSliderValueChanged(float Value);

	UFUNCTION()
	void OnCheckBoxCellLabelsVisibleChanged(bool bVisible);
};
