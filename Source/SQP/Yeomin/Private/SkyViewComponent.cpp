#include "SkyViewComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SkyViewPawn.h"
#include "SQP_PC_PaintRoom.h"
#include "Camera/CameraComponent.h"

USkyViewComponent::USkyViewComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UInputAction> SkyViewAsset(
		TEXT("'/Game/Splatoon/Input/IA_SkyView.IA_SkyView'")
	);
	if (SkyViewAsset.Succeeded())
	{
		SkyViewAction = SkyViewAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCAsset(
		TEXT("'/Game/Input/IMC_Default.IMC_Default'")
	);
	if (IMCAsset.Succeeded())
	{
		IMC = IMCAsset.Object;
	}
}


void USkyViewComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<APawn>(GetOwner());

	if (OwnerPawn && OwnerPawn->IsLocallyControlled())
	{
		// Enhanced Input Subsystem 등록
		PlayerController = Cast<ASQP_PC_PaintRoom>(OwnerPawn->GetController());
		if (PlayerController)
		{
			if (auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PlayerController->GetLocalPlayer()))
			{
				InputSubsystem->AddMappingContext(IMC, 0);
			}

			// 입력 바인딩
			if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				// if (SkyViewAction)
				// 	EIC->BindAction(SkyViewAction, ETriggerEvent::Started, this, &USkyViewComponent::OnSkyView);
			}
		}
	}
}


void USkyViewComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USkyViewComponent::OnSkyView()
{
	if (OwnerPawn->IsLocallyControlled())
	{
		
	}
}
