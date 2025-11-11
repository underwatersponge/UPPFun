// Fill out your copyright notice in the Description page of Project Settings.


#include "UwsToyCppTP/HexMap/Hex/EdgeVertices.h"
#include "UwsToyCppTP/HexMap/Hex/HexMetrics.h"

FEdgeVertices::FEdgeVertices(const FVector& Corner1, const FVector& Corner2)
{
	float delta = 1.0f / 3.0f;
	V1 = Corner1;
	V2 = FMath::Lerp(Corner1, Corner2, delta);
	V3 = FMath::Lerp(Corner1, Corner2, delta * 2.0f);
	V4 = Corner2;
}

FEdgeVertices FEdgeVertices::Terracelerp(const FEdgeVertices& A, const FEdgeVertices& B, const int32 step)
{
	FEdgeVertices Result;
	Result.V1 = FHexMetrics::TerraceLerp(A.V1, B.V1, step);
	Result.V2 = FHexMetrics::TerraceLerp(A.V2, B.V2, step);
	Result.V3 = FHexMetrics::TerraceLerp(A.V3, B.V3, step);
	Result.V4 = FHexMetrics::TerraceLerp(A.V4, B.V4, step);
	return Result;
}

