// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LobbyMenuWidgetBase.h"
#include "HostSideLobbyMenuWidget.generated.h"

class USelectedPRSInfoWidget;
class UPaintRoomSaveInfoWidget;
class UScrollBox;
class UActiveButton;
class UTextBlock;

UCLASS()
class SQP_API UHostSideLobbyMenuWidget : public ULobbyMenuWidgetBase
{
	GENERATED_BODY()

public:
	UHostSideLobbyMenuWidget();

	virtual void NativeConstruct() override;

	//새로운 플레이어가 들어오면 호출되는 메서드
	virtual void OnOtherPlayerEnter(FPlayerInfo& NewPlayerInfo) override;

	//모든 참가자가 준비를 마치면 활성화되는 액티브 버튼
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UActiveButton> StartButton;

	//페인트 룸 세이브 인포 위젯이 나열되는 스크롤 박스
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> SaveDataScrollBox;

	//현재 선택되어 있는 페인트 룸 세이브를 유효하지 않은 값으로 초기화하는 버튼
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CancelSaveDataSelectionButton;

	//현재 선택되어 있는 페인트 룸 세이브의 정보를 디스플레이하는 위젯
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USelectedPRSInfoWidget> SelectedPRSInfoWidget;

	//사용자가 선택할 수 있는 페인트 룸 세이브에 대한 정보를 디스플레이하는 위젯 블루프린트 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PaintRoomSaveInfoWidgetClass;

	//현재 선택되어 있는 페인트 룸 세이브 인포 위젯
	UPROPERTY(VisibleAnywhere)
	UPaintRoomSaveInfoWidget* CurrentSelectedPaintRoomSaveInfoWidget;

	//취소 버튼이 클릭 되었을 때 콜백받는 메서드
	UFUNCTION()
	void OnCancelSaveDataSelectionButtonClicked();

	//페인트 룸 세이브 인포 위젯이 더블클릭 되었을 때 콜백받는 메서드
	UFUNCTION()
	void OnSaveDataSlotDoubleClicked(UPaintRoomSaveInfoWidget* Target);

	//시작 버튼이 클릭되었을 때 콜백받는 메서드
	UFUNCTION()
	void OnStartButtonClicked();
};