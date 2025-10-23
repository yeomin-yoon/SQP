// Fill out your copyright notice in the Description page of Project Settings.

#include "SQPPaintWorldSubsystem.h"

#include "PaintRoomSaveGame.h"
#include "SavablePaint.h"
#include "SQP.h"
#include "Engine/Canvas.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/KismetRenderingLibrary.h"

USQPPaintWorldSubsystem::USQPPaintWorldSubsystem()
{
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

	//노말 브러시의 머터리얼 베이스 로드
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

void USQPPaintWorldSubsystem::TryPaint(
	const FVector& Start,
	const FVector& End,
	const uint8& BrushIndex,
	const float& BrushSize)
{
	TArray<FHitResult> OutHitResult;
	const FCollisionQueryParams Params = FCollisionQueryParams(FName(TEXT("SplatoonPaint")), true);
	if (GetWorld()->LineTraceMultiByChannel(OutHitResult, Start, End, ECC_Visibility, Params))
	{
		for (auto Hit : OutHitResult)
		{
			if (Hit.GetActor())
			{
				UTextureRenderTarget2D* ColorRenderTarget = nullptr;
				UTextureRenderTarget2D* NormalRenderTarget = nullptr;
				GetRenderTargetFromHit(Hit, ColorRenderTarget, NormalRenderTarget);
		
				FVector2D DrawLocation;
				UGameplayStatics::FindCollisionUV(Hit, 0, DrawLocation);

				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red,FString::Printf(TEXT("%d %f"), BrushIndex, BrushSize));

				//PaintRenderTarget(BrushIndex, BrushSize, , DrawLocation, ColorRenderTarget, NormalRenderTarget);
				
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red,TEXT("This Function has been deprecated! Check Again!"));
			}
		}
	}
}

void USQPPaintWorldSubsystem::TryPaintColor(
	const FVector& Start,
	const FVector& End,
	const TArray<AActor*>& ActorsToIgnore,
	const uint8& BrushIndex,
	const float& BrushSize,
	const FLinearColor& BrushColor)
{
	if (FHitResult OutHitResult; UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceTypeQuery1, true, ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHitResult, true))
	{
		UTextureRenderTarget2D* ColorRenderTarget = nullptr;
		UTextureRenderTarget2D* NormalRenderTarget = nullptr;
		GetRenderTargetFromHit(OutHitResult, ColorRenderTarget, NormalRenderTarget);

		//드로잉에 필요한 UV좌표를 획득
		FVector2D BrushLocation;
		UGameplayStatics::FindCollisionUV(OutHitResult, 0, BrushLocation);

		//저장-로드 인터페이스를 구현 중이라면
		if (const auto SaveActor = Cast<ISavablePaint>(OutHitResult.GetActor()))
		{
			//페인트 실행 데이터로 패러미터를 저장해놓는다
			FPaintExecutionData Data(SaveActor->GetPersistantActorID(), OutHitResult.FaceIndex, BrushIndex ,BrushColor, BrushLocation, BrushSize);
			PaintExecutionDataArray.Emplace(Data);

			PRINTLOG(TEXT("Successfully Store PaintExecutionData in the Array!"));
		}

		//페인트 월드 서브 시스템에 페인트 요청
		PaintRenderTarget(BrushIndex, BrushSize, BrushColor, BrushLocation, ColorRenderTarget, NormalRenderTarget);
	}
}

void USQPPaintWorldSubsystem::GetRenderTargetFromHit(
	const FHitResult& Hit,
	UTextureRenderTarget2D*& OutColorRenderTarget,
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

	if (MaterialInterface == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Fail to Get Material from Collision"));
		return;
	}

	//캔버스 머터리얼과 충돌했다면
	if (CheckCanvasMaterialBase(MaterialInterface))
	{
		PRINTLOG(TEXT("Detect Splatoon Canvas Material!"));
		
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
		
		//머터리얼 다이나믹 인스턴스를 하나 생성하고 내부의 컬러 렌더 타겟과 노말 렌더 타겟까지 반환
		UMaterialInstanceDynamic* CreatedMaterialInstance;
		CreateCanvasMaterialInstanceDynamic(CreatedMaterialInstance, OutColorRenderTarget, OutNormalRenderTarget);

		//새롭게 할당받은 다이나믹 머터리얼을 충돌 컴포넌트에 적용
		Hit.Component->SetMaterial(SectionIndex, CreatedMaterialInstance);
	}
}

