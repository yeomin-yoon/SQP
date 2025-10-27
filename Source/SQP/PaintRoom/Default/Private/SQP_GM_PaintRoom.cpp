// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_GM_PaintRoom.h"

#include "PaintRoomWidget.h"
#include "SQP.h"
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

	//페인트 룸 위젯 클래스 로드
	if (static ConstructorHelpers::FClassFinder<UPaintRoomWidget>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintRoomLevel/WBP_PaintRoom.WBP_PaintRoom_C"));
		Finder.Succeeded())
	{
		PaintRoomWidgetClass = Finder.Class;
	}
	
}

void ASQP_GM_PaintRoom::BeginPlay()
{
	Super::BeginPlay();

	//위젯을 생성한다
	const auto Created = CreateWidget(GetWorld(), PaintRoomWidgetClass);
	Created->AddToViewport();

	//선택된 페인트 룸 데이터를 로드에 성공했다면
	if (const auto SaveGame = Cast<USQPGameInstance>(GetWorld()->GetGameInstance())->LoadSelectedPaintRoomData())
	{
		if (USQP_SG_PaintRoom* SG_PaintRoom = Cast<USQP_SG_PaintRoom>(SaveGame))
		{
			for (auto Pair : SG_PaintRoom->PEDContainer)
			{
				PRINTLOG(TEXT("Successfully Load %s PED... : Length %d"), *Pair.Key.ToString(), Pair.Value.PEDArray.Num());
			}
			
			//서버는 직접 페인트 볼 실행 데이터를 적용한다
			GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>()->LoadPaintOfWorld(SG_PaintRoom->PEDContainer);

			//클라이언트에 전송하기 위핸 PED 배열을 추출해서 할당한다
			GetGameState<ASQP_GS_PaintRoom>()->PaintExecutionDataSnapshot = SG_PaintRoom->ConstructFullPEDArray();
		}
	}
}