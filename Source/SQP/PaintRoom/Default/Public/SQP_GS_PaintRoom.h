// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQPGameState.h"
#include "SQP_GS_PaintRoom.generated.h"

struct FPaintExecutionData;

UENUM(BlueprintType)
enum class EPaintRoomState : uint8
{
	None = 0,
	CatchMind = 1,
	DrawingCompetition = 2,
};

UCLASS()
class SQP_API ASQP_GS_PaintRoom : public ASQPGameState
{
	GENERATED_BODY()

public:
	ASQP_GS_PaintRoom();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//서버로부터 PED 배열을 최초 1회만 전달받는다
	UPROPERTY(ReplicatedUsing = OnRep_PaintExecutionDataSnapshot)
	TArray<FPaintExecutionData> PaintExecutionDataSnapshot;

	//PED 배열의 최초 1회 리플리케이션 됐을 때의 콜백 
	UFUNCTION()
	void OnRep_PaintExecutionDataSnapshot();

	//최초 1회만 PED 배열을 받도록 보장하는 불 변수
	bool bHasInitialPaintDataBeenApplied = false;

	UPROPERTY()
	TObjectPtr<UTexture2D> RandomImage;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetRandomImage(UTexture2D* Image);

	UPROPERTY()
	TMap<FString, UTexture2D*> PlayerTextureMap;
	UPROPERTY()
	TArray<UTexture2D*> ComparisonTextures;
	TArray<FString> PlayerNames;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AddPlayerTexture(const FString& PlayerName, UTexture2D* Texture);

	// for (const auto& Pair : PlayerTextureMap)
	// {
	// 	PlayerNames.Add(Pair.Key);
	// 	ComparisonTextures.Add(Pair.Value);
	// }
	//
	// UAISimilarityClient* AIClient = GetGameInstance()->GetSubsystem<UAISimilarityClient>();
	// AIClient->CompareTextures(Original, ComparisonTextures, PlayerNames);

	//페인트 룸 스테이트를 관리하는 프로퍼티
	__declspec(property(get=GetPaintRoomState, put=SetPaintRoomState)) EPaintRoomState PAINT_ROOM_STATE;
	UFUNCTION()
	FORCEINLINE EPaintRoomState GetPaintRoomState() const { return PaintRoomState; };
	UFUNCTION()
	FORCEINLINE void SetPaintRoomState(const EPaintRoomState Value)
	{
		PaintRoomState = Value;
		OnRep_PaintRoomState();
	};

protected:
	//페인트 룸의 상태
	UPROPERTY(ReplicatedUsing=OnRep_PaintRoomState, VisibleAnywhere)
	EPaintRoomState PaintRoomState;

	//서버의 페인트 룸의 상태가 변화했을 때 호출되는 리플리케이션 콜백
	UFUNCTION()
	void OnRep_PaintRoomState();

public:
	UPROPERTY(Replicated)
	float CountdownStartTime;
	UPROPERTY(Replicated)
	float CountdownTotalTime;
	UPROPERTY(Replicated)
	bool bOnCountdown = false;
};
