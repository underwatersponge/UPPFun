// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UwsToyCppTP/HexMap/Hex/HexCoordinates.h"
#include "UwsToyCppTP/HexMap/Hex/HexDirection.h"
#include "HexCellComponent.generated.h"

class USceneComponent;
class AHexGridChunk;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWSTOYCPPTP_API UHexCellComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHexCellComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// the function name is not good you know
	UFUNCTION()
	FVector GetComponentLocation() const { return Location; }
	
	UFUNCTION(BlueprintCallable, Category = "GetHexNei")
	UHexCellComponent* GetNeighbour(EHexDirection Direction)const;

	UFUNCTION(BlueprintCallable, Category = "SetHexNei")
	void SetNeighbour(EHexDirection Direction, UHexCellComponent* Cell);

	UFUNCTION(BlueprintCallable, Category = "HexEdgeType")
	EHexEdgeType GetEdgeType(EHexDirection Direction)const ;

	UFUNCTION(BlueprintCallable, Category = "HexEdgeType", meta = (DisplayName = "EdgeTypeFromAnotherCell"))
	EHexEdgeType GetEdgeTypeFromAnotherCell(const UHexCellComponent* OtherCell)const;

	UFUNCTION()
	void ChangeCellAtts(int32 NewElevation, const FColor& NewCol);

	UFUNCTION()
	void Refresh()const;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	int32 Elevation = 0;

	UPROPERTY()
	FHexCoordinates Coordinates;

	UPROPERTY()
	FColor Color = FColor::White; // to do : stone as FLinearColor?

	UPROPERTY()
	FVector Location = FVector::ZeroVector;
	
	UPROPERTY()
	TArray<UHexCellComponent*> Neighbours;

	UPROPERTY()
	AHexGridChunk* OwnChunk;
};

