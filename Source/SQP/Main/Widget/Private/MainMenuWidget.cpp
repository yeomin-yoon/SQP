// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuWidget.h"

#include "OnlineSessionSettings.h"
#include "SessionInfoWidget.h"
#include "SocketSubsystem.h"
#include "SQP.h"
#include "SQP_GI.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//호스트 버튼 바인딩
	HostMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnHostMenuButtonClicked);
	
	//합류 버튼 바인딩
	JoinMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnJoinMenuButtonClicked);

	//검색 버튼 바인딩
	SearchMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSearchMenuButtonClicked);

	//URL 입력 텍스트 바인딩
	URLEditableTextBox->OnTextCommitted.AddDynamic(this, &UMainMenuWidget::OnURLTextCommitted);

	//세션 검색 버튼 바인딩
	FindButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnFindButtonClicked);

	//뒤로가기 버튼 바인딩
	BackButtonOne->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackButtonClicked);
	BackButtonTwo->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackButtonClicked);

	//나가기 버튼 바인딩
	ExitMenuButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnExitButtonClicked);

	//시션 검색 완료 콜백 바인딩
	if (const auto GI = Cast<USQP_GI>(GetGameInstance()))
	{
		GI->OnFindCompleteDelegate.AddUObject(this, &UMainMenuWidget::OnFindCompleted);
	}

	//메인 세이브를 이용해 재방문 여부 구분
	if (const auto GI = Cast<USQP_GI>(GetGameInstance()))
	{
		if (GI->LoadMainSaveGame())
		{
			WelcomeTextBlock->SetText(FText::FromString(TEXT("다시 돌아오신걸 환영합니다!")));
		}
		else
		{
			WelcomeTextBlock->SetText(FText::FromString(TEXT("처음 뵙겠습니다!")));
		}
	}

	//텍스트 바인딩
	EnterNicknameTextBox->OnTextCommitted.AddDynamic(this, &UMainMenuWidget::OnEnterNicknameTextCommitted);

	if (const auto GI = Cast<USQP_GI>(GetGameInstance()))
	{
		if (GI->CheckNameEmpty())
		{
			//환영 판넬로 스위칭
			WidgetSwitcher->SetActiveWidgetIndex(3);		
		}
		else
		{
			//메뉴 판넬로 스위칭
			WidgetSwitcher->SetActiveWidgetIndex(0);
		}
	}
}

void UMainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenuWidget::OnHostMenuButtonClicked()
{
	//URL을 얻는 단계
	bool bCanBindAll;
	const TSharedPtr<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
	URL = LocalAddr.IsValid() ? LocalAddr->ToString(false) : "127.0.0.1";

	if (const auto GI = Cast<USQP_GI>(GetGameInstance()))
	{
		GI->CreateMySession(TEXT("정우의 방"), 4);
	}
	else
	{
		PRINTLOG(TEXT("Game Instance is Not Valid!"));
	}
}

void UMainMenuWidget::OnJoinMenuButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}

void UMainMenuWidget::OnSearchMenuButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);

	//바로 검색을 시작하도록 명령
	OnFindButtonClicked();
}

void UMainMenuWidget::OnURLTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (InCommitMethod != ETextCommit::OnEnter)
	{
		return;
	}
	
	//inText 에 해당하는 url 검증 과정 필요
	UGameplayStatics::OpenLevel(this, *InText.ToString(), true);
}

void UMainMenuWidget::OnBackButtonClicked()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
}

void UMainMenuWidget::OnEnterNicknameTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (InCommitMethod == ETextCommit::Type::OnEnter)
	{
		if (const FString NewNickname = InText.ToString(); NewNickname.Len() > 0)
		{
			if (const auto GI = Cast<USQP_GI>(GetGameInstance()))
			{
				//게임 인스턴스에 입력한 정보를 저장
				GI->AssignNewUser(NewNickname);
			}

			//비로소 메인 메뉴로 전환
			WidgetSwitcher->SetActiveWidgetIndex(0);
		}	
	}
}

void UMainMenuWidget::OnFindButtonClicked()
{
	//이미 검색 중이라면
	if (bIsFindingSession)
	{
		return;
	}
	
	if (const auto GI = Cast<USQP_GI>(GetGameInstance()))
	{
		//차단기 올리기
		bIsFindingSession = true;

		//세션 검색
		GI->FindOtherSession();

		//버튼 잠시 비활성화
		FindButton->SetIsEnabled(!bIsFindingSession);
	}
}

void UMainMenuWidget::OnFindCompleted(const TArray<FOnlineSessionSearchResult>& Results)
{
	//기존 결과 삭제
	SessionInfoVerticalBox->ClearChildren();

	//세션 정보 위젯을 생성해서 박스에 추가
	for (int32 i = 0; i < Results.Num(); i++)
	{
		const auto Created = CreateWidget(this, SessionInfoWidgetClass);
		if (const auto SessionInfoWidget = Cast<USessionInfoWidget>(Created))
		{
			FString SessionName;
			Results[i].Session.SessionSettings.Get(FName(TEXT("DP_NAME")), SessionName);
			SessionInfoWidget->SetBindingSessionIdx(i);
			SessionInfoWidget->RoomNameTextBlock->SetText(FText::FromString(SessionName));
			SessionInfoVerticalBox->AddChild(SessionInfoWidget);
		}
	}

	//검색 종료
	bIsFindingSession = false;

	//버튼 다시 활성화
	FindButton->SetIsEnabled(!bIsFindingSession);
}

void UMainMenuWidget::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}