// Fill out your copyright notice in the Description page of Project Settings.


#include "SQP_SG_PaintRoom.h"

TArray<FPaintExecutionData> USQP_SG_PaintRoom::ConstructFullPEDArray() const
{
	TArray<FPaintExecutionData> PEDArray;
	for (TPair Pair : PEDContainer)
	{
		for (FPaintExecutionData PaintData : Pair.Value.PEDArray)
		{
			PEDArray.Emplace(PaintData);
		}
	}
	return PEDArray;
}