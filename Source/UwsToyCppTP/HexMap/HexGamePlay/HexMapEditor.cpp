// Fill out your copyright notice in the Description page of Project Settings.


#include "UwsToyCppTP/HexMap/HexGamePlay/HexMapEditor.h"
#include "UwsToyCppTP/HexMap/Hex/HexCellComponent.h"
#include "UwsToyCppTP/HexMap/Hex/HexGrid.h"
#include "UwsToyCppTP/HexMap/UserWidgets/HexSeleteColor.h"
#include "UwsToyCppTP/HexMap/Hex/HexGridChunk.h"
#include "UwsToyCppTP/HexMap/HexGamePlay/HexPawn.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHexMapEditor::UHexMapEditor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHexMapEditor::BeginPlay()
{
	Super::BeginPlay();
	
	// test//
	UE_LOG(LogTemp, Warning, TEXT("FName: %s"), *GetFName().ToString());
	TArray<UObject*> Instances;
	GetArchetypeInstances(Instances);
	UE_LOG(LogTemp, Warning, TEXT("num of chain list: %d"), Instances.Num());
	for (const auto ins : Instances)
	{
		UE_LOG(LogTemp, Warning, TEXT("archetype chain list: %s"), ins->GetFName());
	}

	// test //
	HexGrid = Cast<AHexGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), AHexGrid::StaticClass()));
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		AHexPawn* HexPawn = Cast<AHexPawn>(PlayerController->GetPawn());
		UHexSeleteColor* HexSelectWidget = HexPawn->UWidgetSelColor;
		HexSelectWidget->DelColorCheckBoxChanged.BindUObject(this, &ThisClass::SetActiveColor);
		HexSelectWidget->DelElevationSliderChanged.BindUObject(this, &ThisClass::SetActiveElevation);
		HexSelectWidget->DelBrushSizeSliderChanged.BindUObject(this, &ThisClass::SetBrushSize);
		HexSelectWidget->DelCellLabelVisibility.BindUObject(this, &ThisClass::ShowCellLabels);
	}

}

// Called every frame
void UHexMapEditor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

UHexCellComponent* UHexMapEditor::GetCellFromLocation(const FVector& Location)
{
	return GetHexGrid()->GetCell(Location);
}

void UHexMapEditor::EditCell(const FVector& Position)
{	
	UHexCellComponent* Cell = GetHexGrid()->GetCell(Position);
	FColor OldCol = Cell->Color;
	int32 OldElevation = Cell->Elevation;
	if (OldCol == ActiveColor && OldElevation == ActiveElevation)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("color and ele is same"));// uws
		return;
	}
	Cell->ChangeCellAtts(ActiveElevation, ActiveColor);

	UTexture2D* SampleTex = HexGrid->NoiseSource;
	// useless
	//Cell->Location.Z += (FHexMetrics::SampleNoiseTest(Position, SampleTex).Z * 2.f - 1.f) * FHexMetrics::ElevationPerturbStrength;
	Cell->Refresh();

	UTextRenderComponent* TextComp = HexGrid->GetTextComp(Cell->Coordinates);
	FVector Location = TextComp->GetRelativeLocation();
	Location.Z = FMath::Max(Location.Z, ActiveElevation * FHexMetrics::ElevationStep + 10.f);
	TextComp->SetRelativeLocation(Location);
}

void UHexMapEditor::EditCell(UHexCellComponent* Cell)
{
	if (!Cell)
	{
		return;
	}

	FColor OldCol = Cell->Color;
	int32 OldElevation = Cell->Elevation;
	if (OldCol == ActiveColor && OldElevation == ActiveElevation)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("color and ele is same"));// uws
		return;
	}
	Cell->ChangeCellAtts(ActiveElevation, ActiveColor);
	UTexture2D* SampleTex = HexGrid->NoiseSource;
	// useless
	//Cell->Location.Z += (FHexMetrics::SampleNoiseTest(Position, SampleTex).Z * 2.f - 1.f) * FHexMetrics::ElevationPerturbStrength;
	Cell->Refresh();

	UTextRenderComponent* TextComp = HexGrid->GetTextComp(Cell->Coordinates);
	FVector Location = TextComp->GetRelativeLocation();
	Location.Z = FMath::Max(Location.Z, ActiveElevation * FHexMetrics::ElevationStep + 10.f);
	TextComp->SetRelativeLocation(Location);
}

// note todo splite the cell refresh and edit .uws
void UHexMapEditor::EditCells(const UHexCellComponent* Center)
{
	int32 CenterX = Center->Coordinates.X;
	int32 CenterZ = Center->Coordinates.Z;

	for (int32 R = 0, Z = CenterZ - BrushSize; Z <= CenterZ; Z++, R++)
	{
		for (int32 X = CenterX - R; X <= CenterX + BrushSize; X++)
		{
			// note to do: two way<something is wrong the second way> 
			// because the cells maybe have onw in the bound of a chunk!!!

			UHexCellComponent* Cell = GetHexGrid()->GetCell(FHexCoordinates(X, Z));
			EditCell(Cell);
		}
	}

	for (int32 R = 0, Z = CenterZ + BrushSize; Z > CenterZ; Z--, R++)
	{
		for (int32 X = CenterX - BrushSize; X <= CenterX + R; X++)
		{
			UHexCellComponent* Cell = GetHexGrid()->GetCell(FHexCoordinates(X, Z));
			EditCell(Cell);
		}
	}

}

void UHexMapEditor::ShowCellLabels(bool Visible)
{
	for (const auto CellLabel : HexGrid->CellLabels)
	{
		CellLabel->SetVisibility(Visible);
	}
}

