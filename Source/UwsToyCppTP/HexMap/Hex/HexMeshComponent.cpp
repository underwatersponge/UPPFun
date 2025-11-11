// Fill out your copyright notice in the Description page of Project Settings.

#include "UwsToyCppTP/HexMap/Hex/HexMeshComponent.h"
#include "UwsToyCppTP/HexMap/Hex/HexCellComponent.h"
#include "UwsToyCppTP/HexMap/Hex/HexGrid.h"
#include "UwsToyCppTP/HexMap/Hex/EdgeVertices.h"
//#define UsePerturb;

void UHexMeshComponent::TriangulateAll(const TArray<UHexCellComponent*>& Cells)
{
	Vertices.Empty();
	Triangles.Empty();
	Colors.Empty();

	for (UHexCellComponent* Cell : Cells)
	{
		Triangulate(Cell);
	}

	//CreateMeshSection(
	//	0,
	//	Vertices,
	//	Triangles,
	//	TArray<FVector>(), // Normal
	//	TArray<FVector2D>(), // UV
	//	Colors,
	//	TArray<FProcMeshTangent>(), // Tangents
	//	true // Create Collision
	//);

	// FLinearColor storage precomp as float, FColor storage precomp as uint8<0-255>
	// FlinearColor provide many operator function but FColor only operator +=
	//*@param	bSRGBConversion		Whether to do sRGB conversion when converting FLinearColor to FColor
	CreateMeshSection_LinearColor(
		0,
		Vertices,
		Triangles,
		TArray<FVector>(), // Normal
		TArray<FVector2D>(),// UV
		Colors,// Vert Color
		TArray<FProcMeshTangent>(), // Tangents
		true, // Create Collision
		false); // bSRGBConversion 
}

void UHexMeshComponent::Triangulate(UHexCellComponent* Cell)
{
	if (Cell == nullptr) return;

	// Hex Triangulate count : 6
	for (int32 i = 0; i < 6; i++) {
		Triangulate(i, Cell);
	}
}

// Index: 0->NE, 1->E, 2->SE, 3->SW, 4->W, 5->NW
void UHexMeshComponent::Triangulate(int32 Index, UHexCellComponent* Cell)
{
	FVector Center = Cell->GetComponentLocation();
	FVector V1 = Center + FHexMetrics::GetFirstSolidCorner(Index);
	FVector V2 = Center + FHexMetrics::GetSecondSolidCorner(Index);
	const FEdgeVertices E{ Center + FHexMetrics::GetFirstSolidCorner(Index),
					Center + FHexMetrics::GetSecondSolidCorner(Index) };
	
	// maybe Cell member Color should be stoned as LinearColor?
	FLinearColor OwnCol = (Cell->Color).ReinterpretAsLinear();

	TriangulateEdgeFun(Center, E, OwnCol);

	if (Index < 3) {
		TriangulateConnection(Index, Cell, E);
	}
}

void UHexMeshComponent::TriangulateEdgeFun(const FVector& Center, const FEdgeVertices& Edge, const FLinearColor& Col)
{
	AddTriangle(Center, Edge.V1, Edge.V2);
	AddTriangleColor(Col);
	AddTriangle(Center, Edge.V2, Edge.V3);
	AddTriangleColor(Col);
	AddTriangle(Center, Edge.V3, Edge.V4);
	AddTriangleColor(Col);
}

void UHexMeshComponent::TriangulateEdgeStrip(const FEdgeVertices& E1, const FLinearColor& Col1, const FEdgeVertices& E2, const FLinearColor& Col2)
{
	AddQuad(E1.V1, E1.V2, E2.V1, E2.V2);
	AddQuadColor(Col1, Col2);
	AddQuad(E1.V2, E1.V3, E2.V2, E2.V3);
	AddQuadColor(Col1, Col2);
	AddQuad(E1.V3, E1.V4, E2.V3, E2.V4);
	AddQuadColor(Col1, Col2);
}

