#include "TestPlayer.h"

#include "Engine/Canvas.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"


// Sets default values
ATestPlayer::ATestPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	//페인트 텍스처 로드
	for (int32 i = 1; i <= 9; i++)
	{
		FString Path = FString::Printf(TEXT("/Game/Splatoon/Texture/T_Ink_0%d_Color.T_Ink_0%d_Color"), i, i);
		if (ConstructorHelpers::FObjectFinder<UTexture2D>
			Finder(*Path);
			Finder.Succeeded())
		{
			ColorTextureArray.Emplace(Finder.Object);
		}

		Path = FString::Printf(TEXT("/Game/Splatoon/Texture/T_Ink_0%d_Normal.T_Ink_0%d_Normal"), i, i);
		if (ConstructorHelpers::FObjectFinder<UTexture2D>
			Finder(*Path);
			Finder.Succeeded())
		{
			NormalTextureArray.Emplace(Finder.Object);
		}
	}

	//컬러 브러시의 머터리얼 베이스 로드
	if (static ConstructorHelpers::FObjectFinder<UMaterial>
		Finder(TEXT("/Game/Splatoon/Material/M_SplatoonColorBrush.M_SplatoonColorBrush"));
		Finder.Succeeded())
	{
		ColorBrushMaterialBase = Finder.Object;
	}

	//노말 브러시의 머터링러 베이스 로드
	if (static ConstructorHelpers::FObjectFinder<UMaterial>
		Finder(TEXT("/Game/Splatoon/Material/M_SplatoonNormalBrush.M_SplatoonNormalBrush"));
		Finder.Succeeded())
	{
		NormalBrushMaterialBase = Finder.Object;
	}

	//캔버스의 머터리얼 베이스 로드
	if (static ConstructorHelpers::FObjectFinder<UMaterial>
		Finder(TEXT("/Game/Splatoon/Material/M_SplatoonCanvas.M_SplatoonCanvas"));
		Finder.Succeeded())
	{
		CanvasMaterialBase = Finder.Object;
	}
}

void ATestPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void ATestPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATestPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ATestPlayer::GetRenderTargetFromHit(const FHitResult& Hit, UTextureRenderTarget2D*& OutColorRenderTarget,
                                         UTextureRenderTarget2D*& OutNormalRenderTarget) const
{
	//충돌한 면에 있는 머터리얼 획득
	int32 SectionIndex;

	//충돌 컴포넌트가 없다면 Null 반환
	if (Hit.Component == nullptr)
	{
		return;
	}

	//충돌 컴포넌트에서 머터리얼 인터페이스 획득
	const auto MaterialInterface = Hit.Component->GetMaterialFromCollisionFaceIndex(Hit.FaceIndex, SectionIndex);

	//캔버스 머터리얼과 충돌했다면
	if (const auto BaseMaterialFromCollision = MaterialInterface->GetBaseMaterial(); BaseMaterialFromCollision ==
		CanvasMaterialBase)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Detect Paint Material!"));

		//머터리얼 다이나믹 인스턴스로의 형변환에 성공했다면
		if (const auto MatFromCollision = Cast<UMaterialInstanceDynamic>(MaterialInterface))
		{
			//렌더 타겟 텍스처를 획득해서 반환한다
			UTexture* ColorTexture;
			UTexture* NormalTexture;
			MatFromCollision->GetTextureParameterValue(FName("ColorRenderTarget"), ColorTexture);
			MatFromCollision->GetTextureParameterValue(FName("NormalRenderTarget"), NormalTexture);

			//렌더 타겟 텍스처로 형변환
			OutColorRenderTarget = Cast<UTextureRenderTarget2D>(ColorTexture);
			OutNormalRenderTarget = Cast<UTextureRenderTarget2D>(NormalTexture);
			return;
		}

		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("CreateDynamicMaterialInstance of Canvas!"));

		//머터리얼 다이나믹 인스턴스로의 형변환에 실패했다면 새로운 인스턴스를 할당받은 후에 반환한다
		const auto CreatedMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(
			GetWorld(), CanvasMaterialBase);

		//컬러 값을 가지는 렌더 타겟
		constexpr ETextureRenderTargetFormat ColorFormat = RTF_RGBA8;
		const auto CreatedColorRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(
			GetWorld(), 1024, 1024, ColorFormat, FLinearColor::White, false);

		//노말 값을 가지는 렌더 타겟
		constexpr ETextureRenderTargetFormat NormalFormat = RTF_RGBA16f;
		const auto CreatedNormalRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(
			GetWorld(), 1024, 1024, NormalFormat, FLinearColor(0.5, 0.5, 1.0, 1.0), false);
		if (CreatedNormalRenderTarget)
		{
			CreatedNormalRenderTarget->CompressionSettings = TC_VectorDisplacementmap;
			CreatedNormalRenderTarget->MipGenSettings = TMGS_NoMipmaps;
			CreatedNormalRenderTarget->UpdateResource();
		}

		//새롭게 할당받은 렌더 타겟 텍스처를 머터리얼에 적용
		CreatedMaterialInstance->SetTextureParameterValue(FName("ColorRenderTarget"), CreatedColorRenderTarget);
		CreatedMaterialInstance->SetTextureParameterValue(FName("NormalRenderTarget"), CreatedNormalRenderTarget);

		//새롭게 할당받은 다이나믹 머터리얼을 충돌 컴포넌트에 적용
		Hit.Component->SetMaterial(SectionIndex, CreatedMaterialInstance);

		//반환
		OutColorRenderTarget = CreatedColorRenderTarget;
		OutNormalRenderTarget = CreatedNormalRenderTarget;
	}
}

