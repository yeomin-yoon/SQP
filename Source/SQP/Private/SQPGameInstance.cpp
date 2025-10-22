// Fill out your copyright notice in the Description page of Project Settings.

#include "SQPGameInstance.h"
#include "SQP.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

void USQPGameInstance::Init()
{
	Super::Init();

	//초기 게임 상태
	GAME_STATE = EGameState::Main;

	//네트워크 실패 콜백 등록
	GEngine->OnNetworkFailure().AddUObject(this, &USQPGameInstance::OnNetworkFailure);

	//온라인 서브시스템에서 콜백 등록
	if (const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld()))
	{
		OnlineSessionInterface = Subsystem->GetSessionInterface();
		const TSharedPtr<IOnlineSession> Session = OnlineSessionInterface.Pin();
		Session->OnCreateSessionCompleteDelegates.AddUObject(this, &USQPGameInstance::OnCreateSessionComplete);
		Session->OnFindSessionsCompleteDelegates.AddUObject(this, &USQPGameInstance::OnFindSessionCompleted);
		Session->OnJoinSessionCompleteDelegates.AddUObject(this, &USQPGameInstance::OnJoinSessionCompleted);
	}
}

void USQPGameInstance::CreateMySession(const FString DisplayName, const int32 PlayerCount)
{
	PRINTLOG(TEXT("세션 생성 시도!"));
	
	if (OnlineSessionInterface.IsValid())
	{
		//세션 설정
		FOnlineSessionSettings SessionSettings;
		const FName SubsystemName = Online::GetSubsystem(GetWorld())->GetSubsystemName();
		UE_LOG(LogTemp, Warning, TEXT("호스트 서브시스템 : %s"), *SubsystemName.ToString());
		SessionSettings.bIsLANMatch = SubsystemName.IsEqual(FName("NULL"));
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.NumPublicConnections = PlayerCount;
		SessionSettings.Set(FName("DP_NAME"), DisplayName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		//세션 생성
		const TSharedPtr<IOnlineSession> Session = OnlineSessionInterface.Pin();
		Session->CreateSession(0, FName(DisplayName), SessionSettings);
	}
}

void USQPGameInstance::OnCreateSessionComplete(const FName SessionName, const bool bWasSuccessful)
{
	PRINTLOG(TEXT("세션 [%s] 생성 %s!"), *SessionName.ToString(), bWasSuccessful ? TEXT("성공!") : TEXT("실패!"));

	//리슨 서버로서 게임 맵으로 이동
	GetWorld()->ServerTravel("/Game/Splatoon/Maps/Lobby?Listen");
}

void USQPGameInstance::FindOtherSession()
{
	PRINTLOG(TEXT("세션 조회 시도!"));

	//세션 검색용 클래스 할당
	SessionSearch = MakeShared<FOnlineSessionSearch>();
	const FName SubsystemName = Online::GetSubsystem(GetWorld())->GetSubsystemName();
	SessionSearch->bIsLanQuery = SubsystemName.IsEqual(FName("NULL"));
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	SessionSearch->MaxSearchResults = 100;

	//세션 검색 실행 - 검색 결과는 검색용 클래스에 저장
	const TSharedPtr<IOnlineSession> Session = OnlineSessionInterface.Pin();
	Session->FindSessions(0, SessionSearch.ToSharedRef());
}

void USQPGameInstance::OnFindSessionCompleted(const bool bWasSuccessful)
{
	PRINTLOG(TEXT("세션 검색 %s!"), bWasSuccessful ? TEXT("성공!") : TEXT("실패!"));

	if (bWasSuccessful)
	{
		//검색 결과를 로그로 출력
		auto Results = SessionSearch->SearchResults;
		for (auto Result : Results)
		{
			FString Name;
			Result.Session.SessionSettings.Get(FName(TEXT("DP_NAME")), Name);
			UE_LOG(LogTemp, Warning, TEXT("이름 : %s"), *Name);
		}

		//델리게이트로 검색 결과를 외부에 전파
		OnFindCompleteDelegate.Broadcast(Results);
	}
}

void USQPGameInstance::JoinOtherSession(const int32 SessionIdx)
{
	PRINTLOG(TEXT("세션 합류 시도!"));
	
	//세션 검색 결과에서 지정한 인덱스의 정보를 추출
	auto Results = SessionSearch->SearchResults;
	Results[SessionIdx].Session.SessionSettings.bUseLobbiesIfAvailable = true;
	Results[SessionIdx].Session.SessionSettings.bUsesPresence = true;
	FString SessionName;
	Results[SessionIdx].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), SessionName);

	//지정한 세션에 합류
	const TSharedPtr<IOnlineSession> Session = OnlineSessionInterface.Pin();
	Session->JoinSession(0, FName(SessionName), Results[SessionIdx]);
}

void USQPGameInstance::OnJoinSessionCompleted(const FName SessionName, const EOnJoinSessionCompleteResult::Type Result)
{
	PRINTLOG(TEXT("세션 합류 %s!"), Result == EOnJoinSessionCompleteResult::Success ? TEXT("성공") : TEXT("실패"));
	
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		if (const auto PC = GetFirstLocalPlayerController())
		{
			//세션의 URL 파싱
			FString URL;
			const TSharedPtr<IOnlineSession> Session = OnlineSessionInterface.Pin();
			Session->GetResolvedConnectString(SessionName, URL);
			
			PRINTLOG(TEXT("Now Join to the URL : %s"), *URL);

			//이 클라이언트를 세션에 합류시킨다
			PC->ClientTravel(URL, TRAVEL_Absolute);	
		}
	}
}

void USQPGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	//맵 경로
	FString TargetMapPath = "";

	// 현재 게임 상태에 따라 이동할 맵을 결정
	switch (CurrentGameState)
	{
	case EGameState::Lobby:
		{
			//로비에 있다가 튕겼다면
			PRINTLOG(TEXT("Network Failure in Lobby. Returning to Host Browser."));
			TargetMapPath = TEXT("Main");
			break;
		}
	case EGameState::InGame:
		{
			//게임 플레이 중에 튕겼다면
			PRINTLOG(TEXT("Network Failure in Game. Returning to Main Menu."));
			TargetMapPath = TEXT("Main");
			break;
		}
	default:
		{
			//예기치 못한 네트워크 실패
			PRINTLOG(TEXT("Unexpected Network Failure. Returning to Main Menu."));
			TargetMapPath = TEXT("Main");
			break;
		}
	}

	//결정된 맵으로 클라이언트와 함께 이동
	GetWorld()->ServerTravel(TargetMapPath, true);
}