void UHexMeshComponent::TriangulateConnection(int32 Index, UHexCellComponent* Cell, const FEdgeVertices& E1)
{
	EHexDirection HexCrossDir = static_cast<EHexDirection>(Index);
	UHexCellComponent* Nei = Cell->GetNeighbour(HexCrossDir);
	if (Nei == nullptr)
	{
		return;
	}
	FLinearColor OwnCol = Cell->Color.ReinterpretAsLinear();
	FLinearColor NeiCol = Nei->Color.ReinterpretAsLinear();

	FVector Bridge = FHexMetrics::GetBridge(Index);
	
	Bridge.Z = Nei->GetComponentLocation().Z - Cell->GetComponentLocation().Z;
	const FEdgeVertices E2{E1.V1 + Bridge, E1.V4 + Bridge};

	if (Cell->GetEdgeType(static_cast<EHexDirection>(Index)) == EHexEdgeType::Slope) {
		TriangulateEdgeTerraces(E1, Cell, E2, Nei);
	}
	else
	{
		TriangulateEdgeStrip(E1, OwnCol, E2, NeiCol);
	}

	UHexCellComponent* NextNei = Cell->GetNeighbour(UHexDirection::Next(HexCrossDir));
	if (Index <= 1 && NextNei != nullptr)
	{
		FLinearColor NextCol = NextNei->Color.ReinterpretAsLinear();
		EHexDirection a = UHexDirection::Next(HexCrossDir);
		FVector V5 = E1.V4 + FHexMetrics::GetBridge((int)a);
		//V5.Z = NextNei->Elevation * FHexMetrics::ElevationStep;
		V5.Z = NextNei->GetComponentLocation().Z;

		if (Cell->Elevation <= Nei->Elevation) {
			if (Cell->Elevation <= NextNei->Elevation)
			{
				TriangulateCorner(E1.V4, Cell, E2.V4, Nei, V5, NextNei);
			}
			else
			{
				TriangulateCorner(V5, NextNei, E1.V4, Cell, E2.V4, Nei);
			}
		}
		else if (Nei->Elevation < NextNei->Elevation)
		{
			TriangulateCorner(E2.V4, Nei, V5, NextNei, E1.V4, Cell);
		}
		else
		{
			TriangulateCorner(V5, NextNei, E1.V4, Cell, E2.V4, Nei);
		}
		//AddTriangle(V2, V4, V5);
		//AddTriangleColor(OwnCol, NeiCol, NextCol);
	}
}

void UHexMeshComponent::TriangulateEdgeTerraces(const FEdgeVertices& Begin, const UHexCellComponent* BeginCell,
					const FEdgeVertices& End, const UHexCellComponent* EndCell)
{
	FLinearColor BeginCol = (BeginCell->Color).ReinterpretAsLinear();
	FLinearColor EndCol = (EndCell->Color).ReinterpretAsLinear();
	
	FLinearColor C2 = FHexMetrics::TerraceLerp(BeginCol, EndCol, 1);
	FEdgeVertices E2 = FEdgeVertices::Terracelerp(Begin, End, 1);
	TriangulateEdgeStrip(Begin, BeginCol, E2, C2);

	for (int32 i = 2; i < FHexMetrics::TerracesStpes; i++)
	{
		FEdgeVertices E1 = E2;
		FLinearColor C1 = C2;
		E2 = FEdgeVertices::Terracelerp(Begin, End, i);
		C2 = FHexMetrics::TerraceLerp(BeginCol, EndCol, i);
		TriangulateEdgeStrip(E1, C1, E2, C2);
	}
	TriangulateEdgeStrip(E2, C2, End, EndCol);
	
	//FVector begin1 = Begin.V1;
	//FVector begin2 = Begin.V4;
	//FVector end1 = End.V1;
	//FVector end2 = End.V4;
	//FVector V3 = FHexMetrics::TerraceLerp(begin1, end1, 1);
	//FVector V4 = FHexMetrics::TerraceLerp(begin2, end2, 1);
	//AddQuad(begin1, begin2, V3, V4);
	//AddQuadColor(FLinearColor::Black, FLinearColor::Black);
	//for (int32 i = 2; i < FHexMetrics::TerracesStpes; i++)
	//{
	//	FVector V1 = V3;
	//	FVector V2 = V4;
	//	V3 = FHexMetrics::TerraceLerp(begin1, end1, i);
	//	V4 = FHexMetrics::TerraceLerp(begin2, end2, i);
	//	AddQuad(V1, V2, V3, V4);
	//	AddQuadColor(FLinearColor::Black, FLinearColor::Black);
	//}
	//AddQuad(V3, V4, end1, end2);
	//AddQuadColor(FLinearColor::Black, FLinearColor::Black);
}

