// Fill out your copyright notice in the Description page of Project Settings.

#include "SQPPaintWorldSubsystem.h"

#include "CborTypes.h"
#include "SQP_SG_PaintRoom.h"
#include "PropertyAccess.h"
#include "SavablePaint.h"
#include "SQP.h"
#include "SQP_GI.h"
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
	if (static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		Finder(TEXT("/Game/Splatoon/Material/M_SplatoonColorBrush.M_SplatoonColorBrush"));
		Finder.Succeeded())
	{
		ColorBrushMaterialBase = Finder.Object;
	}

	//노말 브러시의 머터리얼 베이스 로드
	if (static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		Finder(TEXT("/Game/Splatoon/Material/M_SplatoonNormalBrush.M_SplatoonNormalBrush"));
		Finder.Succeeded())
	{
		NormalBrushMaterialBase = Finder.Object;
	}
	//캔버스의 머터리얼 베이스 로드
	if (static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		Finder(TEXT("/Game/Splatoon/Material/M_SplatoonCanvas.M_SplatoonCanvas"));
		Finder.Succeeded())
	{
		CanvasMaterialBase = Finder.Object;
	}
}

#pragma region 컬러 렌더 타겟과 노말 렌더 타겟에 브러시 머터리얼로 채색하는 메서드 시리즈

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

		//대상 액터
		AActor* TargetActor = OutHitResult.GetActor();
		
		//대상이 저장-로드 인터페이스를 구현 중이라면
		if (const auto SaveActor = Cast<ISavablePaint>(TargetActor))
		{
			//고유 ID 획득
			const FGuid ID = SaveActor->GetPersistantActorID(); 
			
			//페인트 실행 데이터로 패러미터를 저장해놓는다
			FPaintExecutionData Data(ID, OutHitResult.FaceIndex, BrushIndex ,BrushColor, BrushLocation, BrushSize);
			
			//ID와 매칭되는 배열을 검색하거나 생성한다
			FPaintExecutionDataWrapper& Wrapper = TemporalPEDContainer.FindOrAdd(ID);
			Wrapper.PEDArray.Emplace(Data);
			
			PRINTLOG(TEXT("Successfully Store PED of %s! Length = %d"), *TargetActor->GetActorNameOrLabel(), Wrapper.PEDArray.Num());
		}

		//페인트 월드 서브 시스템에 페인트 요청
		PaintRenderTarget(BrushIndex, BrushSize, BrushColor, BrushLocation, ColorRenderTarget, NormalRenderTarget);
	}
}

void USQPPaintWorldSubsystem::GetRenderTargetFromHit(
	const FHitResult& Hit,
	UTextureRenderTarget2D*& OutColorRenderTarget,
	UTextureRenderTarget2D*& OutNormalRenderTarget)
{
	//충돌한 면에 있는 머터리얼 획득
	int32 SectionIndex;

	//충돌 컴포넌트가 없다면 Null 반환
	if (Hit.Component == nullptr)
	{
		return;
	}

	//충돌 컴포넌트에서 머터리얼 인터페이스 획득
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("%s"), *Hit.Component->GetOwner()->GetActorNameOrLabel()));
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

void USQPPaintWorldSubsystem::ClearPaint(AActor* Target)
{
	if (const auto MeshComp = Target->GetComponentByClass<UStaticMeshComponent>())
	{
		for (const auto MaterialInterface : MeshComp->GetMaterials())
		{
			if (CheckCanvasMaterialBase(MaterialInterface))
			{
				//머터리얼 다이나믹 인스턴스로의 형변환에 성공했다면
				if (const auto MID = Cast<UMaterialInstanceDynamic>(MaterialInterface))
				{
					//컬러 값을 가지는 렌더 타겟
					constexpr ETextureRenderTargetFormat ColorFormat = RTF_RGBA8;
					const auto CreatedColorRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, ColorFormat, FLinearColor::White, false);

					//노말 값을 가지는 렌더 타겟
					constexpr ETextureRenderTargetFormat NormalFormat = RTF_RGBA16f;
					const auto CreatedNormalRenderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024, NormalFormat, FLinearColor(0, 0, 1.0, 1.0), false);
					if (CreatedNormalRenderTarget)
					{
						CreatedNormalRenderTarget->CompressionSettings = TC_VectorDisplacementmap;
						//CreatedNormalRenderTarget->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
						CreatedNormalRenderTarget->MipLoadOptions = ETextureMipLoadOptions::OnlyFirstMip;
						CreatedNormalRenderTarget->UpdateResource();
						CreatedNormalRenderTarget->SRGB = false;
					}

					//렌더 타겟 텍스처를 획득해서 
					MID->SetTextureParameterValue(FName("ColorRenderTarget"), CreatedColorRenderTarget);
					MID->SetTextureParameterValue(FName("NormalRenderTarget"), CreatedNormalRenderTarget);
				}
			}
		}
	}
}

