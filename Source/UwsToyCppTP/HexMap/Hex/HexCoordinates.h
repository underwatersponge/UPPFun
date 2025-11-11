// Fill out your copyright notice in the Description page of Project Settings.

// kinds of coordinates transform // 

#pragma once

#include "CoreMinimal.h"
#include "UwsToyCppTP/HexMap/Hex/HexMetrics.h"
//#include "UObject/NoExportTypes.h"
#include "HexCoordinates.generated.h"

USTRUCT(BlueprintType)
struct FHexCoordinates
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 X = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Y = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Z = 0;

	FHexCoordinates():X(0), Y(0), Z(0)
	{
	}

	FHexCoordinates(int32 InX, int32 InZ) : X(InX), Z(InZ)
	{
		Y = -X - Z;
	}
	
	int32 GetX() const { return X; }
	int32 GetY() const { return Y; }
	int32 GetZ() const { return Z; }

	// the var name is  not very good
	static FHexCoordinates FromOffsetCoordinates(int32 OffsetX, int32 OffsetY)
	{
		//return FHexCoordinates(OffsetX, OffsetY);
		return FHexCoordinates(OffsetX - OffsetY / 2, OffsetY);
	}

	FString ToString() const
	{
		return FString::Printf(TEXT("%d, %d, %d"), X, Y, Z);
	}

	FString ToStringOnSeparateLines() const
	{
		return FString::Printf(TEXT("%d\n%d\n%d"), X, Y, Z);
	}

	static FHexCoordinates FromPosition(FVector Position)
	{
		// ============= // 
		float x = Position.Y / (FHexMetrics::InnerRadius * 2.0f);
		float y = -x;
		float offset = Position.X / (FHexMetrics::OuterRadius * 3.0f);
		y -= offset;
		x -= offset;

		int32 IX = FMath::RoundToInt(x);
		int32 IY = FMath::RoundToInt(y);
		int32 IZ = FMath::RoundToInt(-x - y);

		// =============== // 
		if (IX + IY + IZ != 0)
		{
			float ErrorX = FMath::Abs(x - IX);
			float ErrorY = FMath::Abs(y - IY);
			float ErrorZ = FMath::Abs(-x - y- IZ);

			if (ErrorX > ErrorY && ErrorX > ErrorZ)
			{
				IX = -IY - IZ;
			}
			else if (ErrorZ > ErrorY)
			{
				IZ = -IX - IY;
			}
		}

		return FHexCoordinates(IX, IZ);
	}

};