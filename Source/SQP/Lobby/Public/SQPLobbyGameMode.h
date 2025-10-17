// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SQPLobbyGameMode.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()
	
	// **플레이어를 식별하는 고유 ID**
	// APlayerState::GetUniqueId()->ToString() 값을 저장합니다.
	// 이 ID는 플레이어가 서버에 접속하는 동안 절대로 변하지 않습니다.
	UPROPERTY(BlueprintReadOnly)
	FString PlayerUniqueId;

	// 표시될 플레이어 이름 (닉네임)
	// APlayerState::GetPlayerName() 값을 저장합니다.
	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;
    
	//생성자
	FPlayerInfo() {}
	FPlayerInfo(const FString& InPlayerUniqueId, const FString& InPlayerName) : PlayerUniqueId(InPlayerUniqueId), PlayerName(InPlayerName) {}

	//직렬화
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		bOutSuccess = true; // 일단 성공했다고 가정하고 시작!

		// 변수들을 택배 상자(Ar)에 넣거나 빼는 작업
		Ar << PlayerUniqueId;
		Ar << PlayerName;
        
		return true;
	}

	//비교 연산자 오버로딩
	bool operator==(const FPlayerInfo& Other) const
	{
		return this->PlayerUniqueId == Other.PlayerUniqueId;
	}
};

template<>
struct TStructOpsTypeTraits<FPlayerInfo> : public TStructOpsTypeTraitsBase2<FPlayerInfo>
{
	enum 
	{
		WithNetSerializer = true // "저는 NetSerialize 설명서가 있어요!" 라고 알려주는 팻말
	};
};

UCLASS()
class SQP_API ASQPLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASQPLobbyGameMode();

	virtual void BeginPlay() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION()
	void OnPlayerReadyStateChanged();

	UFUNCTION()
	void CheckAllPlayersReady();


	UFUNCTION()
	void KickPlayerByUniqueId(const FString& PlayerUniqueId);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ServerSideLobbyMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ClientSideLobbyMenuWidgetClass;
};