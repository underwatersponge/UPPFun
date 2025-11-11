// Fill out your copyright notice in the Description page of Project Settings.


#include "UwsToyCppTP/HexMap/Hex/HexDirection.h"

EHexDirection UHexDirection::Opposite(EHexDirection Direction)
{
	int32 Cur = static_cast<int>(Direction);
	return static_cast<EHexDirection>(Cur < 3 ? Cur + 3 : Cur - 3);
}

EHexDirection UHexDirection::Previous(EHexDirection Direction)
{
	return (Direction == EHexDirection::NE ? EHexDirection::NW :
			static_cast<EHexDirection>(static_cast<int>(Direction) - 1));
}

EHexDirection UHexDirection::Next(EHexDirection Direction)
{
	return (Direction == EHexDirection::NW ? EHexDirection::NE :
		static_cast<EHexDirection>(static_cast<int>(Direction) + 1));
}