void ATestPlayer::PaintRenderTarget(const int32 BrushIndex, const float BrushSize, const FVector2D& DrawLocation,
                                    UTextureRenderTarget2D* ColorRenderTarget,
                                    UTextureRenderTarget2D* NormalRenderTarget)
{
	if (ColorRenderTarget)
	{
		PaintColorRenderTarget(ColorTextureArray[BrushIndex], BrushSize, DrawLocation, ColorRenderTarget);
	}

	if (NormalRenderTarget)
	{
		PaintNormalRenderTarget(NormalTextureArray[BrushIndex], BrushSize, DrawLocation, NormalRenderTarget);
	}
}

void ATestPlayer::PaintColorRenderTarget(UTexture2D* BrushTexture, const float BrushSize, const FVector2D& DrawLocation,
                                         UTextureRenderTarget2D* ColorRenderTarget)
{
	//만약 컬러 브러시 머터리얼의 다이나믹 인스턴스가 없다면
	if (ColorBrushMaterialDynamicInstance == nullptr)
	{
		//새로운 머터리얼을 하나 생성한다
		ColorBrushMaterialDynamicInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(
			GetWorld(), ColorBrushMaterialBase);
	}

	//브러시 머터리얼의 텍스처 패러미터를 요청한 텍스처로 교체
	ColorBrushMaterialDynamicInstance->SetTextureParameterValue(FName("BrushTexture"), BrushTexture);

	//드로우 객체 준비
	UCanvas* Canvas = NewObject<UCanvas>();
	FVector2D Size;
	FDrawToRenderTargetContext Context;

	//드로우 시작
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), ColorRenderTarget, Canvas, Size, Context);

	//드로잉
	Canvas->K2_DrawMaterial(ColorBrushMaterialDynamicInstance, (Size * DrawLocation - BrushSize / 2),
	                        FVector2D(BrushSize, BrushSize), FVector2D::ZeroVector);

	//드로우 종료
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
}

void ATestPlayer::PaintNormalRenderTarget(UTexture2D* BrushTexture, const float BrushSize,
                                          const FVector2D& DrawLocation, UTextureRenderTarget2D* NormalRenderTarget)
{
	//만약 컬러 브러시 머터리얼의 다이나믹 인스턴스가 없다면
	if (NormalBrushMaterialDynamicInstance == nullptr)
	{
		//새로운 머터리얼을 하나 생성한다
		NormalBrushMaterialDynamicInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(
			GetWorld(), ColorBrushMaterialBase);
	}

	//브러시 머터리얼의 텍스처 패러미터를 요청한 텍스처로 교체
	NormalBrushMaterialDynamicInstance->SetTextureParameterValue(FName("BrushTexture"), BrushTexture);

	//드로우 객체 준비
	UCanvas* Canvas = NewObject<UCanvas>();
	FVector2D Size;
	FDrawToRenderTargetContext Context;

	//드로우 시작
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), NormalRenderTarget, Canvas, Size, Context);

	//드로잉
	Canvas->K2_DrawMaterial(NormalBrushMaterialDynamicInstance, (Size * DrawLocation - BrushSize / 2),
	                        FVector2D(BrushSize, BrushSize), FVector2D::ZeroVector);

	//드로우 종료
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
}