void USQPPaintWorldSubsystem::ResetCanvasMaterialUV(AActor* Target)
{
	const FVector Scale = Target->GetActorScale3D();

	PRINTLOG(TEXT("%s"), *Scale.ToString());
	
	if (const auto MeshComp = Target->GetComponentByClass<UStaticMeshComponent>())
	{
		const TArray<UMaterialInterface*> MaterialInterfaces = MeshComp->GetMaterials();
		for (int32 i = 0; i < MaterialInterfaces.Num(); i++)
		{
			if (CheckCanvasMaterialBase(MaterialInterfaces[i]))
			{
				//머터리얼 다이나믹 인스턴스로의 형변환에 성공했다면
				if (const auto MID = Cast<UMaterialInstanceDynamic>(MaterialInterfaces[i]))
				{
					//패러미터 설정
					MID->SetVectorParameterValue(FName("TilingAndOffset"), FVector4(1 * Scale.X, 1 * Scale.Y, 0, 0));
				}
				else
				{
					//새로운 MID 생성
					UMaterialInstanceDynamic* CreatedMaterialInstance;
					UTextureRenderTarget2D* OutColorRenderTarget;
					UTextureRenderTarget2D* OutNormalRenderTarget;
					CreateCanvasMaterialInstanceDynamic(CreatedMaterialInstance, OutColorRenderTarget, OutNormalRenderTarget);

					//패러미터 설정
					CreatedMaterialInstance->SetVectorParameterValue(FName("TilingAndOffset"), FVector4(1 * Scale.X, 1 * Scale.Y, 0, 0));

					//생성한 MID로 바꿔치기
					MeshComp->SetMaterial(i, CreatedMaterialInstance);
				}
			}	
		}	
	}
}

#pragma endregion 

#pragma region 페인트 실행 데이터를 저장하고 다시 로드하는 메서드 시리즈

void USQPPaintWorldSubsystem::SavePaintOfWorld(const FString& PaintRoomSaveName)
{
	//저장 객체
	const auto SaveGame = Cast<USQP_SG_PaintRoom>(UGameplayStatics::CreateSaveGameObject(USQP_SG_PaintRoom::StaticClass()));

	//덮어쓰기
	SaveGame->PEDContainer = TemporalPEDContainer;

	for (auto Pair : TemporalPEDContainer)
	{
		PRINTLOG(TEXT("Now Saving %s PED... : Length %d"), *Pair.Key.ToString(), Pair.Value.PEDArray.Num());
	}

	for (auto Pair : SaveGame->PEDContainer)
	{
		PRINTLOG(TEXT("Successfully Store %s PED... : Length %d"), *Pair.Key.ToString(), Pair.Value.PEDArray.Num());
	}

	//게임 인스턴스에 저장 요청
	Cast<USQP_GI>(GetWorld()->GetGameInstance())->SavePaintRoomData(PaintRoomSaveName, FGuid::NewGuid(), SaveGame);

	PRINTLOG(TEXT("PaintWorldSubsystem Save PaintTextureData!"));
}

