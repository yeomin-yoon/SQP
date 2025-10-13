// Fill out your copyright notice in the Description page of Project Settings.

#include "SQPPaintBallProjectile.h"
#include "SQPPaintWorldSubsystem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ASQPPaintBallProjectile::ASQPPaintBallProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASQPPaintBallProjectile::BeginPlay()
{
	Super::BeginPlay();

	//서버 혹은 클라이언트에 따라 실제 작동 여부 결정
	if (bIsReal)
	{
		//바인드
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASQPPaintBallProjectile::OnOverlapBeginCallback);	
	}
}

void ASQPPaintBallProjectile::OnOverlapBeginCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("PaintBall BeginOverlap!"));
	
	if (HasAuthority())
	{
		//채색 시도 명령
		Multicast_TryPaint();
		
		//비활성화
		InactivateProjectile();
	}
}

void ASQPPaintBallProjectile::Multicast_TryPaint_Implementation()
{
	const FVector Offset = GetActorForwardVector() * 100;
	const FVector Start = GetActorLocation() - Offset;
	const FVector End = GetActorLocation() + Offset;
	const uint8 BrushIndex = FMath::RandRange(0, 8);

	const TArray<AActor*> ActorsToIgnore { this };
	if (FHitResult OutHitResult; UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceTypeQuery1, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHitResult, true))
	{
		if (const auto Subsystem = GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>())
		{
			constexpr float BrushSize = 500;
			
			UTextureRenderTarget2D* ColorRenderTarget = nullptr;
			UTextureRenderTarget2D* NormalRenderTarget = nullptr;
			Subsystem->GetRenderTargetFromHit(OutHitResult, ColorRenderTarget, NormalRenderTarget);
		
			FVector2D DrawLocation;
			UGameplayStatics::FindCollisionUV(OutHitResult, 0, DrawLocation);
				
			Subsystem->PaintRenderTarget(BrushIndex, BrushSize, DrawLocation, ColorRenderTarget, NormalRenderTarget);		
		}
	}
}