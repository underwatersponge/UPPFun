// Fill out your copyright notice in the Description page of Project Settings.

#include "UwsToyCppTP/HexMap/Hex/HexGrid.h"
#include "UwsToyCppTP/HexMap/Hex/HexCellComponent.h"
#include "UwsToyCppTP/HexMap/Hex/HexMeshComponent.h"
#include "UwsToyCppTP/HexMap/HexGamePlay/HexPawn.h"
#include "UwsToyCppTP/HexMap/UserWidgets/HexSeleteColor.h"
#include "UwsToyCppTP/HexMap/Hex/HexGridChunk.h"
#include "Components/SceneComponent.h"
#include "Components/TextRenderComponent.h"
#include "Editor.h"
//#include "HexMapPawn.h"
//#include "UI/SetColorWidget.h""
//#include "HexMapHUD.h"

// Sets default values
AHexGrid::AHexGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootCoponent"));
	
	NoiseSource = CreateDefaultSubobject<UTexture2D>(TEXT("NoiseSource"));

	CellCountAlongAxisY = ChunkCountAlongAxisY * FHexMetrics::ChunkSizeX;
	CellCountAlongAxisX = ChunkCountAlongAxisX * FHexMetrics::ChunkSizeY;
}

// Called when the game starts or when spawned
void AHexGrid::BeginPlay()
{
	Super::BeginPlay();

}

void AHexGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AHexGrid::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Awake();
}

void AHexGrid::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void AHexGrid::Awake()
{
	CreateChunks();
	CreateCells();
}

