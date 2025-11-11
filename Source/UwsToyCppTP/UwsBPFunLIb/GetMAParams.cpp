// Fill out your copyright notice in the Description page of Project Settings.


#include "UwsToyCppTP/UwsBPFunLIb/GetMAParams.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GetMAParams.h"

TArray<FUwsMAParamInfo> UGetMAParams::GetParams(UMaterialInstanceDynamic* MaterialInstanceDynamic, float value = 3.7)
{
	int32 Index = 0;
	float NewValue = 3.7f;// not used, maybe set it to which usedful value for you 
	TArray<FUwsMAParamInfo> ParamsArray;
	TArray<FMaterialParameterInfo> OutParameterInfo;
	TArray<FGuid> OutParameterIds;
	
	MaterialInstanceDynamic->GetAllScalarParameterInfo(OutParameterInfo, OutParameterIds);

	for (const FMaterialParameterInfo& Param : OutParameterInfo)
	{
		FUwsMAParamInfo NewParm;
		NewParm.ParamsName = Param.Name;
		NewParm.Value = value; // set to value whick good for you
		ParamsArray.Add(NewParm);
	}
	return ParamsArray;
}
