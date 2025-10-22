// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/SlateEnums.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UWidgetSwitcher;
class UEditableTextBox;
class UVerticalBox;
class UButton;
class UCanvasPanel;
class FOnlineSessionSearchResult;

UCLASS()
class SQP_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnHostMenuButtonClicked();
	
	UFUNCTION()
	void OnJoinMenuButtonClicked();

	UFUNCTION()
	void OnSearchMenuButtonClicked();
	
	UFUNCTION()
	void OnURLTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

	UFUNCTION()
	void OnBackButtonClicked();

	UPROPERTY(VisibleAnywhere)
	bool bIsFindingSession;

	UFUNCTION()
	void OnFindButtonClicked();

	void OnFindCompleted(const TArray<FOnlineSessionSearchResult>& Results);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SessionInfoWidgetClass;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> ButtonContainer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> HostMenuButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> JoinMenuButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> SearchMenuButton;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> ExitMenuButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UEditableTextBox> URLEditableTextBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCanvasPanel> URLInputCanvasPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> BackButtonOne;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> BackButtonTwo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> FindButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UVerticalBox> SessionInfoVerticalBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString URL;
};