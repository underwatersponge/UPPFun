// Fill out your copyright notice in the Description page of Project Settings.


#include "UwsToyCppTP/HexMap/Hex/HexGridChunk.h"
#include "UwsToyCppTP/HexMap/Hex/HexMeshComponent.h"
#include "UwsToyCppTP/HexMap/Hex/HexMetrics.h"

#include "UwsToyCppTP/HexMap/Hex/HexCellComponent.h"
// Sets default values
AHexGridChunk::AHexGridChunk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	HexMesh = CreateDefaultSubobject<UHexMeshComponent>(TEXT("HexMesh"));
	HexMesh->SetupAttachment(RootComponent);

	// ================ //
	//static ConstructorHelpers::FObjectFinder<UMaterialInterface>
	//MaterialFinder( TEXT("/Script/Engine.Material'/Game/Uws/HexMap/Materials/MA_Hex.MA_Hex'"));
	//if (MaterialFinder.Succeeded())
	//{
	//	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(MaterialFinder.Object, this);

	//	HexMesh->SetMaterial(0, DynMaterial);
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Failed to load Material"));
	//}
	// =================== //

	Cells.SetNum(FHexMetrics::ChunkSizeX * FHexMetrics::ChunkSizeY);
}

void AHexGridChunk::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UMaterial* Material = LoadObject<UMaterial>(nullptr, TEXT("/Script/Engine.Material'/Game/Uws/HexMap/Materials/MA_Hex.MA_Hex'"));

	if (Material)
	{
		HexMesh->SetMaterial(0, Material);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Failed to load Material"));
	}
}

// Called when the game starts or when spawned
void AHexGridChunk::BeginPlay()
{
	Super::BeginPlay();
	HexMesh->TriangulateAll(Cells);

	// uws test
	//int32 CellNum = Cells.Num();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("CellNums: %d"), CellNum));
	//UHexCellComponent* Cell = Cells[0];
	//if (!Cell)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("The Cells is zero Array"));
	//}
	//else 
	//{
	//	FVector Location = Cell->GetComponentLocation();
	//	UE_LOG(LogTemp, Warning, TEXT("Pos: %s"), *Location.ToString());
	//	//UE_LOG(LogTemp, Warning, TEXT("CellNum: %d"), CellNum);
	//}
}

// Called every frame
void AHexGridChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//ETickingGroup::TG_PostUpdateWork;

}

void AHexGridChunk::AddCell(int32 Index, UHexCellComponent* Cell)
{
	Cells[Index] = Cell;
	Cell->OwnChunk = this;
}

void AHexGridChunk::Refresh() const
{
	HexMesh->TriangulateAll(Cells);
}

