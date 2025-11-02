#include "MainUIComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "LikeUI.h"
#include "MainUI.h"
#include "SQP_PC_PaintRoom.h"
#include "SQP_PS_Master.h"
#include "SQP_PS_PaintRoomComponent.h"
#include "UIManager.h"
#include "Components/AudioComponent.h"
#include "Components/BackgroundBlur.h"
#include "Components/Button.h"
#include "Components/RichTextBlock.h"
#include "Components/ScaleBox.h"
#include "Components/Slider.h"
#include "Components/WidgetComponent.h"
#include "Features/UEaseFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Shared/FEaseHelper.h"


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
	LikeUIComp->SetWidgetSpace(EWidgetSpace::World);
	LikeUIComp->SetupAttachment(this);

	ConstructorHelpers::FObjectFinder<UInputAction> ToggleMouseAsset(
		TEXT("'/Game/Splatoon/Input/IA_ToggleMouse.IA_ToggleMouse'")
	);
	if (ToggleMouseAsset.Succeeded())
	{
		ToggleMouseAction = ToggleMouseAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> WheelAsset(
		TEXT("'/Game/Splatoon/Input/IA_WheelAction.IA_WheelAction'")
	);
	if (WheelAsset.Succeeded())
	{
		IA_WheelAction = WheelAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCAsset(
		TEXT("'/Game/Input/IMC_Default.IMC_Default'")
	);
	if (IMCAsset.Succeeded())
	{
		IMC = IMCAsset.Object;
	}

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->bAutoActivate = false;
	
	if (static ConstructorHelpers::FObjectFinder<USoundWave> USoundWave(
			TEXT("'/Game/Assets/Sounds/ThumpsUp.ThumpsUp'"));
		USoundWave.Succeeded())
	{
		AudioComp->SetSound(USoundWave.Object);
	}
}


void UMainUIComponent::OnClick()
{
	if (const APawn* TargetPawn = Cast<APawn>(GetOwner()))
	{
		if (ASQP_PS_Master* TargetPS = TargetPawn->GetPlayerState<ASQP_PS_Master>())
		{
			ASQP_PC_PaintRoom* LocalPC = Cast<ASQP_PC_PaintRoom>(GetWorld()->GetFirstPlayerController());
			LocalPC->Server_CountLike(TargetPS);
			if (LocalPC->IsLocalController())
			{
				
				AudioComp->Play();
			}
		}
	}
}

void UMainUIComponent::OnMouseWheel(const FInputActionValue& InputActionValue)
{
	float WheelAxis = InputActionValue.Get<float>();
	if (Slider)
	{
		float Current = Slider->GetValue();
		Slider->SetValue(Current + WheelAxis * 50.f);
	}
}

void UMainUIComponent::SetMainUIScale(float Scale)
{
	if (MainUI)
	{
		MainUI->MainUIScaleBox->SetRenderScale(FVector2D(Scale));
	}
}

void UMainUIComponent::SetBlurStrength(float Value)
{
	if (MainUI)
	{
		MainUI->BlurBackground->SetBlurStrength(Value);
	}
}

void UMainUIComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ULikeUI* LikeUI = Cast<ULikeUI>(LikeUIComp->GetWidget()))
	{
		LikeUI->LikeBtn->OnPressed.AddDynamic(this, &UMainUIComponent::OnClick);
	}
	LikeUIComp->SetRelativeLocation(FVector(0, 0.f, 0.f));

	UIManager = GetWorld()->GetGameInstance()->GetSubsystem<UUIManager>();

	// Only hide local player's LikeUI
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn->IsLocallyControlled())
	{
		MainUI = UIManager->CreateMainUI();
		LikeUIComp->SetVisibility(false);
		LikeUIComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MainUI->MainUIScaleBox->SetRenderScale(FVector2D(0.4f));
		Slider = MainUI->BrushSlider;
	}

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
			if (ToggleMouseAction)
				EIC->BindAction(ToggleMouseAction, ETriggerEvent::Started, this, &UMainUIComponent::OnToggleMouse);
			EIC->BindAction(ToggleMouseAction, ETriggerEvent::Completed, this, &UMainUIComponent::OffToggleMouse);
			if (IA_WheelAction)
			{
				EIC->BindAction(IA_WheelAction, ETriggerEvent::Started, this, &UMainUIComponent::OnMouseWheel);
			}
		}
	}
}


void UMainUIComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	BillboardLikeUI();

	ElapsedTime += DeltaTime;

	if (bScaleUp)
	{
		SetMainUIScale(UEaseFunctionLibrary::LerpFloatEase(0.48f, 1.f, ElapsedTime / TimeSpeed, EEaseType::EaseOutExpo));
		if (ElapsedBlurStrength <= 3.4f)
		{
			ElapsedBlurStrength = UEaseFunctionLibrary::LerpFloatEase(0.f, 3.1f, ElapsedTime / TimeSpeed, EEaseType::EaseOutExpo);
			SetBlurStrength(ElapsedBlurStrength);
		}
		return;
	}
	SetMainUIScale(UEaseFunctionLibrary::LerpFloatEase(1.f, 0.48f, ElapsedTime / TimeSpeed, EEaseType::EaseOutExpo));
	if (ElapsedBlurStrength >= 0.f)
	{
		ElapsedBlurStrength = UEaseFunctionLibrary::LerpFloatEase(3.1f, 0.f, ElapsedTime / TimeSpeed, EEaseType::EaseOutExpo);
		SetBlurStrength(ElapsedBlurStrength);
	}
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
	ElapsedTime = 0.f;
	if (bCursorEnabled)
	{
		int32 SizeX, SizeY;
		PC->GetViewportSize(SizeX, SizeY);
		PC->SetMouseLocation(SizeX * 80 / 100, SizeY * 27 / 100);
		PC->bShowMouseCursor = bCursorEnabled;

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetWidgetToFocus(nullptr);
		PC->SetInputMode(InputMode);
		bCursorEnabled = false;

		bScaleUp = true;
		return;
	}
	PC->bShowMouseCursor = bCursorEnabled;
	PC->SetInputMode(FInputModeGameOnly());
	bCursorEnabled = true;

	bScaleUp = false;
}

void UMainUIComponent::OffToggleMouse()
{
}
