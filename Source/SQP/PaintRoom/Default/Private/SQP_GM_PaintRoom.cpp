// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_GM_PaintRoom.h"

#include "AISimilarityClient.h"
#include "CompareActor.h"
#include "CompetitorName.h"
#include "EngineUtils.h"
#include "PaintGameActor.h"
#include "PaintRoomWidget.h"
#include "SkyViewPawn.h"
#include "SQP.h"
#include "SQP_GI.h"
#include "SQPPaintWorldSubsystem.h"
#include "SQP_GS_PaintRoom.h"
#include "SQP_PaintableActor.h"
#include "SQP_PC_PaintRoom.h"
#include "SQP_PS_Master.h"
#include "SQP_PS_PaintRoomComponent.h"
#include "TankCharacter.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

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
	if (static ConstructorHelpers::FClassFinder<ASQP_PS_Master>
		Finder(TEXT("/Game/Splatoon/Blueprint/Default/BP_SQP_PS_Master.BP_SQP_PS_Master_C"));
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

	//캐치 마인드 데이터 테이블 로드
	if (static ConstructorHelpers::FObjectFinder<UDataTable>
		Finder(TEXT("/Game/Splatoon/Data/CatchMindDataTable.CatchMindDataTable"));
		Finder.Succeeded())
	{
		CatchMindMiniGameDataTable = Finder.Object;
	}

	//캐치 마인드 캔버스 액터 블루프린트 클래스 로드
	if (static ConstructorHelpers::FClassFinder<ASQP_PaintableActor>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintRoomLevel/BP_CatchMindCanvas.BP_CatchMindCanvas_C"));
		Finder.Succeeded())
	{
		CatchMindCanvasActorClass = Finder.Class;
	}

	if (static ConstructorHelpers::FClassFinder<ACompareActor>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintGaming/BP_Compare.BP_Compare_C"));
		Finder.Succeeded())
	{
		CompareActorClass = Finder.Class;
	}
}

void ASQP_GM_PaintRoom::BeginPlay()
{
	Super::BeginPlay();

	//위젯을 생성한다
	const auto Created = CreateWidget(GetWorld(), PaintRoomWidgetClass);
	Created->AddToViewport();

	//액터를 생성한다
	const auto Spawned = GetWorld()->SpawnActor<ASQP_PaintableActor>(CatchMindCanvasActorClass);
	Spawned->SetActorLocation(FVector(2200, 0, 700));
	Spawned->SetActorScale3D(FVector(15, 15, 1));
	Spawned->SetActorRotation(FRotator(90, 0, 0));
	CatchMindCanvasActor = Spawned;

	//선택된 페인트 룸 데이터를 로드에 성공했다면
	if (const auto SaveGame = Cast<USQP_GI>(GetWorld()->GetGameInstance())->LoadSelectedPaintRoomData())
	{
		if (USQP_SG_PaintRoom* SG_PaintRoom = Cast<USQP_SG_PaintRoom>(SaveGame))
		{
			for (auto Pair : SG_PaintRoom->PEDContainer)
			{
				PRINTLOG(TEXT("Successfully Load %s PED... : Length %d"), *Pair.Key.ToString(),
				         Pair.Value.PEDArray.Num());
			}

			//서버는 직접 페인트 볼 실행 데이터를 적용한다
			GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>()->LoadPaintOfWorld(SG_PaintRoom->PEDContainer);

			//클라이언트에 전송하기 위핸 PED 배열을 추출해서 할당한다
			GetGameState<ASQP_GS_PaintRoom>()->PaintExecutionDataSnapshot = SG_PaintRoom->ConstructFullPEDArray();
		}
	}



	SimilarityClient = GetGameInstance()->GetSubsystem<UAISimilarityClient>();
}

void ASQP_GM_PaintRoom::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


	const auto PCPaint = Cast<ASQP_PC_PaintRoom>(NewPlayer);
	const auto GSPaint = GetGameState<ASQP_GS_PaintRoom>();
	
	if (HasAuthority() && NewPlayer->IsLocalController())
	{
		if (const auto GI = GetGameInstance<USQP_GI>(); GI->bHostAsSpectator)
		{
			if (APawn* OldPawn = NewPlayer->GetPawn())
			{
				OldPawn->Destroy();
			}
		
			ASkyViewPawn* SpectatorPawn = GetWorld()->SpawnActor<ASkyViewPawn>(ASkyViewPawn::StaticClass(), FVector(0.f, 0.f, 200.f), FRotator(0, 0, 0));
			if (SpectatorPawn)
			{
				NewPlayer->Possess(SpectatorPawn);
			}
		}
	}
}

