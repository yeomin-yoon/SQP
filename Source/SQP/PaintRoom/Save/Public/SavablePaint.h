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

	//페인트 데이터 맵을 위한 액터의 고유 ID를 할당 - 만약 이미 설정되어 있다면 건너뛴다
	virtual void ValidPersistantActorID() = 0;
};