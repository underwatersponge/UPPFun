// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "HexMeshComponent.generated.h"

class UHexCellComponent;
class UTextRenderComponent;
struct FEdgeVertices;

UCLASS()
class UWSTOYCPPTP_API UHexMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:	
	void TriangulateAll(const TArray<UHexCellComponent*>& Cells);
	TArray<FVector> GetVers() { return Vertices; }
private:

	void Triangulate(UHexCellComponent* Cell); // onw cell
	void Triangulate(int32 TriIndex, UHexCellComponent* Cell); // pre cell triangle 
	
	void TriangulateEdgeFun(const FVector& Center, const FEdgeVertices& Edge, const FLinearColor& Col);
	void TriangulateEdgeStrip(const FEdgeVertices& E1, const FLinearColor& Col1, const FEdgeVertices& E2, const FLinearColor& Col2);

	void TriangulateConnection(int32 Index, UHexCellComponent* Cell, const FEdgeVertices& E1);
	void TriangulateEdgeTerraces(const FEdgeVertices& Begin, const UHexCellComponent* BeginCell,
								const FEdgeVertices& End, const UHexCellComponent* EndCell);
	void TriangulateCorner(const FVector& Bottom, const UHexCellComponent* BottomCell, 
							const FVector& Left, const UHexCellComponent* LeftCell,
							const FVector& Right, const UHexCellComponent* RightCell);
	void TriangulateCornerTerraces(const FVector& Begin, const UHexCellComponent* BeginCell,
							const FVector& Left, const UHexCellComponent* LeftCell,
							const FVector& Right, const UHexCellComponent* RightCell);
	void TriangulateCornerTerracesCliff(const FVector& Begin, const UHexCellComponent* BeginCell,
							const FVector& Left, const UHexCellComponent* LeftCell,
							const FVector& Right, const UHexCellComponent* RightCell);
	void TriangulateCornerCliffTerraces(const FVector& Begin, const UHexCellComponent* BeginCell,
		const FVector& Left, const UHexCellComponent* LeftCell,
		const FVector& Right, const UHexCellComponent* RightCell);
	void TriangulateBoundaryTriangle(const FVector& Begin, const UHexCellComponent* BeginCell,
		const FVector& Left, const UHexCellComponent* LeftCell,
		const FVector& Boundary, const FLinearColor& BoundaryCol);

	FVector Perturb(const FVector& WorldPos)const;

	void AddTriangle(const FVector& V1, const FVector& V2, const FVector& V3);	
	void AddTriangleColor(const FLinearColor& Color); //  hole triangle
	void AddTriangleColor(const FLinearColor& C1, const FLinearColor& C2, const FLinearColor& C3); // pre vertice 
	
	void AddQuad(const FVector& V1, const FVector& V2, const FVector& V3, const FVector& V4);
	void AddQuadColor(const FLinearColor& C1, const FLinearColor& C2);
	void AddQuadColor(const FLinearColor& C1, const FLinearColor& C2, const FLinearColor& C3, const FLinearColor& C4);

	void AddTriangleUnperturbed(const FVector& V1, const FVector& V2, const FVector& V3);

private:

	UPROPERTY()
	TArray<UTextRenderComponent*> CellLabels;
	UPROPERTY()
	TArray<FVector> Vertices;
	UPROPERTY()
	TArray<int32> Triangles;
	UPROPERTY()
	TArray<FLinearColor> Colors;

};
