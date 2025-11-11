// Fill out your copyright notice in the Description page of Project Settings.

// Hex based informations //

#pragma once

#include "CoreMinimal.h"
#include "HexMetrics.generated.h"

UENUM(BlueprintType)
enum class EHexEdgeType : uint8
{
	Flat, Slope, Cliff
};


USTRUCT(BlueprintType)
struct FHexMetrics
{
	GENERATED_BODY()

	FHexMetrics();
public:
	static const float OuterRadius;
	static const float InnerRadius;
	static const float SolidFactor;
	static const float BlendFactor;
	static const float ElevationStep;

	static const int TerracesPerSlope;
	static const int TerracesStpes;
	static const float HorizonTerraceStepSize;
	static const float VerticalTerraceStepSize;
	
	static const float CellPerturbStrength;
	static const float NoiseScale;
	static const float ElevationPerturbStrength;

	static const int ChunkSizeX; // alonge the axisY in UE
	static const int ChunkSizeY; // alonge the axisX in UE
private:
	static TArray<FVector> Corners;	

public:
	static FVector TerraceLerp(const FVector& A, const FVector& B, const int32 Step)
	{
		FVector Vel = A;
		float h = Step * FHexMetrics::HorizonTerraceStepSize;
		Vel.X += (B.X - A.X) * h;
		Vel.Y += (B.Y - A.Y) * h;
		float v = ((Step + 1) / 2) * FHexMetrics::VerticalTerraceStepSize;
		Vel.Z += (B.Z - A.Z) * v;
		return Vel;
	}

	static FLinearColor TerraceLerp(const FLinearColor& A, const FLinearColor& B, const int32 Step)
	{
		float h = Step * FHexMetrics::HorizonTerraceStepSize;
		return FMath::Lerp(A, B, h); //FLinearColor::LerpUsingHSV : <better result but explation>
	}

	static FVector4 SampleNoise(const FVector& WorldPos, const UTexture2D* SampleTex)
	{
	
		if (!SampleTex)
		{
			UE_LOG(LogTemp, Warning, TEXT("SampleTex: SampleTex is nullptr!"));
			return FVector4::Zero();
		}

		const FTexturePlatformData* PlatformData = SampleTex->GetPlatformData();
		if (!PlatformData)
		{
			UE_LOG(LogTemp, Warning, TEXT("SampleTex: Invalid PlatformData"));
			return FVector4::Zero();
		}

		if (PlatformData->Mips.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("SampleTex: No Mips"));
			return FVector4::Zero();
		}

		if (PlatformData->PixelFormat != PF_B8G8R8A8)
		{
			UE_LOG(LogTemp, Warning, TEXT("Unsupported pixel format!"));
			return FVector4::Zero();
		}

		const FTexture2DMipMap& Mip = PlatformData->Mips[0];
		const void* DataPtr = Mip.BulkData.LockReadOnly();
		if (!DataPtr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to lock bulk data!"));
			return FVector4::Zero();
		}

		const int32 Width = Mip.SizeX;
		const int32 Height = Mip.SizeY;
		float U = WorldPos.X * NoiseScale;
		float V = WorldPos.Y * NoiseScale;
		U -= FMath::FloorToFloat(U);
		V -= FMath::FloorToFloat(V);

		const float FX = U * (Width - 1);
		const float FY = V * (Height - 1);
		const int32 X0 = FMath::Clamp(FMath::FloorToInt(FX), 0, Width - 1);
		const int32 Y0 = FMath::Clamp(FMath::FloorToInt(FY), 0, Height - 1);
		const int32 X1 = FMath::Clamp(X0 + 1, 0, Width - 1);
		const int32 Y1 = FMath::Clamp(Y0 + 1, 0, Height - 1);
		const float FracX = FX - X0;
		const float FracY = FY - Y0;

		FVector4 Result = FVector4::Zero();
		const float Rev255 = 1.0f / 255.f;

		const FColor* Colors = static_cast<const FColor*>(DataPtr);
		const FColor& Col_LB = Colors[Y0 * Width + X0];
		const FColor& Col_RB = Colors[Y0 * Width + X1];
		const FColor& Col_LT = Colors[Y1 * Width + X0];
		const FColor& Col_RT = Colors[Y1 * Width + X1];
			
		Result.X = Rev255 * FMath::Lerp(
			FMath::Lerp(Col_LB.R, Col_RB.R, FracX),
			FMath::Lerp(Col_LT.R, Col_RT.R, FracX),
			FracY);
		Result.Y = Rev255 * FMath::Lerp(
			FMath::Lerp(Col_LB.G, Col_RB.G, FracX),
			FMath::Lerp(Col_LT.G, Col_RT.G, FracX),
			FracY);
		Result.Z = Rev255 * FMath::Lerp(
			FMath::Lerp(Col_LB.B, Col_RB.B, FracX),
			FMath::Lerp(Col_LT.B, Col_RT.B, FracX),
			FracY);
		Result.W = Rev255 * FMath::Lerp(
			FMath::Lerp(Col_LB.A, Col_RB.A, FracX),
			FMath::Lerp(Col_LT.A, Col_RT.A, FracX),
			FracY);

