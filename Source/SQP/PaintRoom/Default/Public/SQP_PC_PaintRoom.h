// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQPPlayerController.h"
#include "SQP_PC_PaintRoom.generated.h"

class ASQP_PS_Master;

UCLASS()
class SQP_API ASQP_PC_PaintRoom : public ASQPPlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynMat;
	UTexture2D* LoadTextureByIndex(int32 Index);
	
public:	
	virtual void OnPossess(APawn* InPawn) override;
	UPROPERTY()
	TObjectPtr<APawn> CurrentPawn;
	UPROPERTY()
	TObjectPtr<APawn> PreviousPawn;

	UFUNCTION(Server, Reliable)
	void Server_PaintColorChange(FLinearColor Value);

	UFUNCTION(Server, Reliable)
	void Server_ChangeBrushSize(float Value);

	UFUNCTION(Server, Reliable)
	void Server_UpdateLikes(int32 LikeNum);

	void SpawnSkyViewPawn();
	UPROPERTY(EditDefaultsOnly, Category="Sky View")
	FVector InitialLocationOffset = FVector(0.f, 0.f, 100);
	UPROPERTY(EditDefaultsOnly, Category="Sky View")
	float InitialPitchOffset = -15.f;
	UPROPERTY()
	TObjectPtr<class ASkyViewPawn> SkyViewPawn;
	UFUNCTION(Server, Reliable)
	void Server_PossessSkyView();
	UFUNCTION(Server, Reliable)
	void Server_PossessPreviousPawn();
	void OnSkyView();

	UFUNCTION(Server, Reliable)
	void Server_CountLike(ASQP_PS_Master* TargetPS);
};