void UHexMeshComponent::TriangulateCorner(const FVector& Bottom, const UHexCellComponent* BottomCell, const FVector& Left, const UHexCellComponent* LeftCell, const FVector& Right, const UHexCellComponent* RightCell)
{
	const FLinearColor BottomCol = BottomCell->Color.ReinterpretAsLinear();
	const FLinearColor LeftCol = LeftCell->Color.ReinterpretAsLinear();
	const FLinearColor RightCol = RightCell->Color.ReinterpretAsLinear();

	EHexEdgeType LeftEdgeType = BottomCell->GetEdgeTypeFromAnotherCell(LeftCell);
	EHexEdgeType RightEdgeType = BottomCell->GetEdgeTypeFromAnotherCell(RightCell);

	if (LeftEdgeType == EHexEdgeType::Slope)
	{
		if (RightEdgeType == EHexEdgeType::Slope)
		{
			// SSF
			TriangulateCornerTerraces(Bottom, BottomCell, Left, LeftCell, Right, RightCell);
		}
		else if (RightEdgeType == EHexEdgeType::Flat)
		{
			// SFS FSS
			TriangulateCornerTerraces(Left, LeftCell, Right, RightCell, Bottom, BottomCell);
		}
		else
		{
			// SCS SCC
			TriangulateCornerTerracesCliff(Bottom, BottomCell, Left, LeftCell, Right, RightCell);
		}
	}
	else if (RightEdgeType == EHexEdgeType::Slope)
	{
		if (LeftEdgeType == EHexEdgeType::Flat)
		{
			// SSF
			TriangulateCornerTerraces(Right, RightCell, Bottom, BottomCell, Left, LeftCell);
		}
		else 
		{
			// SCS SCC
			TriangulateCornerCliffTerraces(Bottom, BottomCell, Left, LeftCell, Right, RightCell);
		}
	}
	else if (LeftCell->GetEdgeTypeFromAnotherCell(RightCell) == EHexEdgeType::Slope)
	{
		if (LeftCell->Elevation < RightCell->Elevation)
		{
			// CCSR
			TriangulateCornerCliffTerraces(Right, RightCell, Bottom, BottomCell, Left, LeftCell);
		}
		else 
		{
			// CCSL
			TriangulateCornerTerracesCliff(Left, LeftCell, Right, RightCell, Bottom, BottomCell);
		}
	}

	else 
	{
		//remaining: FFF CCF CCCR CCCL
		AddTriangle(Bottom, Left, Right);
		AddTriangleColor(BottomCol, LeftCol, RightCol);
	}
}

void UHexMeshComponent::TriangulateCornerTerraces(const FVector& Begin, const UHexCellComponent* BeginCell, const FVector& Left, const UHexCellComponent* LeftCell, const FVector& Right, const UHexCellComponent* RightCell)
{
	const FLinearColor BeginCol = BeginCell->Color.ReinterpretAsLinear();
	const FLinearColor LeftCol = LeftCell->Color.ReinterpretAsLinear();
	const FLinearColor RightCol = RightCell->Color.ReinterpretAsLinear();

	FLinearColor C3 = FHexMetrics::TerraceLerp(BeginCol, LeftCol, 1);
	FLinearColor C4 = FHexMetrics::TerraceLerp(BeginCol, RightCol, 1);
	FVector V3 = FHexMetrics::TerraceLerp(Begin, Left, 1);
	FVector V4 = FHexMetrics::TerraceLerp(Begin, Right, 1);
	AddTriangle(Begin, V3, V4);
	AddTriangleColor(BeginCol, C3, C4);

	for (int32 i = 2; i < FHexMetrics::TerracesStpes; i++)
	{
		FLinearColor C1 = C3;
		FLinearColor C2 = C4;
		FVector V1 = V3;
		FVector V2 = V4;

		V3 = FHexMetrics::TerraceLerp(Begin, Left, i);
		V4 = FHexMetrics::TerraceLerp(Begin, Right, i);
		C3 = FHexMetrics::TerraceLerp(BeginCol, LeftCol, i);
		C4 = FHexMetrics::TerraceLerp(BeginCol, RightCol, i);

		AddQuad(V1, V2, V3, V4);
		AddQuadColor(C1, C2, C3, C4);
	}
	AddQuad(V3, V4, Left, Right);
	AddQuadColor(C3, C4, LeftCol, RightCol);
}

