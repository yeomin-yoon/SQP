#include "MainUIComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "UIManager.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


UMainUIComponent::UMainUIComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LikeUIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("LikeWidget"));

	ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(
		TEXT("Class'/Game/Splatoon/UI/WBP_LikeUI.WBP_LikeUI_C'"));
	if (WidgetClassFinder.Succeeded())
	{
		LikeUIComp->SetWidgetClass(WidgetClassFinder.Class);
	}
	LikeUIComp->SetupAttachment(this);
	LikeUIComp->SetWidgetSpace(EWidgetSpace::World);

	ConstructorHelpers::FObjectFinder<UInputAction> ToggleMouseAsset(
		TEXT("'/Game/Splatoon/Input/IA_ToggleMouse.IA_ToggleMouse'")
	);
	if (ToggleMouseAsset.Succeeded())
	{
		ToggleMouseAction = ToggleMouseAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCAsset(
	TEXT("'/Game/Input/IMC_Default.IMC_Default'")
);
	if (IMCAsset.Succeeded())
	{
		IMC = IMCAsset.Object;
	}
}


void UMainUIComponent::BeginPlay()
{
	Super::BeginPlay();

	LikeUIComp->SetupAttachment(GetOwner()->GetRootComponent());
	LikeUIComp->SetRelativeLocation(FVector(0, 0, 130));

	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();

	UIManager->CreateMainUI();

	// Only hide local player's LikeUI
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn->IsLocallyControlled())
	{
		LikeUIComp->SetVisibility(false);

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
				if (ToggleMouseAction)
					EIC->BindAction(ToggleMouseAction, ETriggerEvent::Started, this, &UMainUIComponent::OnToggleMouse);
			}
		}
	}
}


void UMainUIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	BillboardLikeUI();
}

void UMainUIComponent::BillboardLikeUI()
{
	// 내가 컨트롤하고 있는 카메라를 가져오자.
	auto* Cam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	// 카메라의 앞 방향 (반대), 윗 방향을 이용해서 Rotator 를 구하자.
	FRotator Rot = UKismetMathLibrary::MakeRotFromXZ(-Cam->GetActorForwardVector(), Cam->GetActorUpVector());
	// 구한 Rotator 를 comHP 에 설정
	LikeUIComp->SetWorldRotation(Rot);
}

void UMainUIComponent::OnToggleMouse(const FInputActionValue& InputActionValue)
{
	if (bCursorEnabled)
	{
		int32 SizeX, SizeY;
		PC->GetViewportSize(SizeX, SizeY);
		PC->SetMouseLocation(SizeX / 2, SizeY / 2);
		PC->bShowMouseCursor = bCursorEnabled;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetWidgetToFocus(nullptr);
		PC->SetInputMode(InputMode);
		bCursorEnabled = false;
		return;
	}
	PC->bShowMouseCursor = bCursorEnabled;
	PC->SetInputMode(FInputModeGameOnly());
	bCursorEnabled = true;
}