void USQPPaintWorldSubsystem::LoadPaintOfWorld(TMap<FGuid, FPaintExecutionDataWrapper>& PEDContainer)
{
//저장-로드 기능을 호출할 수 있는 인터페이스를 가진 액터 검색
		TArray<AActor*>	OutActors;
		UGameplayStatics::GetAllActorsWithInterface(this, USavablePaint::StaticClass(), OutActors);

		//빠른 작업을 위해서 고유 ID와 액터를 이어주는 맵부터 구축
		TMap<FGuid, AActor*> ActorMap; 
		for (AActor* OutActor : OutActors)
		{
			const auto SaveActor = Cast<ISavablePaint>(OutActor);
			ActorMap.Emplace(SaveActor->GetPersistantActorID(), OutActor);
		}

		//게임 세이브 데이터를 복사해온다
		TemporalPEDContainer = PEDContainer;

		//모든 페인트 액터에 대해서 순회하며
		for (const auto Pair : ActorMap)
		{
			//대상의 ID와 포인터 획득
			const FGuid TargetID = Pair.Key;
			const AActor* TargetActor = Pair.Value;

			//대상의 PED에 접근 시도
			if (TemporalPEDContainer.Find(TargetID) == nullptr)
			{
				continue;
			}
			TArray<FPaintExecutionData> TargetPedContainer = TemporalPEDContainer[TargetID].PEDArray;
			
			PRINTLOG(TEXT("PEDContainer Length %s : %d"), *TargetActor->GetActorNameOrLabel(), TargetPedContainer.Num());
			
			//대상의 스태틱 메시 컴포넌트에 접근 시도
			UStaticMeshComponent* MeshComp = TargetActor->GetComponentByClass<UStaticMeshComponent>();
			if (MeshComp == nullptr)
			{
				continue;
			}

			//구조화돤 바인딩? - 대상에게 필요한 PED 구조체 배열을 순회하며
			for (const auto [PersistantUniqueID, FaceIndex, BrushIndex, BrushColor, BrushLocation, BrushSize] : TargetPedContainer)
			{
				PRINTLOG(TEXT("Try to Repaint %s"), *TargetActor->GetActorNameOrLabel());
				
				//저장되어 있던 면 인덱스를 이용하여 머터리얼 획득
				int32 SectionIndex;
				UMaterialInterface* MaterialInterface = MeshComp->GetMaterialFromCollisionFaceIndex(FaceIndex, SectionIndex);
				if (MaterialInterface == nullptr)
				{
					continue;
				}

				//캔버스 머터리얼이 아니라면 적절한 작업이 불가능하므로
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
				}
				else
				{
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

TArray<FPaintExecutionData> USQPPaintWorldSubsystem::FilterPrimaryPaintExecutionData(const TArray<FPaintExecutionData>& TargetData, const float SimilarityLimit)
{
	//최적화의 의미가 없다면
	if (TargetData.Num() < 2)
    {
        return TargetData;
    }

    //제곱근 사용을 피하고 거리 비교 연산 최적화를 위해 제곱값을 사용
    const float LimitSquared = SimilarityLimit * SimilarityLimit;

    //필터링 결과물이 담길 배열
    TArray<FPaintExecutionData> FilteredData;

	//데이터 규모가 클 경우 동적 배열의 재할당이 빈번해질 수도 있다
    FilteredData.Reserve(TargetData.Num());
	
    for (const FPaintExecutionData& NewData : TargetData)
    {
        //다음 루프에서 FilteredData 대신 임시로 사용할 배열
        TArray<FPaintExecutionData> NextFilteredData;
        NextFilteredData.Reserve(FilteredData.Num() + 1);

        // 현재까지 살아남은 데이터(FilteredData)를 순회하며
        // 방금 가져온 NewData와 겹치는지 검사합니다.
        for (const FPaintExecutionData& ExistingData : FilteredData)
        {
            bool bIsOverlapping = false;

            //동일한 액터의 동일한 면이다
            if (NewData.UniqueID == ExistingData.UniqueID && NewData.FaceIndex == ExistingData.FaceIndex)
            {
                // 2. BrushLocation이 Limit 이내로 겹치는지 확인
                if (FVector2D::DistSquared(NewData.BrushLocation, ExistingData.BrushLocation) < LimitSquared)
                {
                    // 겹침 발견!
                    // "순서가 앞인 녀석"(ExistingData)을 제거해야 합니다.
                    // -> 즉, NextFilteredData에 ExistingData를 추가하지 않으면
                    //    자동으로 제거(필터링)됩니다.
                    bIsOverlapping = true;
                }
            }

            if (!bIsOverlapping)
            {
                // 이 ExistingData는 겹치지 않았으므로 다음 세대(NextFilteredData)로 살아남습니다.
                NextFilteredData.Add(ExistingData);
            }
        }

        // 3. 겹쳤던 녀석들을 모두 제거(Add 안 함)했으니,
        //    "순서가 뒤인" NewData를 최종적으로 추가합니다.
        NextFilteredData.Add(NewData);

        // 4. FilteredData를 NextFilteredData로 교체합니다.
        //    MoveTemp는 O(1)이며, 메모리 복사가 발생하지 않습니다.
        FilteredData = MoveTemp(NextFilteredData);
    }

    return FilteredData;	
}

#pragma endregion 

#pragma region 서브 시스템의 타 메서드에서 반복되는 로직을 재사용하기 위해 분리해놓은 메서드 시리즈

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
		//CreatedNormalRenderTarget->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		CreatedNormalRenderTarget->MipLoadOptions = ETextureMipLoadOptions::OnlyFirstMip;
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

#pragma endregion 