void UHexMeshComponent::TriangulateCornerTerracesCliff(const FVector& Begin, const UHexCellComponent* BeginCell, const FVector& Left, const UHexCellComponent* LeftCell, const FVector& Right, const UHexCellComponent* RightCell)
{
	const FLinearColor BeginCol = BeginCell->Color.ReinterpretAsLinear();
	const FLinearColor LeftCol = LeftCell->Color.ReinterpretAsLinear();
	const FLinearColor RightCol = RightCell->Color.ReinterpretAsLinear();

	float b = 1.0 / (RightCell->Elevation - BeginCell->Elevation);
	if (b < 0.f) b = -b;
	//FVector Boundary = FMath::Lerp(Begin, Right, b);
	FVector Boundary = FMath::Lerp(Perturb(Begin), Perturb(Right), b);
	FLinearColor BoundaryCol = FMath::Lerp(BeginCol, RightCol, b);

	TriangulateBoundaryTriangle(Begin, BeginCell, Left, LeftCell, Boundary, BoundaryCol);

	if (LeftCell->GetEdgeTypeFromAnotherCell(RightCell) == EHexEdgeType::Slope)
	{
		TriangulateBoundaryTriangle(Left, LeftCell, Right, RightCell, Boundary, BoundaryCol);
	}
	else
	{
		//AddTriangle(Left, Right, Boundary);
		AddTriangleUnperturbed(Perturb(Left), Perturb(Right), Boundary);
		AddTriangleColor(LeftCol, RightCol, BoundaryCol);
	}

}
void UHexMeshComponent::TriangulateCornerCliffTerraces(const FVector& Begin, const UHexCellComponent* BeginCell, const FVector& Left, const UHexCellComponent* LeftCell, const FVector& Right, const UHexCellComponent* RightCell)
{
	const FLinearColor BeginCol = BeginCell->Color.ReinterpretAsLinear();
	const FLinearColor LeftCol = LeftCell->Color.ReinterpretAsLinear();
	const FLinearColor RightCol = RightCell->Color.ReinterpretAsLinear();

	float b = 1.0 / (LeftCell->Elevation - BeginCell->Elevation);
	if (b < 0.f) b = -b;
	//FVector Boundary = FMath::Lerp(Begin, Left, b);
	FVector Boundary = FMath::Lerp(Perturb(Begin), Perturb(Left), b);
	FLinearColor BoundaryCol = FMath::Lerp(BeginCol, LeftCol, b);

	TriangulateBoundaryTriangle(Right, RightCell, Begin, BeginCell, Boundary, BoundaryCol);

	if (LeftCell->GetEdgeTypeFromAnotherCell(RightCell) == EHexEdgeType::Slope)
	{
		TriangulateBoundaryTriangle(Left, LeftCell, Right, RightCell, Boundary, BoundaryCol);
	}
	else
	{
		//AddTriangle(Left, Right, Boundary);
		AddTriangleUnperturbed(Perturb(Left), Perturb(Right), Boundary);
		AddTriangleColor(LeftCol, RightCol, BoundaryCol);
	}
}
// note to do : maybe not transfer UHexCellComponent* ; just transger FLinearCol
void UHexMeshComponent::TriangulateBoundaryTriangle(const FVector& Begin, const UHexCellComponent* BeginCell, const FVector& Left, const UHexCellComponent* LeftCell, const FVector& Boundary, const FLinearColor& BoundaryCol)
{
	const FLinearColor BeginCol = BeginCell->Color.ReinterpretAsLinear();
	const FLinearColor LeftCol = LeftCell->Color.ReinterpretAsLinear();

	FVector V2 = Perturb(FHexMetrics::TerraceLerp(Begin, Left, 1));
	FLinearColor C2 = FHexMetrics::TerraceLerp(BeginCol, LeftCol, 1);

	//AddTriangle(Begin, V2, Boundary);
	AddTriangleUnperturbed(Perturb(Begin), V2, Boundary);
	AddTriangleColor(BeginCol, C2, BoundaryCol);

	for (int32 i = 2; i < FHexMetrics::TerracesStpes; i++)
	{
		FLinearColor C1 = C2;
		FVector V1 = V2;
		V2 = Perturb(FHexMetrics::TerraceLerp(Begin, Left, i));
		C2 = FHexMetrics::TerraceLerp(BeginCol, LeftCol, i);
		//AddTriangle(V1, V2, Boundary);
		AddTriangleUnperturbed(V1, V2, Boundary);
		AddTriangleColor(C1, C2, BoundaryCol);
	}

	//AddTriangle(V2, Left, Boundary);
	AddTriangleUnperturbed(V2, Perturb(Left), Boundary);
	AddTriangleColor(C2, LeftCol, BoundaryCol);
}

