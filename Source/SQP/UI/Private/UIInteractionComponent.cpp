#include "UIInteractionComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "LikeUI.h"
#include "MainUIComponent.h"
#include "SQP_PS_PaintRoom.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


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

	InteractionDistance = 1500.f;
}


void UUIInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn->IsLocallyControlled())
	{
		PC = Cast<APlayerController>(OwnerPawn->GetController());
		if (PC)
		{
			if (auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PC->GetLocalPlayer()))
			{
				InputSubsystem->AddMappingContext(IMC, 0);
			}
			if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				if (RightClickAction)
					EIC->BindAction(RightClickAction, ETriggerEvent::Started, this,
					                &UUIInteractionComponent::OnRightClickPressed);
				EIC->BindAction(RightClickAction, ETriggerEvent::Completed, this,
				                &UUIInteractionComponent::OnRightClickReleased);
			}
		}

		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), Actors);

		for (AActor* Actor : Actors)
		{
			if (!Actor) continue;
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Actor->GetName());
			if (UMainUIComponent* MainUIComp = Actor->FindComponentByClass<UMainUIComponent>())
			{
				MainUIComp->OnLikeChanged.AddUObject(this, &UUIInteractionComponent::OnClick);
			}
		}
	}
}


void UUIInteractionComponent::OnRightClickPressed()
{
	PressPointerKey(EKeys::LeftMouseButton);

	UpdateLikes();
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



void UUIInteractionComponent::OnClick()
{
		//Server_UpdateLikes(TargetPawn);
}

void UUIInteractionComponent::UpdateLikes()
{
	if (UWidgetComponent* HoveredUI = GetHoveredWidgetComponent())
	{
		if (APawn* TargetPawn = Cast<APawn>(HoveredUI->GetOwner()))
		{
			if (ASQP_PS_PaintRoom* TargetPS = Cast<ASQP_PS_PaintRoom>(TargetPawn->GetPlayerState()))
			{
				Server_CountLike(TargetPS);
			}
		}
	}
}

void UUIInteractionComponent::Server_CountLike_Implementation(ASQP_PS_PaintRoom* TargetPS)
{
	TargetPS->IncreaseLikeCounter();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *TargetPS->GetName())
	UE_LOG(LogTemp, Warning, TEXT("%d"), TargetPS->LikeCounter)
}
