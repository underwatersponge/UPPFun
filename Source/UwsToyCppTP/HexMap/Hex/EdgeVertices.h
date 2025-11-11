// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdgeVertices.generated.h"
/**
 * 
 */

USTRUCT(BlueprintType)
struct FEdgeVertices
{
	GENERATED_BODY()

public:
	FEdgeVertices() = default;
	FEdgeVertices(const FVector& Corner1, const FVector& Corner2);
	~FEdgeVertices() = default;
public:
	static FEdgeVertices Terracelerp(const FEdgeVertices& A, const FEdgeVertices& B, const int32 Step);

public:
	UPROPERTY()
	FVector V1;
	UPROPERTY()
	FVector V2;
	UPROPERTY()
	FVector V3;
	UPROPERTY()
	FVector V4;

};
