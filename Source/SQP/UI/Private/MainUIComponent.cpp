#include "MainUIComponent.h"

#include "UIManager.h"


UMainUIComponent::UMainUIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


void UMainUIComponent::BeginPlay()
{
	Super::BeginPlay();

	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();

	UIManager->CreateMainUI();
}


void UMainUIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

