// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintRoom/Default/Public/SQP_PC_PaintRoom.h"

#include "SkyViewPawn.h"
#include "SQP_PS_PaintRoom.h"
#include "TankCharacter.h"
#include "GameFramework/GameStateBase.h"


void ASQP_PC_PaintRoom::Server_PaintColorChange_Implementation(const FLinearColor Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("PaintColorChange!"));
	GetPlayerState<ASQP_PS_PaintRoom>()->SelectedColor = Value;
}

void ASQP_PC_PaintRoom::Server_ChangeBrushSize_Implementation(float Value)
{
	GetPlayerState<ASQP_PS_PaintRoom>()->SelectedBrushSize = Value;
}

void ASQP_PC_PaintRoom::Server_UpdateLikes_Implementation(int32 LikeNum)
{
	ASQP_PS_PaintRoom* PS = Cast<ASQP_PS_PaintRoom>(PlayerState);
	PS->LikeCounter = LikeNum;
}

void ASQP_PC_PaintRoom::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		SpawnSkyViewPawn();
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

void ASQP_PC_PaintRoom::Server_PossessSkyView_Implementation()
{
	Possess(SkyViewPawn);
}

void ASQP_PC_PaintRoom::Server_PossessPreviousPawn_Implementation()
{
	Possess(PreviousPawn);
}

void ASQP_PC_PaintRoom::Server_CountLike_Implementation(class ASQP_PS_PaintRoom* TargetPS)
{
	TargetPS->IncreaseLikeCounter();
}