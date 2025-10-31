// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintRoom/Default/Public/SQP_PC_PaintRoom.h"

#include "CatchMindWidget.h"
#include "SkyViewPawn.h"
#include "SQP.h"
#include "SQP_GM_PaintRoom.h"
#include "SQP_GS_PaintRoom.h"
#include "SQP_PS_Master.h"
#include "SQP_PS_PaintRoomComponent.h"
#include "TankCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

ASQP_PC_PaintRoom::ASQP_PC_PaintRoom()
{
	//캐치 마인드 위젯 블루프린트 클래스 획득
	if (static ConstructorHelpers::FClassFinder<UUserWidget>
		Finder(TEXT("/Game/Splatoon/Blueprint/PaintRoomLevel/WBP_CatchMind.WBP_CatchMind_C"));
		Finder.Succeeded())
	{
		CatchMindWidgetClass = Finder.Class;
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

void ASQP_PC_PaintRoom::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SpawnSkyViewPawn();
	}

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
	}
}

void ASQP_PC_PaintRoom::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ATankCharacter* player = Cast<ATankCharacter>(InPawn))
	{
		DynMat = player->GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	}

	
	// int32 Index = GetWorld()->GetGameState()->PlayerArray.IndexOfByKey(this);
	// UTexture2D* Tex = LoadTextureByIndex(Index);
	// if (DynMat && Tex)
	// {
	// 	DynMat->SetTextureParameterValue(FName("BaseColorTexture"), Tex);
	// }

	PreviousPawn = CurrentPawn;
	CurrentPawn = InPawn;

	FVector CamLoc;
	FRotator CamRot;
	GetPlayerViewPoint(CamLoc, CamRot);
	CamLoc += InitialLocationOffset;
	CamRot.Pitch = InitialPitchOffset;
	if (SkyViewPawn)
	{
		SkyViewPawn->SetActorLocationAndRotation(CamLoc, CamRot);
	}
}

UTexture2D* ASQP_PC_PaintRoom::LoadTextureByIndex(int32 Index)
{
	FString Path = FString::Printf(
		TEXT("'/Game/Assets/Tanks/red_tank/Textures/TankTexture/tank_texture_%d.tank_texture_%d'"), Index, Index);
	return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Path));
}

void ASQP_PC_PaintRoom::SpawnSkyViewPawn()
{
	FVector CamLoc;
	FRotator CamRot;
	GetPlayerViewPoint(CamLoc, CamRot);
	CamLoc += InitialLocationOffset;
	CamRot.Pitch = InitialPitchOffset;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	SkyViewPawn = GetWorld()->SpawnActor<ASkyViewPawn>(
		ASkyViewPawn::StaticClass(),
		CamLoc,
		CamRot,
		SpawnParams
	);
}


void ASQP_PC_PaintRoom::OnSkyView()
{
	Server_PossessSkyView();
}

void ASQP_PC_PaintRoom::Client_NotifyAnswerIsWrong_Implementation()
{
	//서버에서 오답이라는 피드백이 왔으므로 위젯으로 표시
	CatchMindWidget->ShowWrong();
}

void ASQP_PC_PaintRoom::Server_ReceiveCatchMindAnswer_Implementation(const FString& Answer)
{
	if (const auto GS = Cast<ASQP_GS_PaintRoom>(GetWorld()->GetGameState()))
	{
		//정답 여부 확인
		if (GS->CheckCatchMindAnswer(Answer))
		{
			PRINTLOGNET(TEXT("Correct!"));
			
			//즉각적으로 캐치 마인드 미니 게임을 종료
			if (const auto GM = Cast<ASQP_GM_PaintRoom>(GetWorld()->GetAuthGameMode()))
			{
				GM->EndCatchMindMiniGame();
			}

			//클라이언트들에 정답 사실을 전파
			GS->Multicast_BroadcastSomeoneWin(GetPlayerState<ASQP_PS_Master>());
		}
		else
		{
			PRINTLOGNET(TEXT("Who the fuck are you?"));
			
			//클라이언트에 오답 사실을 전닳
			Client_NotifyAnswerIsWrong();
		}
	}
}

void ASQP_PC_PaintRoom::Client_ReceiveCatchMindSuggestion_Implementation(const FString& Suggestion, const FString& Hint)
{
	CatchMindWidget->SetSuggestionText(Suggestion, Hint);
}

void ASQP_PC_PaintRoom::Server_CountLike_Implementation(ASQP_PS_Master* TargetPS)
{
	TargetPS->PaintRoom->IncreaseLikeCounter();
}

void ASQP_PC_PaintRoom::Server_PossessSkyView_Implementation()
{
	Possess(SkyViewPawn);
}

void ASQP_PC_PaintRoom::Server_PossessPreviousPawn_Implementation()
{
	Possess(PreviousPawn);
}