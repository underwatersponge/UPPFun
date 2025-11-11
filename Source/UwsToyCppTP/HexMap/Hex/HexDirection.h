// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HexDirection.generated.h"

UENUM(BlueprintType)
enum class EHexDirection : uint8
{
	NE   UMETA(DisplayName = "NorthEast"),
	E    UMETA(DisplayName = "East"),
	SE   UMETA(DisplayName = "SouthEast"),
	SW   UMETA(DisplayName = "SouthWest"),
	W    UMETA(DisplayName = "West"),
	NW   UMETA(DisplayName = "NorthWest")
};



UCLASS()
class UWSTOYCPPTP_API UHexDirection : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static EHexDirection Opposite(EHexDirection Direction);
	static EHexDirection Previous(EHexDirection Direction);
	static EHexDirection Next(EHexDirection Direction);

};
