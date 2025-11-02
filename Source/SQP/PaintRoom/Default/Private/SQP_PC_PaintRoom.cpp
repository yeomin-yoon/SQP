// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintRoom/Default/Public/SQP_PC_PaintRoom.h"
#include "CatchMindWidget.h"
#include "CompetitionWidget.h"
#include "PlaygroundScoreWidget.h"
#include "SQP.h"
#include "SQP_GI.h"
#include "SQP_GM_PaintRoom.h"
#include "SQP_GS_PaintRoom.h"
#include "SQP_PS_Master.h"
#include "SQP_PS_PaintRoomComponent.h"
#include "TankCharacter.h"
#include "TimerUI.h"
#include "UIManager.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "PlaygroundMenuWidget.h"
#include "Components/AudioComponent.h"

ASQP_PC_PaintRoom::ASQP_PC_PaintRoom()
{
	//캐치 마인드 위젯 블루프린트 클래스 획득
	if (static ConstructorHelpers::FClassFinder<UUserWidget>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintRoomLevel/WBP_CatchMind.WBP_CatchMind_C"));
		Finder.Succeeded())
	{
		CatchMindWidgetClass = Finder.Class;
	}

	//플레이 그라운드 위젯 블루프린트 클래스 획득
	if (static ConstructorHelpers::FClassFinder<UUserWidget>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintRoomLevel/WBP_PlaygroundScore.WBP_PlaygroundScore_C"));
		Finder.Succeeded())
	{
		PlaygroundScoreWidgetClass = Finder.Class;
	}

	bReplicates = true;

	//플레이 그라운드 메뉴 위젯 블루프린트 클래스 획득
	if (static ConstructorHelpers::FClassFinder<UUserWidget>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintRoomLevel/WBP_PlaygroundMenu.WBP_PlaygroundMenu_C"));
		Finder.Succeeded())
	{
		PlaygroundMenuWidgetClass = Finder.Class;
	}

	//컴페티션 위젯 블루프린트 클래스 획득
	if (static ConstructorHelpers::FClassFinder<UUserWidget>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintRoomLevel/WBP_Competition.WBP_Competition_C"));
		Finder.Succeeded())
	{
		CompetitionWidgetClass = Finder.Class;
	}

	AudioComp1 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp1"));
	AudioComp2 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp2"));
	AudioComp1->bAutoActivate = false;
	AudioComp2->bAutoActivate = false;
	
	if (static ConstructorHelpers::FObjectFinder<USoundWave> USoundWave1(
			TEXT("'/Game/Assets/Sounds/OST/Spray_Paint_Dreams-1.Spray_Paint_Dreams-1'"));
		USoundWave1.Succeeded())
	{
		AudioComp1->SetSound(USoundWave1.Object);
	}
	
	if (static ConstructorHelpers::FObjectFinder<USoundWave> USoundWave2(
			TEXT("'/Game/Assets/Sounds/OST/Spray_Paint_Dreams-1.Spray_Paint_Dreams-2'"));
		USoundWave2.Succeeded())
	{
		AudioComp2->SetSound(USoundWave2.Object);
	}
}

void ASQP_PC_PaintRoom::BeginPlay()
{
	Super::BeginPlay();

	GI = Cast<USQP_GI>(GetGameInstance());
	GM = Cast<ASQP_GM_PaintRoom>(UGameplayStatics::GetGameMode(GetWorld()));
	GS = Cast<ASQP_GS_PaintRoom>(UGameplayStatics::GetGameState(this));
	PS = Cast<ASQP_PS_Master>(PlayerState);
	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();

	if (IsLocalController())
	{
		//캐치 마인드 위젯 블루프린트 생성
		if (const auto Created = CreateWidget(this, CatchMindWidgetClass))
		{
			if (const auto Casted = Cast<UCatchMindWidget>(Created))
			{
				CatchMindWidget = Casted;
				CatchMindWidget->HideAll();
				CatchMindWidget->AddToViewport();
			}
		}

		//컴페티션 위젯 블루프린트 생성
		if (const auto Created = CreateWidget(this, CompetitionWidgetClass))
		{
			if (const auto Casted = Cast<UCompetitionWidget>(Created))
			{
				CompetitionWidget = Casted;
				CompetitionWidget->HideAll();
				CompetitionWidget->AddToViewport();
			}
		}

		//플레이그라운드 스코어 위젯 블루프린트 생성
		if (const auto Created = CreateWidget(this, PlaygroundScoreWidgetClass))
		{
			if (const auto Casted = Cast<UPlaygroundScoreWidget>(Created))
			{
				PlaygroundScoreWidget = Casted;
				PlaygroundScoreWidget->AddToViewport();
			}
		}

		//플레이 그라운드 메뉴 위젯 블루프린트 생성
		if (const auto Created = CreateWidget(this, PlaygroundMenuWidgetClass))
		{
			if (const auto Casted = Cast<UPlaygroundMenuWidget>(Created))
			{
				PlaygroundMenuWidget = Casted;
				PlaygroundMenuWidget->AddToViewport();
			}
		}

		//타이머 UI 생성
		TimerUI = UIManager->CreateTimerUI();
		TimerUI->TimerRichTextBlock->SetVisibility(ESlateVisibility::Hidden);
		TimerUI->ReferImage->SetVisibility(ESlateVisibility::Hidden);

		AudioComp1->Play();
		if (AudioComp1)
		{
			AudioComp1->OnAudioFinished.AddDynamic(this, &ASQP_PC_PaintRoom::OnOST1Finished);
		}
		if (AudioComp2)
		{
			AudioComp2->OnAudioFinished.AddDynamic(this, &ASQP_PC_PaintRoom::OnOST2Finished);
		}
		
	}
}

