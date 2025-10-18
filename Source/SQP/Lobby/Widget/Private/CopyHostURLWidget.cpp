// Fill out your copyright notice in the Description page of Project Settings.

#include "CopyHostURLWidget.h"
#include "SocketSubsystem.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

void UCopyHostURLWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//URL 획득 실패 시
	CopiedHostURL = TEXT("Fail to get Host IPAddress. Please Check Again.");

	//URL을 얻는 단계
	bool bCanBindAll;
	const TSharedPtr<FInternetAddr> LocalAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBindAll);
	CopiedHostURL = LocalAddr.IsValid() ? LocalAddr->ToString(false) : "127.0.0.1";

	//텍스트에 URL 설정
	URLTextBlock->SetText(FText::FromString(*CopiedHostURL));

	//버튼에 이벤트 바인딩
	CopyHostURLButton->OnClicked.AddDynamic(this, &UCopyHostURLWidget::OnCopyHostURLButtonClicked);

	//알림 텍스트 비가시화
	CopyNotificationTextBlock->SetVisibility(ESlateVisibility::Hidden);
}

void UCopyHostURLWidget::OnCopyHostURLButtonClicked()
{
	//문자열을 클립보드에 복사
	FPlatformApplicationMisc::ClipboardCopy(*CopiedHostURL);

	//알림 메시지 가시화
	CopyNotificationTextBlock->SetVisibility(ESlateVisibility::Visible);

	//딜레이 후에 알림 텍스트 비가시화
	GetWorld()->GetTimerManager().SetTimer(HideNotificationTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		CopyNotificationTextBlock->SetVisibility(ESlateVisibility::Hidden);
	}), 1.5f, false);
}