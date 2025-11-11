// Fill out your copyright notice in the Description page of Project Settings.


#include "UwsToyCppTP/HexMap/Hex/HexCellComponent.h"
#include "UwsToyCppTP/HexMap/Hex/HexGridChunk.h"
// Sets default values for this component's properties
UHexCellComponent::UHexCellComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	Neighbours.SetNum(6);
}


// Called when the game starts
void UHexCellComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHexCellComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UHexCellComponent* UHexCellComponent::GetNeighbour(EHexDirection Direction) const
{
	return Neighbours[static_cast<int>(Direction)];
}

void UHexCellComponent::SetNeighbour(EHexDirection Direction, UHexCellComponent* Cell)
{
	// Neighbours relationships are bidirectional and for that 
	// we can avoid SetNeighbour before who did not be created
	Neighbours[static_cast<int>(Direction)] = Cell;
	Cell->Neighbours[static_cast<int>(UHexDirection::Opposite(Direction))] = this;
}

EHexEdgeType UHexCellComponent::GetEdgeType(EHexDirection Direction) const 
{
	return FHexMetrics::GetEdgeType(Elevation, Neighbours[static_cast<int>(Direction)]->Elevation);
}

EHexEdgeType UHexCellComponent::GetEdgeTypeFromAnotherCell(const UHexCellComponent* OtherCell) const
{
	return FHexMetrics::GetEdgeType(Elevation, OtherCell->Elevation);
}

void UHexCellComponent::ChangeCellAtts(int32 NewElevation, const FColor& NewCol)
{
	Elevation = NewElevation;
	Color = NewCol;
	Location.Z = NewElevation * FHexMetrics::ElevationStep;
}

void UHexCellComponent::Refresh() const
{
	if (OwnChunk)
	{
		OwnChunk->Refresh();
		for (int32 i = 0; i < Neighbours.Num(); i++)
		{
			UHexCellComponent* NeiCell = Neighbours[i];
			if (NeiCell && NeiCell->OwnChunk)
			{
				NeiCell->OwnChunk->Refresh();
			}
		}
	}
}



