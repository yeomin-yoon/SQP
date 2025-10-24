// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SQP_SG_PaintRoomID.generated.h"

USTRUCT(BlueprintType)
struct FSQP_PaintRoomSaveFormat
{
	GENERATED_BODY()

	//세이브를 로드할 때 위젯을 통해 디스플레이하는 이름
	UPROPERTY()
	FString SaveGameName;

	//세이브를 로드할 때 위젯을 통해 디스플레이하는 날짜
	UPROPERTY()
	FString SaveGameDate;

	//이 세이브 게임과 바인딩되어 있는 페인트 룸 저장 파일의 ID
	UPROPERTY()
	FString PaintRoomSaveGameID;

	//생성자
	FSQP_PaintRoomSaveFormat() {};
	FSQP_PaintRoomSaveFormat(
		const FString& InSaveGameName,
		const FString& InSaveGameDate,
		const FString& InPaintRoomSaveGameID) :
		SaveGameName(InSaveGameName),
		SaveGameDate(InSaveGameDate),
		PaintRoomSaveGameID(InPaintRoomSaveGameID) {};
};

UCLASS()
class SQP_API USQP_SG_PaintRoomID : public USaveGame
{
	GENERATED_BODY()

public:
	//페인트 룸의 이름, 날짜, 구체적인 ID가 명시되어 있는 리스트
	UPROPERTY()
	TArray<FSQP_PaintRoomSaveFormat> PaintRoomSaveGameList;
};