// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexGridChunk.generated.h"


class UHexMeshComponent;
class UHexCellComponent;

UCLASS()
class UWSTOYCPPTP_API AHexGridChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexGridChunk();

	virtual void PostInitializeComponents() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AddCell(int32 Index, UHexCellComponent* Cell);
	void Refresh()const;

public:
	UPROPERTY()
	TArray<UHexCellComponent*> Cells;
private:
	UPROPERTY()
	UHexMeshComponent* HexMesh;
};