void AHexGrid::CreateChunks()
{
	// We first fill the Chunk which along the axisY in UE <HexInnterRadians Dir>
	Chunks.SetNum(ChunkCountAlongAxisY * ChunkCountAlongAxisX);
	
	int32 i = 0;

	for (int32 X = 0; X < ChunkCountAlongAxisX; X++)
	{
		for (int32 Y = 0; Y < ChunkCountAlongAxisY; Y++)
		{
			AHexGridChunk* Chunk = GetWorld()->SpawnActor<AHexGridChunk>(AHexGridChunk::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			Chunk->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			Chunks[i++] = Chunk;
		}
	}
}

void AHexGrid::CreateCells()
{
#if WITH_EDITOR
	if (GEditor)
	{
		UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();
		if (EditorWorld)
		{
			Cells.SetNum(CellCountAlongAxisY * CellCountAlongAxisX);

			int32 CellIndex = 0;
			for (int32 X = 0; X < CellCountAlongAxisX; X++)
			{
				for (int32 Y = 0; Y < CellCountAlongAxisY; Y++)
				{
					CreateCell(Y, X, CellIndex++);
				}
			}
		}
	}
#endif

}

void AHexGrid::CreateCell(int32 Y, int32 X, int32 Index)
{
	FVector Position;
	Position.X = X * (FHexMetrics::OuterRadius * 1.5f);            
	Position.Y = (Y + X * 0.5f - X / 2) * (FHexMetrics::InnerRadius * 2.0f);
	//Position.Y = (X + Y * 0.5f) * (FHexMetrics::InnerRadius * 2.0f);
	Position.Z = 0.0f;                                             

	UHexCellComponent* Cell = NewObject<UHexCellComponent>(this);
	Cell->Color = FColor::White;
	Cell->Location = Position;
	Cell->Coordinates = FHexCoordinates::FromOffsetCoordinates(Y, X);
	if (Cell)
	{
		Cells[Index] = Cell;
		if (Y > 0)
		{
			Cell->SetNeighbour(EHexDirection::W, Cells[Index - 1]);
		}

		if (X > 0)
		{
			if ((X & 1) == 0)
			{
				Cell->SetNeighbour(EHexDirection::SE, Cells[Index - CellCountAlongAxisY]);
				if (Y > 0)
				{
					Cell->SetNeighbour(EHexDirection::SW, Cells[Index - CellCountAlongAxisY - 1]);
				}
			}
			else
			{
				Cell->SetNeighbour(EHexDirection::SW, Cells[Index - CellCountAlongAxisY]);
				if (Y < CellCountAlongAxisY - 1)
				{
					Cell->SetNeighbour(EHexDirection::SE, Cells[Index - CellCountAlongAxisY + 1]);
				}
			}
		}
	}
	AddCellToChunk(Y, X, Cell);
	AddCoordVisText(Position, FHexCoordinates::FromOffsetCoordinates(Y, X));//AddCoordVisText(Position, Cell);
}

void AHexGrid::AddCellToChunk(int32 Y, int32 X, UHexCellComponent* Cell)
{
	int32 ChunkY = Y / FHexMetrics::ChunkSizeX; 
	int32 ChunkX = X / FHexMetrics::ChunkSizeY;
	AHexGridChunk* Chunk = Chunks[ChunkY + ChunkX * ChunkCountAlongAxisY];

	int32 LocalY = Y - ChunkY * FHexMetrics::ChunkSizeX;
	int32 LocalX = X - ChunkX * FHexMetrics::ChunkSizeY;
	Chunk->AddCell(LocalY + LocalX * FHexMetrics::ChunkSizeX, Cell);
}

UHexCellComponent* AHexGrid::GetCell(FVector Position)
{
	Position -= GetActorLocation();
	FHexCoordinates Coordinates = FHexCoordinates::FromPosition(Position);
	int32 Index = Coordinates.X + Coordinates.Z * CellCountAlongAxisY + Coordinates.Z / 2;

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT(
	//	"X: %d, Z: %d, Index: %d"), Coordinates.X, Coordinates.Z, Index)); // uws test
	if (Cells.IsValidIndex(Index))
	{
		//CheckCellNei(Cells[Index]);// uws test
		return Cells[Index];
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid cell index: %d"), Index);
		return nullptr;
	}

}

UHexCellComponent* AHexGrid::GetCell(const FHexCoordinates& Coord)
{
	int32 Z = Coord.Z;
	if (Z < 0 || Z >= CellCountAlongAxisX)
	{
		return nullptr;
	}
	int32 X = Coord.X + Z / 2;
	if (X < 0 || X >= CellCountAlongAxisY)
	{
		return nullptr;
	}
	return Cells[X + Z * CellCountAlongAxisY];
}

UTextRenderComponent* AHexGrid::GetTextComp(const FHexCoordinates& Coord)
{
	int32 Index = Coord.X + Coord.Z * CellCountAlongAxisY + Coord.Z / 2;
	if (CellLabels.IsValidIndex(Index))
	{
		return CellLabels[Index];
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Invalid celllabel index: %d"), Index);
		return nullptr;
	}
}

// ============== utility =================== //

void AHexGrid::CheckCellNei(UHexCellComponent* Cell) const
{
	// uws test
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("CheckCelNei!!"));// uws
	for (const UHexCellComponent* Nei : Cell->Neighbours)
	{
		if (Nei == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Nei is nullptr"));
			continue;
		}
		FHexCoordinates Coord = Nei->Coordinates;
		int32 Id = Coord.X + Coord.Z * CellCountAlongAxisY + Coord.Z / 2;

		UE_LOG(LogTemp, Warning, TEXT("%d"), Id);
	}
	// uws test
}

void AHexGrid::AddCoordVisText(FVector& Position, const UHexCellComponent* Cell)
{
	Position.Z += 50.f;
	UTextRenderComponent* TextComp = NewObject<UTextRenderComponent>(this);
	TextComp->RegisterComponent();
	TextComp->SetTextRenderColor(FColor::Black);
	TextComp->SetHorizontalAlignment(EHTA_Center);
	TextComp->SetVerticalAlignment(EVRTA_TextCenter);
	TextComp->SetWorldRotation(FRotator(90.f, 180.f, 0.f));
	TextComp->SetWorldLocation(Position);
	TextComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//TextComp->SetRelativeRotation(FRotator(180.f, 0.f, 0.f));
	FString XYZString = FString::Printf(TEXT("X=%d\nY=%d\nZ=%d"), Cell->Coordinates.GetX(), Cell->Coordinates.GetY(), Cell->Coordinates.GetZ());
	TextComp->SetText(FText::FromString(XYZString));
	TextComp->SetVisibility(false);
	CellLabels.Add(TextComp);
}

void AHexGrid::AddCoordVisText(FVector& Position, const FHexCoordinates& HexCoord)
{
	Position.Z += 50.f;
	UTextRenderComponent* TextComp = NewObject<UTextRenderComponent>(this);
	TextComp->RegisterComponent();
	TextComp->SetTextRenderColor(FColor::Black);
	TextComp->SetHorizontalAlignment(EHTA_Center);
	TextComp->SetVerticalAlignment(EVRTA_TextCenter);
	TextComp->SetWorldRotation(FRotator(90.f, 180.f, 0.f));
	TextComp->SetWorldLocation(Position);
	TextComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	FString XYZCoord = FString::Printf(TEXT("X=%d\nY=%d\nZ=%d"), HexCoord.GetX(), HexCoord.GetY(), HexCoord.GetZ());
	TextComp->SetText(FText::FromString(XYZCoord));
	TextComp->SetVisibility(false);
	CellLabels.Add(TextComp);
}
