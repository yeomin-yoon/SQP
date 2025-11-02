// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SQPGameState.h"
#include "SQP_GS_PaintRoom.generated.h"

class ASQP_PS_Master;
struct FPaintExecutionData;

UENUM(BlueprintType)
enum class EPaintRoomState : uint8
{
	None = 0,
	CatchMindStart = 1,
	CatchMindTimeUp = 2,
	DrawingCompetition = 3,
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
	TObjectPtr<class APaintGameActor> PaintGameActor;
	UPROPERTY()
	TObjectPtr<class UIMGManager> IMGManager;
	void StartGame();
	
	UPROPERTY()
	TObjectPtr<UTexture2D> RandomImage;
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetRandomImage(UTexture2D* Image);

	// for (const auto& Pair : PlayerTextureMap)
	// {
	// 	PlayerNames.Add(Pair.Key);
	// 	ComparisonTextures.Add(Pair.Value);
	// }

	//캐치마인드 정답자에 대해 전파하는 메서드
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastSomeoneWin(APlayerState* WinnerPS);

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

	//캐치마인드 제시어를 관리하는 프로퍼티
	__declspec(property(get=GetCatchMindSuggestion, put=SetCatchMindSuggestion)) FString CATCH_MIND_SUGGESTION;
	UFUNCTION()
	FORCEINLINE FString GetCatchMindSuggestion() const { return CatchMindSuggestion; }
	UFUNCTION()
	FORCEINLINE void SetCatchMindSuggestion(const FString& Value)
	{
		CatchMindSuggestion = Value;
	}

	//캐치 마인드 정답을 확인해주는 메서드
	UFUNCTION()
	bool CheckCatchMindAnswer(const FString& OtherAnswer) const;

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

	//캐치 마인드 제시어
	UPROPERTY(VisibleAnywhere)
	FString CatchMindSuggestion;

	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_SetSpawnActorText(class ACompareActor* PaintableActor, const FString& Name, FLinearColor Color);
};