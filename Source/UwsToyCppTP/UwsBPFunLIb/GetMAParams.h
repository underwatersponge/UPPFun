// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GetMAParams.generated.h"

USTRUCT(BlueprintType)
struct FUwsMAParamInfo 
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName ParamsName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Value;
};


UCLASS()
class UWSTOYCPPTP_API UGetMAParams : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "UwsMa")
	static TArray<FUwsMAParamInfo> GetParams(UMaterialInstanceDynamic* MaterialInstanceDynamic, float value);

};
