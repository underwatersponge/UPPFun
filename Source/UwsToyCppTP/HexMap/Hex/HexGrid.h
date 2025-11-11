// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UwsToyCppTP/HexMap/Hex/HexCoordinates.h"
#include "HexGrid.generated.h"

class AHexCell;
class AHexMesh;
class AHexPawn;
class AHexGridChunk;
class UHexMeshComponent;
class UHexCellComponent;
class UTextRenderComponent;

UCLASS()
class UWSTOYCPPTP_API AHexGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexGrid();
	//void Refresh(); // uws 10 / 24
	UHexCellComponent* GetCell(FVector Position);
	UHexCellComponent* GetCell(const FHexCoordinates& Coord);
	UTextRenderComponent* GetTextComp(const FHexCoordinates& Coord);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void PostInitializeComponents() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	void Awake();
	void CreateChunks();
	void CreateCells();
	void CreateCell(int32 Y, int32 X, int32 Index);
	void AddCellToChunk(int32 Y, int32 X, UHexCellComponent* Cell);

// utility
	void CheckCellNei(UHexCellComponent* Cell)const;
	void AddCoordVisText(FVector& Position, const UHexCellComponent* Cell);
	void AddCoordVisText(FVector& Position, const FHexCoordinates& HexCoord);
// utility
private:	
	UPROPERTY()
	int32 CellCountAlongAxisY = 0;

	UPROPERTY()
	int32 CellCountAlongAxisX = 0;

public:
	UPROPERTY(EditAnywhere, Category = "Hex Grid")
	int32 ChunkCountAlongAxisY = 4;

	UPROPERTY(EditAnywhere, Category = "Hex Grid")
	int32 ChunkCountAlongAxisX = 3;

	UPROPERTY()
	TArray<UTextRenderComponent*> CellLabels;

	UPROPERTY()
	TArray<UHexCellComponent*> Cells;

	UPROPERTY()
	TArray<AHexGridChunk*> Chunks;

	UPROPERTY(EditAnywhere, Category = "NoiseSouTex")
	TObjectPtr<UTexture2D> NoiseSource;
};
