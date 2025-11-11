// Fill out your copyright notice in the Description page of Project Settings.


#include "UwsToyCppTP/HexMap/Hex/HexMetrics.h"

const float FHexMetrics::OuterRadius = 100.f;
const float FHexMetrics::InnerRadius = OuterRadius * 0.866025404f;
const float FHexMetrics::SolidFactor = 0.8f;
const float FHexMetrics::BlendFactor = 1.0f - SolidFactor;
const float FHexMetrics::ElevationStep = 30.0f;

const int FHexMetrics::TerracesPerSlope = 2;
const int FHexMetrics::TerracesStpes = TerracesPerSlope *  2+ 1;
const float FHexMetrics::HorizonTerraceStepSize = 1.0 / TerracesStpes;
const float FHexMetrics::VerticalTerraceStepSize = 1.0 / (TerracesPerSlope + 1);

const float FHexMetrics::CellPerturbStrength = 50.0f;
const float FHexMetrics::NoiseScale = 0.0003f; // 10000 * 0.0001 = 1 <0 - 100m -> 0-1> 
const float FHexMetrics::ElevationPerturbStrength = 15.f;

const int FHexMetrics::ChunkSizeX = 5;
const int FHexMetrics::ChunkSizeY = 5;
 
TArray<FVector> FHexMetrics::Corners =
{
	FVector(OuterRadius, 0.f, 0.f),
	FVector(0.5f * OuterRadius, InnerRadius, 0.f),
	FVector(-0.5f * OuterRadius, InnerRadius, 0.f),
	FVector(-OuterRadius, 0.f, 0.f),
	FVector(-0.5f * OuterRadius, -InnerRadius, 0.f),
	FVector(0.5f * OuterRadius, -InnerRadius, 0.f),
	
};

FHexMetrics::FHexMetrics()
{
}