void ASQP_GM_PaintRoom::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void ASQP_GM_PaintRoom::StartCatchMindMiniGame()
{
	//이미 캐치 마인드 미니 게임이 진행 중이었다면
	if (CatchMindMiniGameTimerHandle.IsValid())
	{
		return;
	}

	PRINTLOGNET(TEXT("GM_PaintRoom::StartCatchMindMiniGame"));

	if (const auto GSPaint = GetGameState<ASQP_GS_PaintRoom>())
	{
		//편리한 작업을 위해 임시 변수 구축
		TArray<ASQP_PS_Master*> TempPSMasterArray;
		for (const auto PS : GSPaint->PlayerArray)
		{
			TempPSMasterArray.Emplace(Cast<ASQP_PS_Master>(PS));
		}
		TArray<ASQP_PC_PaintRoom*> TempPCPaintArray;
		for (const auto PS : GSPaint->PlayerArray)
		{
			TempPCPaintArray.Emplace(Cast<ASQP_PC_PaintRoom>(PS->GetPlayerController()));
		}
		const int32 Size = TempPSMasterArray.Num();

		//이번에 그림을 제시어를 묘사할 캐치마인드 플레이어를 선택
		const int PainterIdx = FMath::RandRange(1, Size - 1);

		//모든 유저의 페인트 룸 역할을 갱신
		for (int i = 0; i < Size; i++)
		{
			TempPSMasterArray[i]->PaintRoom->PAINT_ROOM_ROLE = (i == PainterIdx
				                                                    ? EPaintRoomRole::CatchMindPainter
				                                                    : EPaintRoomRole::CatchMindParticipant);
		}

		//랜덤 제시어를 하나 선택
		const int32 Rand = FMath::RandRange(1, SuggestionArray.Num());
		const FCatchMind* Selected = CatchMindMiniGameDataTable->FindRow<FCatchMind>(
			FName(FString::Printf(TEXT("제시어%d"), Rand)), TEXT(""));
		const FString Hint = Selected->Hint;
		const FString Suggestion = Selected->Suggestion;

		//캐치 마인드를 위한 캔버스를 초기화
		CatchMindCanvasActor->Multicast_ClearPaint();

		//모든 유저에게 제시어 업데이트 명령
		for (int i = 0; i < Size; i++)
		{
			FString Target;
			if (i == PainterIdx)
			{
				Target = Suggestion;
			}
			else
			{
				Target = TEXT("");
				for (int j = 0; j < Suggestion.Len(); j++)
				{
					Target += TEXT("?");
				}
			}
			TempPCPaintArray[i]->Client_ReceiveCatchMindSuggestion(Target, Hint);
		}

		//제시어 업데이트
		GSPaint->CATCH_MIND_SUGGESTION = Suggestion;

		//모든 클라이언트가 알 수 있도록 게임 스테이트의 변수를 변경
		GSPaint->PAINT_ROOM_STATE = EPaintRoomState::CatchMindStart;

		//30초 동안 선택받은 플레이어는 페인트 볼을 쏠 수 있고, 나머지는 정답을 서버에 전송 가능
		StartTimer(GSPaint, 30);
	}
}

void ASQP_GM_PaintRoom::TimeUpCatchMindMiniGame()
{
	GetWorldTimerManager().ClearTimer(CatchMindMiniGameTimerHandle);
	
	PRINTLOGNET(TEXT("GM_PaintRoom::EndCatchMindMiniGame"));
	
	if (const auto GSPaint = GetGameState<ASQP_GS_PaintRoom>())
	{
		//GS의 PS를 PS_Master로 변환해서 임시 저장
		TArray<ASQP_PS_Master*> TempPSMasterArray;
		for (const auto PS : GSPaint->PlayerArray)
		{
			TempPSMasterArray.Emplace(Cast<ASQP_PS_Master>(PS));
		}
		const int32 Size = TempPSMasterArray.Num();

		//모든 유저가 페인트 볼을 발사할 수 있도록 초기화
		for (int i = 0; i < Size; i++)
		{
			TempPSMasterArray[i]->PaintRoom->PAINT_ROOM_ROLE = EPaintRoomRole::None;
		}

		//제시어 초기화
		GSPaint->CATCH_MIND_SUGGESTION = FString();

		//모든 클라이언트가 알 수 있도록 게임 스테이트의 변수를 변경
		GSPaint->PAINT_ROOM_STATE = EPaintRoomState::CatchMindTimeUp;

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, GSPaint]()
		{
			//모든 클라이언트가 알 수 있도록 게임 스테이트의 변수를 변경
			GSPaint->PAINT_ROOM_STATE = EPaintRoomState::None;

			//캐치 마인드를 위한 캔버스를 초기화
			CatchMindCanvasActor->ClearPaint();
		}), 5, false);
	}
}

