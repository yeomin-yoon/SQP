#include "UIInteractionComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "LikeUI.h"
#include "Components/WidgetComponent.h"


UUIInteractionComponent::UUIInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction> RightClickAsset(
		TEXT("'/Game/Splatoon/Input/IA_RightClick.IA_RightClick'")
	);
	if (RightClickAsset.Succeeded())
	{
		RightClickAction = RightClickAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCAsset(
		TEXT("'/Game/Input/IMC_Default.IMC_Default'")
	);
	if (IMCAsset.Succeeded())
	{
		IMC = IMCAsset.Object;
	}
}


void UUIInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Only hide local player's LikeUI
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn->IsLocallyControlled())
	{
		// Enhanced Input Subsystem 등록
		PC = Cast<APlayerController>(OwnerPawn->GetController());
		if (PC)
		{
			if (auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PC->GetLocalPlayer()))
			{
				InputSubsystem->AddMappingContext(IMC, 0);
			}

			// 입력 바인딩
			if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				if (RightClickAction)
					EIC->BindAction(RightClickAction, ETriggerEvent::Started, this, &UUIInteractionComponent::OnRightClickPressed);
					EIC->BindAction(RightClickAction, ETriggerEvent::Completed, this, &UUIInteractionComponent::OnRightClickReleased);
			}
		}
	}
}


void UUIInteractionComponent::OnRightClickPressed()
{
	PressPointerKey(EKeys::LeftMouseButton);
	
	if (auto* WidgetComp = GetHoveredWidgetComponent())
	{
		if (auto* Widget = Cast<ULikeUI>(WidgetComp->GetUserWidgetObject()))
		{
			Widget->ClickingActor = GetOwner();
		}
	}
}

void UUIInteractionComponent::OnRightClickReleased()
{
	ReleasePointerKey(EKeys::LeftMouseButton);
}

void UUIInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