void USQPPaintWorldSubsystem::PaintRenderTarget(
	const uint8& BrushIndex,
	const float& BrushSize,
	const FLinearColor& BrushColor,
	const FVector2D& BrushLocation,
	UTextureRenderTarget2D* ColorRenderTarget,
	UTextureRenderTarget2D* NormalRenderTarget)
{
	if (ColorRenderTarget)
	{
		PaintColorRenderTarget(ColorTextureArray[BrushIndex], BrushSize, BrushColor, BrushLocation, ColorRenderTarget);
	}

	if (NormalRenderTarget)
	{
		PaintNormalRenderTarget(NormalTextureArray[BrushIndex], ColorTextureArray[BrushIndex], BrushSize, BrushLocation, NormalRenderTarget);
	}
}

void USQPPaintWorldSubsystem::PaintColorRenderTarget(
	UTexture2D* BrushTexture,
	const float& BrushSize,
	const FLinearColor& BrushColor,
	const FVector2D& BrushLocation,
	UTextureRenderTarget2D* ColorRenderTarget)
{
	//만약 컬러 브러시 머터리얼의 다이나믹 인스턴스가 없다면
	if (ColorBrushMaterialDynamicInstance == nullptr)
	{
		//새로운 머터리얼을 하나 생성한다
		ColorBrushMaterialDynamicInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), ColorBrushMaterialBase);
	}
	
	//브러시 머터리얼의 텍스처 패러미터를 요청한 텍스처로 교체
	ColorBrushMaterialDynamicInstance->SetVectorParameterValue(FName("PaintColor"), BrushColor);
	ColorBrushMaterialDynamicInstance->SetTextureParameterValue(FName("BrushTexture"), BrushTexture);

	//드로우 객체 준비
	UCanvas* Canvas = NewObject<UCanvas>();
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	
	//드로우 시작
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), ColorRenderTarget, Canvas, Size, Context);

	//드로잉
	Canvas->K2_DrawMaterial(ColorBrushMaterialDynamicInstance, (Size * BrushLocation - BrushSize / 2), FVector2D(BrushSize, BrushSize), FVector2D::ZeroVector);

	//드로우 종료
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
}

void USQPPaintWorldSubsystem::PaintNormalRenderTarget(
	UTexture2D* BrushTexture,
	UTexture2D* BrushAlphaTexture,
	const float BrushSize,
	const FVector2D& BrushLocation,
	UTextureRenderTarget2D* NormalRenderTarget)
{
	//만약 컬러 브러시 머터리얼의 다이나믹 인스턴스가 없다면
	if (NormalBrushMaterialDynamicInstance == nullptr)
	{
		//새로운 머터리얼을 하나 생성한다
		NormalBrushMaterialDynamicInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), NormalBrushMaterialBase);
	}
	
	//브러시 머터리얼의 텍스처 패러미터를 요청한 텍스처로 교체
	NormalBrushMaterialDynamicInstance->SetTextureParameterValue(FName("BrushTexture"), BrushTexture);
	NormalBrushMaterialDynamicInstance->SetTextureParameterValue(FName("BrushAlphaTexture"), BrushAlphaTexture);

	//드로우 객체 준비
	UCanvas* Canvas = NewObject<UCanvas>();
	FVector2D Size;
	FDrawToRenderTargetContext Context;
	
	//드로우 시작
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), NormalRenderTarget, Canvas, Size, Context);

	//드로잉
	Canvas->K2_DrawMaterial(NormalBrushMaterialDynamicInstance, (Size * BrushLocation - BrushSize / 2), FVector2D(BrushSize, BrushSize), FVector2D::ZeroVector);

	//드로우 종료
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), Context);
}