void ASQP_GM_PaintRoom::EndCatchMindMiniGame()
{
	GetWorldTimerManager().ClearTimer(CatchMindMiniGameTimerHandle);
	
	PRINTLOGNET(TEXT("GM_PaintRoom::EndCatchMindMiniGame"));

	if (const auto GSPaint = GetGameState<ASQP_GS_PaintRoom>())
	{
		//GS의 PS를 PS_Master로 변환해서 임시 저장
		TArray<ASQP_PS_Master*> TempPSMasterArray;
		for (const auto PS : GSPaint->PlayerArray)
		{
			TempPSMasterArray.Emplace(Cast<ASQP_PS_Master>(PS));
		}
		const int32 Size = TempPSMasterArray.Num();

		//모든 유저가 페인트 볼을 발사할 수 있도록 초기화
		for (int i = 0; i < Size; i++)
		{
			TempPSMasterArray[i]->PaintRoom->PAINT_ROOM_ROLE = EPaintRoomRole::None;
		}

		//제시어 초기화
		GSPaint->CATCH_MIND_SUGGESTION = FString();

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this, GSPaint]()
		{
			//모든 클라이언트가 알 수 있도록 게임 스테이트의 변수를 변경
			GSPaint->PAINT_ROOM_STATE = EPaintRoomState::None;

			//캐치 마인드를 위한 캔버스를 초기화
			CatchMindCanvasActor->ClearPaint();
		}), 5, false);
	}
}

void ASQP_GM_PaintRoom::StartTimer(ASQP_GS_PaintRoom* GS, const float Time) const
{
	GS->CountdownStartTime = GetWorld()->GetTimeSeconds();
	GS->CountdownTotalTime = Time;
	GS->bOnCountdown = true;
}

void ASQP_GM_PaintRoom::StartCompetitionMiniGame()
{
	if (bIsCompetition)
		return;
	bIsCompetition = true;

	ASQP_GS_PaintRoom* GSPaint = GetGameState<ASQP_GS_PaintRoom>();
	if (!GSPaint)
		return;

	for (APlayerState* PS : GSPaint->PlayerArray)
	{
		APlayerController* PC = Cast<APlayerController>(PS->GetOwner());

		if (!PC->HasAuthority())
			continue;
		
		PlayerNames.Add(PS->GetPlayerName());
	}

	int32 TotalPlayers = PlayerNames.Num();
	CompareTextures.SetNum(TotalPlayers);

	// 스폰 Canvas
	SpawnActorsInCircle(CompareActorClass, TotalPlayers, 1470.f, FVector(0.f, 0.f, 300.f));

	// 문제 그림 표시
	GSPaint->StartGame();

	// 게임 시작
	StartTimer(GSPaint, 10.f);
}

void ASQP_GM_PaintRoom::EndCompetitionMiniGame()
{
	ASQP_GS_PaintRoom* GSPaint = GetGameState<ASQP_GS_PaintRoom>();
	
	for (int32 i = 0; i < PaintableCompareActors.Num(); i++)
	{
		PlayerNames[i] = PaintableCompareActors[i]->CompetitionPlayerName;
		UTexture* CompareImage;
		PaintableCompareActors[i]->DynMat->GetTextureParameterValue(TEXT("ColorRenderTarget"), CompareImage);
		CompareTextures[i] = Cast<UTexture2D>(CompareImage);
	}

	SimilarityClient->CompareTextures(GSPaint->RandomImage, CompareTextures, PlayerNames);

	InitCompetition();
}


void ASQP_GM_PaintRoom::SpawnActorsInCircle(TSubclassOf<ACompareActor> ActorClass, int32 NumActors, float Radius,
                                            FVector Center)
{
	if (!ActorClass || NumActors <= 0)
		return;

	UWorld* World = GetWorld();
	if (!World)
		return;
	
	float GapDegrees = 60.f;
	float GapRadians = FMath::DegreesToRadians(GapDegrees);
	float StartAngle = PI / 2 + (GapRadians / 2);
	float FillRadians = 2 * PI - GapRadians;
	
	for (int32 i = 0; i < NumActors; ++i)
	{
		float Angle = StartAngle + (FillRadians / (NumActors - 1)) * i;
		FVector Pos = Center + FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0) * Radius;
		FRotator Rot = (Center - Pos).Rotation();

		ACompareActor* PaintableActor = World->SpawnActor<ACompareActor>(ActorClass, Pos, Rot);
		PaintableActor->SetReplicates(true);
		PaintableActor->bAlwaysRelevant = true;
		GetGameState<ASQP_GS_PaintRoom>()->MultiCast_SetSpawnActorText(PaintableActor, PlayerNames[i]);

		PaintableCompareActors.Add(PaintableActor);
	}
}



void ASQP_GM_PaintRoom::InitCompetition()
{
	PlayerNames.Empty();
	CompareTextures.Empty();
	for (ACompareActor* Actor : PaintableCompareActors)
	{
		Actor->Destroy();
	}
	PaintableCompareActors.Empty();
	bIsCompetition = false;
}
