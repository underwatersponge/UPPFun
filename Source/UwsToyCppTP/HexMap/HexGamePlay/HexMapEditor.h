// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexMapEditor.generated.h"

class UHexCellComponent;
class AHexGrid;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWSTOYCPPTP_API UHexMapEditor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHexMapEditor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// note todo: not good now change that latter uws
	UHexCellComponent* GetCellFromLocation(const FVector& Location);
	void EditCell(const FVector& Position);
	void EditCell(UHexCellComponent* Cell);
	void EditCells(const UHexCellComponent* Center);
	AHexGrid* GetHexGrid() const { return HexGrid; }

	void SetActiveColor(FColor InColor) { ActiveColor= InColor; }
	void SetActiveElevation(float Elevation) { ActiveElevation = Elevation; }
	void SetBrushSize(float Value) { BrushSize = static_cast<int>(Value); }
	void ShowCellLabels(bool Visible);
private:
	//UPROPERTY()
	//class AHexPawn* HexPawn = nullptr;

	UPROPERTY()
	AHexGrid* HexGrid = nullptr;
	
	UPROPERTY()
	FColor ActiveColor = FColor::Yellow;

	UPROPERTY()
	int32 ActiveElevation = 0;

	UPROPERTY()
	int32 BrushSize;
};