void USQPPaintWorldSubsystem::SavePaintOfWorld()
{
	//저장 객체
	const auto SaveGame = Cast<UPaintRoomSaveGame>(UGameplayStatics::CreateSaveGameObject(UPaintRoomSaveGame::StaticClass()));

	//덮어쓰기
	SaveGame->PaintExecutionDataArray = this->PaintExecutionDataArray;
	
	//게임 세이브를 슬롯에 저장
	UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("MySlotName"), 0);

	PRINTLOG(TEXT("PaintWorldSubsystem Save PaintTextureData!"));
}

void USQPPaintWorldSubsystem::LoadPaintOfWorld()
{
	//슬롯에 저장되어 있던 게임 세이브를 로드
	if (const auto LoadGame = Cast<UPaintRoomSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("MySlotName"), 0)))
	{
		//저장-로드 기능을 호출할 수 있는 인터페이스를 가진 액터 검색
		TArray<AActor*>	OutActors;
		UGameplayStatics::GetAllActorsWithInterface(this, USavablePaint::StaticClass(), OutActors);

		//빠른 작업을 위해서 고유 ID와 액터를 이어주는 맵 구축
		TMap<FGuid, AActor*> ActorMap; 
		for (AActor* OutActor : OutActors)
		{
			const auto SaveActor = Cast<ISavablePaint>(OutActor);
			ActorMap.Emplace(SaveActor->GetPersistantActorID(), OutActor);
		}

		//로드
		this->PaintExecutionDataArray = LoadGame->PaintExecutionDataArray;

		//구조화돤 바인딩?
		for (const auto [PersistantUniqueID, FaceIndex, BrushIndex, BrushColor, BrushLocation, BrushSize] : this->PaintExecutionDataArray)
		{
			//고유 ID를 이용하여 대상 액터를 획득
			const AActor* Target = ActorMap[PersistantUniqueID];

			//스태틱 메시 컴포넌트 획득에 성공했다면
			if (UStaticMeshComponent* MeshComp = Target->GetComponentByClass<UStaticMeshComponent>())
			{
				//저장되어 있던 면 인덱스를 이용하여 머터리얼 획득
				int32 SectionIndex;
				if (UMaterialInterface* MaterialInterface = MeshComp->GetMaterialFromCollisionFaceIndex(FaceIndex, SectionIndex))
				{
					//건너뛰기
					if (CheckCanvasMaterialBase(MaterialInterface) == false)
					{
						continue;
					}

					//머터리얼 다이나믹 인스턴스로의 형변환에 성공했다면
					if (const auto MatFromCollision = Cast<UMaterialInstanceDynamic>(MaterialInterface))
					{
						//렌더 타겟 텍스처를 획득해서 반환한다
						UTexture* ColorTexture;
						UTexture* NormalTexture;
						MatFromCollision->GetTextureParameterValue(FName("ColorRenderTarget"), ColorTexture);
						MatFromCollision->GetTextureParameterValue(FName("NormalRenderTarget"), NormalTexture);

						//렌더 타겟 텍스처로 형변환
						UTextureRenderTarget2D* OutColorRenderTarget = Cast<UTextureRenderTarget2D>(ColorTexture);
						UTextureRenderTarget2D* OutNormalRenderTarget = Cast<UTextureRenderTarget2D>(NormalTexture);

						//채색 시도
						PaintRenderTarget(BrushIndex, BrushSize, BrushColor, BrushLocation, OutColorRenderTarget, OutNormalRenderTarget);
						continue;
					}
						
					//머터리얼 다이나믹 인스턴스를 하나 생성하고 내부의 컬러 렌더 타겟과 노말 렌더 타겟까지 반환
					UMaterialInstanceDynamic* CreatedMaterialInstance;
					UTextureRenderTarget2D* OutColorRenderTarget;
					UTextureRenderTarget2D* OutNormalRenderTarget;
					CreateCanvasMaterialInstanceDynamic(CreatedMaterialInstance, OutColorRenderTarget, OutNormalRenderTarget);

					//채색 시도
					PaintRenderTarget(BrushIndex, BrushSize, BrushColor, BrushLocation, OutColorRenderTarget, OutNormalRenderTarget);

					//머터리얼 할당
					MeshComp->SetMaterial(SectionIndex, CreatedMaterialInstance);
				}
			}
			
		}
	}
}

