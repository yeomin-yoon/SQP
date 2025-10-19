// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "SocketSubsystem.h"
#include "SQP.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	URLInputCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
	URLEditableTextBox->SetVisibility(ESlateVisibility::Hidden);
	
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinButtonClicked);
	URLEditableTextBox->OnTextCommitted.AddDynamic(this, &UMainMenu::OnURLTextCommitted);
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostButtonClicked);
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::OnBackButtonClicked);
}

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenu::OnHostButtonClicked()
{
	//URL을 얻는 단계
	bool bCanBindAll;
	const TSharedPtr<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
	URL = LocalAddr.IsValid() ? LocalAddr->ToString(false) : "127.0.0.1";
	
	PRINTLOG(TEXT("%s"), *URL);
	
	//로비 레벨을 리슨 서버 옵션으로 시작
	UGameplayStatics::OpenLevel(this, FName("Lobby"), true, "listen");
}

void UMainMenu::OnJoinButtonClicked()
{
	URLInputCanvasPanel->SetVisibility(ESlateVisibility::Visible);
	URLEditableTextBox->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenu::OnURLTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (InCommitMethod != ETextCommit::OnEnter)
	{
		return;
	}
	
	//inText 에 해당하는 url 검증 과정 필요
	UGameplayStatics::OpenLevel(this, *InText.ToString(), true);
}

void UMainMenu::OnBackButtonClicked()
{
	URLInputCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
	URLEditableTextBox->SetVisibility(ESlateVisibility::Hidden);
}