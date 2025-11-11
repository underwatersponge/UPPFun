// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HexGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UWSTOYCPPTP_API AHexGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AHexGameMode();
protected:
	virtual void BeginPlay() override;
	
};
