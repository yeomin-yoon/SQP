// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SQP_SG_Main.generated.h"

USTRUCT(BlueprintType)
struct FSQP_PainRoomSaveFormat
{
	GENERATED_BODY()

	//세이브를 로드할 때 위젯을 통해 디스플레이하는 이름
	UPROPERTY()
	FString Name;

	//세이브를 로드할 때 위젯을 통해 디스플레이하는 날짜
	UPROPERTY()
	FString Date;

	//이 세이브 게임과 바인딩되어 있는 페인트 룸 저장 파일의 ID
	UPROPERTY()
	FString ID;

	//이 세이브 게임과 바인딩되어 있는 페인트 룸의 레벨 이름
	UPROPERTY()
	FString Level;

	//생성자
	FSQP_PainRoomSaveFormat() {};
	FSQP_PainRoomSaveFormat(
		const FString& InName,
		const FString& InDate,
		const FString& InID,
		const FString& InLevel) :
		Name(InName),
		Date(InDate),
		ID(InID),
		Level(InLevel) {}
};

UCLASS()
class SQP_API USQP_SG_Main : public USaveGame
{
	GENERATED_BODY()

public:
	//페인트 룸의 이름, 날짜, 구체적인 ID가 명시되어 있는 리스트
	UPROPERTY()
	TArray<FSQP_PainRoomSaveFormat> PaintRoomSaveArray;
};