void ASQP_PC_PaintRoom::OnOST1Finished()
{
	if (IsLocalController())
	{
		AudioComp2->Play();
	}
}

void ASQP_PC_PaintRoom::OnOST2Finished()
{
	if (IsLocalController())
	{
		AudioComp1->Play();
	}
}

void ASQP_PC_PaintRoom::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GS)
	{
		if (IsLocalController())
		{
			ReplicatedCountDown();
		}
	}
}


void ASQP_PC_PaintRoom::Server_PaintColorChange_Implementation(const FLinearColor Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("PaintColorChange!"));
	GetPlayerState<ASQP_PS_Master>()->PaintRoom->SelectedColor = Value;
}

void ASQP_PC_PaintRoom::Server_ChangeBrushSize_Implementation(const float Value)
{
	GetPlayerState<ASQP_PS_Master>()->PaintRoom->SelectedBrushSize = Value;
}

void ASQP_PC_PaintRoom::Server_UpdateLikes_Implementation(const int32 LikeNum)
{
	GetPlayerState<ASQP_PS_Master>()->PaintRoom->LikeCounter = LikeNum;
}


void ASQP_PC_PaintRoom::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ATankCharacter* player = Cast<ATankCharacter>(InPawn))
	{
		DynMat = player->GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	}
}


void ASQP_PC_PaintRoom::Client_NotifyAnswerIsWrong_Implementation()
{
	//서버에서 오답이라는 피드백이 왔으므로 위젯으로 표시
	CatchMindWidget->ShowWrong();
}

UTexture2D* ASQP_PC_PaintRoom::LoadTextureByIndex(int32 Index)
{
	FString Path = FString::Printf(
		TEXT("'/Game/Assets/Tanks/red_tank/Textures/TankTexture/tank_texture_%d.tank_texture_%d'"), Index, Index);
	return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));
}

void ASQP_PC_PaintRoom::Client_ReceiveCatchMindSuggestion_Implementation(const FString& Suggestion, const FString& Hint)
{
	//서버에서 오답이라는 피드백이 왔으므로 위젯으로 표시
	CatchMindWidget->SetSuggestionText(Suggestion, Hint);
}

void ASQP_PC_PaintRoom::Server_ReceiveCatchMindAnswer_Implementation(const FString& Answer)
{
	if (const auto GSPaint = Cast<ASQP_GS_PaintRoom>(GetWorld()->GetGameState()))
	{
		//정답 여부 확인
		if (GSPaint->CheckCatchMindAnswer(Answer))
		{
			PRINTLOGNET(TEXT("Correct!"));

			//즉각적으로 캐치 마인드 미니 게임을 종료
			if (GM)
			{
				GM->EndCatchMindMiniGame();
			}

			//클라이언트들에 정답 사실을 전파
			const auto WinnerPS = GetPlayerState<ASQP_PS_Master>();
			WinnerPS->SCORE += 10;
			GS->Multicast_BroadcastSomeoneWin(WinnerPS);
		}
		else
		{
			PRINTLOGNET(TEXT("Who the fuck are you?"));

			//클라이언트에 오답 사실을 전닳
			Client_NotifyAnswerIsWrong();
		}
	}
}


void ASQP_PC_PaintRoom::Server_CountLike_Implementation(ASQP_PS_Master* TargetPS)
{
	TargetPS->PaintRoom->IncreaseLikeCounter();
}

void ASQP_PC_PaintRoom::ReplicatedCountDown()
{
	if (GS->bOnCountdown)
	{
		Elapsed = GS->GetServerWorldTimeSeconds() - GS->CountdownStartTime;
		Remaining = GS->CountdownTotalTime - Elapsed;

		RemainingTime = FMath::CeilToInt(Remaining);
		if (RemainingTime != LastRemainingTime)
		{
			if (IsLocalController())
			{
				TimerUI->TimerRichTextBlock->SetVisibility(ESlateVisibility::Visible);
				if (GS->PAINT_ROOM_STATE == EPaintRoomState::DrawingCompetition)
				{
					TimerUI->ReferImage->SetVisibility(ESlateVisibility::Visible);
					TimerUI->ReferImage->SetBrushFromTexture(GS->RandomImage);
				}
			}

			UpdateCountdownUI(RemainingTime, TimerUI);
			LastRemainingTime = RemainingTime;
		}
		if (Remaining < 0.f)
		{
			Remaining = 0.f;
			GS->bOnCountdown = false;
			LastRemainingTime = -1;
			if (IsLocalController())
			{
				TimerUI->TimerRichTextBlock->SetVisibility(ESlateVisibility::Hidden);
				TimerUI->ReferImage->SetVisibility(ESlateVisibility::Hidden);
			}
			if (HasAuthority())
			{
				if (GM->bIsCompetition)
				{
					GM->EndCompetitionMiniGame();
				}
				else
				{
					GM->TimeUpCatchMindMiniGame();
				}
			}
		}
	}
}

void ASQP_PC_PaintRoom::UpdateCountdownUI(int RemainingSeconds, UTimerUI* UI)
{
	FString RichText = FString::Printf(TEXT("<Timer>%d</>"), RemainingSeconds);

	if (UI)
	{
		UI->TimerRichTextBlock->SetText(FText::FromString(RichText));
	}
}
