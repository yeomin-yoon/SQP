// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_GM_PaintRoom.h"

#include "SQPGameInstance.h"
#include "SQPPaintWorldSubsystem.h"
#include "SQP_GS_PaintRoom.h"
#include "SQP_PC_PaintRoom.h"
#include "SQP_PS_PaintRoom.h"

ASQP_GM_PaintRoom::ASQP_GM_PaintRoom()
{
	//플레이어 컨트롤러 클래스 로드
	if (static ConstructorHelpers::FClassFinder<ASQP_PC_PaintRoom>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintRoomLevel/BP_SQP_PC_PaintRoom.BP_SQP_PC_PaintRoom_C"));
		Finder.Succeeded())
	{
		PlayerControllerClass = Finder.Class;
	}

	//플레이어 스테이트 클래스 로드
	if (static ConstructorHelpers::FClassFinder<ASQP_PS_PaintRoom>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintRoomLevel/BP_SQP_PS_PaintRoom.BP_SQP_PS_PaintRoom_C"));
		Finder.Succeeded())
	{
		PlayerStateClass = Finder.Class;
	}

	//게임 스테이트 클래스 로드
	if (static ConstructorHelpers::FClassFinder<ASQP_GS_PaintRoom>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintRoomLevel/BP_SQP_GS_PaintRoom.BP_SQP_GS_PaintRoom_C"));
		Finder.Succeeded())
	{
		GameStateClass = Finder.Class;
	}
}

void ASQP_GM_PaintRoom::BeginPlay()
{
	Super::BeginPlay();

	//선택된 페인트 룸 데이터를 로드에 성공했다면
	if (const auto SaveGame = Cast<USQPGameInstance>(GetWorld()->GetGameInstance())->LoadSelectedPaintRoomData())
	{
		//페인트 볼 실행 데이터를 적용하도록 지시한다
		GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>()->LoadPaintOfWorld(SaveGame);	
	}
}

void ASQP_GM_PaintRoom::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HasAuthority())
	{
		GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>()->SavePaintOfWorld();	
	}
}