		Mip.BulkData.Unlock();
		return Result;
	}

	// note to do: it is too slow not good change it latter
	static FVector4 SampleNoiseTest(const FVector& WorldPos, UTexture2D* SampleTex)
	{
		TArray64<uint8> RowData;
		if (SampleTex->Source.IsValid() && SampleTex->Source.GetFormat() == TSF_BGRA8)
		{
			SampleTex->Source.GetMipData(RowData, 0);
			//reinterpret_cast<FColor*>(RowData.GetData());

			const int32 Width = SampleTex->GetSizeX();
			const int32 Height = SampleTex->GetSizeY();
			float U = WorldPos.X * NoiseScale;
			float V = WorldPos.Y * NoiseScale;
			U = FMath::Fmod(U, 1.0f); // mod for float<examp: Fmod(5.37f, 2.5f) = 0.37f>
			V = FMath::Fmod(V, 1.0f);
			if (U < 0.f) U += 1.0f;
			if (V < 0.f) V += 1.0f;

			const float FX = U * (Width - 1);
			const float FY = V * (Height - 1);
			const int32 X0 = FMath::Clamp(FMath::FloorToInt(FX), 0, Width - 1);
			const int32 Y0 = FMath::Clamp(FMath::FloorToInt(FY), 0, Height - 1);
			const int32 X1 = FMath::Clamp(X0 + 1, 0, Width - 1);
			const int32 Y1 = FMath::Clamp(Y0 + 1, 0, Height - 1);
			const float FracX = FX - X0;
			const float FracY = FY - Y0;

			/*
			TArray<FColor> Colors;
			const int32 NumCols = RowData.Num() / 4;
			Colors.Reserve(NumCols);
			for (int32 i = 0; i < NumCols; i++)
			{
				int32 Index = i * 4;
				FColor Color = {
					RowData[Index + 0],
					RowData[Index + 1],
					RowData[Index + 2],
					RowData[Index + 3],
				};

				Colors.Add(Color);
			}
			*/

			FVector4 Result = FVector4::Zero();
			const float Rev255 = 1.0f / 255.f;

			//const FColor& Col_LB = Colors[Y0 * Width + X0];
			//const FColor& Col_RB = Colors[Y0 * Width + X1];
			//const FColor& Col_LT = Colors[Y1 * Width + X0];
			//const FColor& Col_RT = Colors[Y1 * Width + X1];

			const FColor Col_LB = FColor{ RowData[4 * (Y0 * Width + X0)], RowData[4 * (Y0 * Width + X0) + 1],
										  RowData[4 * (Y0 * Width + X0) + 2], RowData[4 * (Y0 * Width + X0) + 3] };
			
			const FColor Col_RB = FColor{ RowData[4 * (Y0 * Width + X1)], RowData[4 * (Y0 * Width + X1) + 1],
										  RowData[4 * (Y0 * Width + X1) + 2], RowData[4 * (Y0 * Width + X1) + 3] };
			
			const FColor Col_LT = FColor{ RowData[4 * (Y1 * Width + X0)], RowData[4 * (Y1 * Width + X0) + 1],
										  RowData[4 * (Y1 * Width + X0) + 2], RowData[4 * (Y1 * Width + X1) + 3] };

			const FColor Col_RT = FColor{ RowData[4 * (Y1 * Width + X1)], RowData[4 * (Y1 * Width + X1) + 1],
										  RowData[4 * (Y1 * Width + X1) + 2], RowData[4 * (Y1 * Width + X1) + 3] };


			Result.X = Rev255 * FMath::Lerp(
				FMath::Lerp(Col_LB.R, Col_RB.R, FracX),
				FMath::Lerp(Col_LT.R, Col_RT.R, FracX),
				FracY);
			Result.Y = Rev255 * FMath::Lerp(
				FMath::Lerp(Col_LB.G, Col_RB.G, FracX),
				FMath::Lerp(Col_LT.G, Col_RT.G, FracX),
				FracY);
			Result.Z = Rev255 * FMath::Lerp(
				FMath::Lerp(Col_LB.B, Col_RB.B, FracX),
				FMath::Lerp(Col_LT.B, Col_RT.B, FracX),
				FracY);
		/*	Result.W = Rev255 * FMath::Lerp(
				FMath::Lerp(Col_LB.A, Col_RB.A, FracX),
				FMath::Lerp(Col_LT.A, Col_RT.A, FracX),
				FracY);*/
			Result.W = 1.0f;
			return Result;
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("failed"));
			return FVector4::Zero();
		}
	}

	static EHexEdgeType GetEdgeType(int32 Elevation1, int32 Elevation2)
	{
		if (Elevation1 == Elevation2)
		{
			return EHexEdgeType::Flat;
		}
		int Delta = Elevation2 - Elevation1;
		if (Delta == 1 || Delta == -1)
		{
			return EHexEdgeType::Slope;
		}
		return EHexEdgeType::Cliff;
	}

	static FVector GetFirstCorner(int32 Index)
	{
		return Corners[Index];
	}

	static FVector GetSecondCorner(int32 Index)
	{
		return Corners[(Index + 1) % 6];
	}

	static FVector GetFirstSolidCorner(int32 Index)
	{
		return Corners[Index] * SolidFactor;
	}

	static FVector GetSecondSolidCorner(int32 Index)
	{
		return Corners[(Index + 1) % 6] * SolidFactor;
	}

	static FVector GetBridge(int32 Index)
	{
		return ((Corners[Index] + Corners[(Index + 1) % 6]) * BlendFactor);
	}

};


//FNoiseSampler;