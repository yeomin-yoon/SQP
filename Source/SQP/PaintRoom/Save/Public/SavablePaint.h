// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SavablePaint.generated.h"

struct FPaintExecutionData;

// This class does not need to be modified.
UINTERFACE()
class USavablePaint : public UInterface
{
	GENERATED_BODY()
};

class SQP_API ISavablePaint
{
	GENERATED_BODY()

public:
	//페인트 데이터 맵을 위한 액터의 고유 ID를 반환
	virtual FGuid GetPersistantActorID() = 0;
};