bool USQPPaintWorldSubsystem::CheckCanvasMaterialBase(UMaterialInterface* Target) const
{
	return Target->GetBaseMaterial() == CanvasMaterialBase;
}

void USQPPaintWorldSubsystem::CreateCanvasMaterialInstanceDynamic(
	UMaterialInstanceDynamic*& OutMaterialInstanceDynamic,
	UTextureRenderTarget2D*& OutColorRenderTarget,
	UTextureRenderTarget2D*& OutNormalRenderTarget) const
{
	//머터리얼 다이나믹 인스턴스로의 형변환에 실패했다면 새로운 인스턴스를 할당받은 후에 반환한다
	const auto CreatedMaterialInstance = UKismetMaterialLibrary::CreateDynamicMaterialInstance(GetWorld(), CanvasMaterialBase);

	//컬러 값을 가지는 렌더 타겟
	constexpr ETextureRenderTargetFormat ColorFormat = RTF_RGBA8;
	const auto CreatedColorRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, ColorFormat, FLinearColor::White, false);

	//노말 값을 가지는 렌더 타겟
	constexpr ETextureRenderTargetFormat NormalFormat = RTF_RGBA16f;
	const auto CreatedNormalRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, NormalFormat, FLinearColor(0, 0, 1.0, 1.0), false);
	if (CreatedNormalRenderTarget)
	{
		CreatedNormalRenderTarget->CompressionSettings = TC_VectorDisplacementmap;
		CreatedNormalRenderTarget->MipGenSettings = TMGS_NoMipmaps;
		CreatedNormalRenderTarget->UpdateResource();
		CreatedNormalRenderTarget->SRGB = false;
	}

	//새롭게 할당받은 렌더 타겟 텍스처를 머터리얼에 적용
	CreatedMaterialInstance->SetTextureParameterValue(FName("ColorRenderTarget"), CreatedColorRenderTarget);
	CreatedMaterialInstance->SetTextureParameterValue(FName("NormalRenderTarget"), CreatedNormalRenderTarget);

	//반환값
	OutMaterialInstanceDynamic = CreatedMaterialInstance;
	OutColorRenderTarget = CreatedColorRenderTarget;
	OutNormalRenderTarget = CreatedNormalRenderTarget;
	
	PRINTLOG(TEXT("Create Material Instance Dynamic of Splatoon Canvas"));
}

// UTexture2D* USQPPaintWorldSubsystem::CreateTextureFromPixelData(const int32 Width, const int32 Height, const TArray<FLinearColor>& Pixels)
// {
// 	if (Width <= 0 || Height <= 0 || Pixels.Num() == 0)
// 	{
// 		return nullptr;
// 	}
//
// 	//임시 텍스처 생성
// 	UTexture2D* NewTexture = UTexture2D::CreateTransient(Width, Height, EPixelFormat::PF_B8G8R8A8);
// 	if (NewTexture == nullptr)
// 	{
// 		return nullptr;
// 	}
//
// 	//픽셀 데이터를 텍스처에 복사하기 위해 메모리를 잠금
// 	void* TextureData = NewTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
//     
// 	//저장했던 픽셀 데이터를 텍스처 메모리에 복사
// 	FMemory::Memcpy(TextureData, Pixels.GetData(), Pixels.Num() * sizeof(FColor));
//     
// 	//메모리 잠금을 해제
// 	NewTexture->GetPlatformData()->Mips[0].BulkData.Unlock();
//     
// 	//텍스처 리소스를 업데이트하여 변경 사항을 적용
// 	NewTexture->UpdateResource();
//
// 	return NewTexture;
// }