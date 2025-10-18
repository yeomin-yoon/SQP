// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SQP_GM_Lobby.generated.h"

class ASQP_PC_Lobby;

UENUM(Blueprintable)
enum class ELobbyState : uint8
{
	None = 0,
	Host = 1,
	ReadyClient = 2,
	UnreadyClient = 3,
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()
	
	//플레이어의 고유 식별 아이디 : 플레이어 스테이트 유니크 아이디
	UPROPERTY(BlueprintReadOnly)
	FString PlayerUniqueId;

	//로비 메뉴 위젯에 디스플레이되는 플레이어 이름 : 플레이어 스테이트 네임
	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;

	//플레이어의 준비 상태
	UPROPERTY(BlueprintReadOnly)
	ELobbyState LobbyState;
    
	//생성자
	FPlayerInfo() : PlayerUniqueId(""), PlayerName(""), LobbyState(ELobbyState::None) {}
	FPlayerInfo(const FString& InPlayerUniqueId, const FString& InPlayerName, const ELobbyState& InLobbyState) : PlayerUniqueId(InPlayerUniqueId), PlayerName(InPlayerName), LobbyState(InLobbyState) {}

	//직렬화
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		//일단 성공했다고 가정하고 시작
		bOutSuccess = true;

		//아카이빙
		Ar << PlayerUniqueId;
		Ar << PlayerName;
		Ar << LobbyState;
        
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
class SQP_API ASQP_GM_Lobby : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASQP_GM_Lobby();

	virtual void BeginPlay() override;
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION()
	ASQP_PC_Lobby* GetHostPlayerController() const;

	UFUNCTION()
	bool CheckAllPlayersReady() const;

	UFUNCTION()
	void MoveToGameMap();

	UFUNCTION()
	void KickPlayerByUniqueId(const FString& PlayerUniqueId);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ServerSideLobbyMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ClientSideLobbyMenuWidgetClass;
};