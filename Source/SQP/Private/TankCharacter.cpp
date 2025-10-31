// Fill out your copyright notice in the Description page of Project Settings.


#include "TankCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "MainUIComponent.h"
#include "ProjectileShooterComponent.h"
#include "SkyViewComponent.h"
#include "SwimComponent.h"
#include "UIInteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ATankCharacter::ATankCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction> FireAsset(
		TEXT("'/Game/Splatoon/Input/IA_Fire.IA_Fire'")
	);
	if (FireAsset.Succeeded())
	{
		FireAction = FireAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCAsset(
		TEXT("'/Game/Input/IMC_Default.IMC_Default'")
	);
	if (IMCAsset.Succeeded())
	{
		IMC = IMCAsset.Object;
	}
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(
		TEXT("'/Game/Assets/Tanks/red_tank/SkeletalMeshes/red_tank.red_tank'")
	);
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -55.4f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetRelativeScale3D(FVector(0.006f));
	}

	SetRootComponent(GetCapsuleComponent());
	
	ProjectileShooter = CreateDefaultSubobject<UProjectileShooterComponent>(TEXT("ShooterComp"));
	ProjectileShooter->SetupAttachment(GetMesh(), FName("TurretBarrel"));

	BoomOffset = CreateDefaultSubobject<USceneComponent>(TEXT("BoomOffset"));
	BoomOffset->SetupAttachment(FollowCamera);

	InteractionBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("InteractionBoom"));
	InteractionBoom->SetupAttachment(BoomOffset);

	InteractionComp = CreateDefaultSubobject<UUIInteractionComponent>(TEXT("InteractionComp"));
	InteractionComp->SetupAttachment(InteractionBoom);
	InteractionComp->SetRelativeLocation(FVector(0.f, 0.f, -55.f));

	SwimComp = CreateDefaultSubobject<USwimComponent>(TEXT("SwimComp"));

	SkyViewComp = CreateDefaultSubobject<USkyViewComponent>(TEXT("SkyViewComp"));

	bReplicates = true;
}

// Called when the game starts or when spawned
void ATankCharacter::BeginPlay()
{
	Super::BeginPlay();
	// if (HasAuthority() && IsLocallyControlled())
	// {
	// 	ProjectileShooter->UnregisterComponent();
	// }

	if (IsLocallyControlled())
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PC->GetLocalPlayer()))
			{
				InputSubsystem->AddMappingContext(IMC, 0);
			}
			if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
			{
				if (FireAction)
				{
					EIC->BindAction(FireAction, ETriggerEvent::Started, this, &ATankCharacter::StartFire);
					EIC->BindAction(FireAction, ETriggerEvent::Completed, this, &ATankCharacter::CompleteFire);
				}
			}
		}
	}
}

// Called every frame
void ATankCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsInactivePawn)
		return;
	if (GetPlayerState())
	{
		DrawDebugString(GetWorld(), GetActorLocation(), GetPlayerState()->GetPlayerName(), nullptr, FColor::Red, 0);
	}
}

// Called to bind functionality to input
void ATankCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATankCharacter::StartFire()
{
	ProjectileShooter->PullTrigger();
}

void ATankCharacter::CompleteFire()
{
	ProjectileShooter->ReleaseTrigger();
}