FVector UHexMeshComponent::Perturb(const FVector& WorldPos) const
{
#ifdef UsePerturb
	AActor* Owner = GetOwner();
	if (Cast<AHexGrid>(Owner))
	{
		UTexture2D* SampleTex = Cast<AHexGrid>(Owner)->NoiseSource;
		if (!SampleTex)
		{
			UE_LOG(LogTemp, Warning, TEXT("HexGrid: invalid SampleTex !!"));
			return WorldPos;
		}
		else
		{
			FVector Pos = FVector::ZeroVector;
			FVector4 Sample = FHexMetrics::SampleNoiseTest(WorldPos, SampleTex); // too slow
			//FVector Sample = FHexMetrics::SampleNoise(WorldPos, SampleTex); // will crash
			Pos.X = WorldPos.X + (Sample.X * 2.f - 1.f) * FHexMetrics::CellPerturbStrength;
			//Pos.Y = WorldPos.Y + (Sample.Y * 2.f - 1.f) * FHexMetrics::CellPerturbStrength;
			Pos.Y = WorldPos.Y;
			Pos.Z = WorldPos.Z + (Sample.Z * 2.f - 1.f) * FHexMetrics::CellPerturbStrength;
			return WorldPos;
		}
	}
	return FVector::ZeroVector;
#endif // UsePerturb

#ifndef UsePerturb
	return WorldPos;
#endif
}

void UHexMeshComponent::AddTriangle(const FVector& V1, const FVector& V2, const FVector& V3)
{
	int32 VertexIndex = Vertices.Num();
	// UE: vertex order is counter-dockwise
	// Unity : vertex orde is clockwise
	Vertices.Add(Perturb(V1));
	Vertices.Add(Perturb(V2));
	Vertices.Add(Perturb(V3));

	Triangles.Add(VertexIndex);
	Triangles.Add(VertexIndex + 2);
	Triangles.Add(VertexIndex + 1);
}



void UHexMeshComponent::AddTriangleColor(const FLinearColor& Color)
{
	Colors.Add(Color);
	Colors.Add(Color);
	Colors.Add(Color);
}

void UHexMeshComponent::AddTriangleColor(const FLinearColor& C1, const FLinearColor& C2, const FLinearColor& C3)
{

	Colors.Add(C1);
	Colors.Add(C2);
	Colors.Add(C3);
}

void UHexMeshComponent::AddQuad(const FVector& V1, const FVector& V2, const FVector& V3, const FVector& V4)
{
	int32 VertexIndex = Vertices.Num();
	Vertices.Add(Perturb(V1));
	Vertices.Add(Perturb(V2));
	Vertices.Add(Perturb(V3));
	Vertices.Add(Perturb(V4));
	Triangles.Add(VertexIndex);
	Triangles.Add(VertexIndex + 1);
	Triangles.Add(VertexIndex + 2);
	Triangles.Add(VertexIndex + 1);
	Triangles.Add(VertexIndex + 3);
	Triangles.Add(VertexIndex + 2);
}

void UHexMeshComponent::AddQuadColor(const FLinearColor& C1, const FLinearColor& C2)
{
	Colors.Add(C1);
	Colors.Add(C1);
	Colors.Add(C2);
	Colors.Add(C2);
}

void UHexMeshComponent::AddQuadColor(const FLinearColor& C1, const FLinearColor& C2, const FLinearColor& C3, const FLinearColor& C4)
{
	Colors.Add(C1);
	Colors.Add(C2);
	Colors.Add(C3);
	Colors.Add(C4);
}

// note we do not use noisesample now so this have no change with function <AddTriangle>
void UHexMeshComponent::AddTriangleUnperturbed(const FVector& V1, const FVector& V2, const FVector& V3)
{
	int32 VertexIndex = Vertices.Num();
	// UE: vertex order is counter-dockwise
	// Unity : vertex orde is clockwise
	Vertices.Add(V1);
	Vertices.Add(V2);
	Vertices.Add(V3);

	Triangles.Add(VertexIndex);
	Triangles.Add(VertexIndex + 2);
	Triangles.Add(VertexIndex + 1);
}
