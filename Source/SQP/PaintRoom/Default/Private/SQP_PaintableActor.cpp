// Fill out your copyright notice in the Description page of Project Settings.

#include "SQP_PaintableActor.h"

#include "HeadMountedDisplayTypes.h"
#include "PaintRoomSaveGame.h"
#include "SQP.h"
#include "SQPPaintWorldSubsystem.h"
#include "Kismet/KismetRenderingLibrary.h"

ASQP_PaintableActor::ASQP_PaintableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//아직 고유 ID가 없다면 새롭게 할당
	if (PersistantActorID.IsValid() == false)
	{
		PersistantActorID = FGuid::NewGuid();
	}
}

void ASQP_PaintableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASQP_PaintableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FGuid ASQP_PaintableActor::GetPersistantActorID()
{
	return PersistantActorID;
}

// FGuid ASQP_PaintableActor::GetPersistantActorID()
// {
// 	return PersistantActorID;
// }
//
// FPaintTextureData ASQP_PaintableActor::GetPaintData()
// {
// 	//페인트 월드 서브 시스템 획득
// 	const auto Subsystem = GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>();
//
// 	//반환값을 저장하기 위한 구조체
// 	FPaintTextureData PaintedTextureData;
//
// 	//스태틱 메시 컴포넌트 검색
// 	if (const auto MeshComp = FindComponentByClass<UStaticMeshComponent>())
// 	{
// 		//해당 메시 컴포넌트의 모든 머터리얼을 순회
// 		for (const auto MaterialInterface : MeshComp->GetMaterials())
// 		{
// 			//캔버스 머터리얼 형태인지 확인
// 			if (Subsystem->CheckCanvasMaterialBase(MaterialInterface) == false)
// 			{
// 				continue;
// 			}
//
// 			//머터리얼에서 텍스처를 획득
// 			UTexture* Texture = nullptr;
// 			MaterialInterface->GetTextureParameterValue(FName(TEXT("ColorRenderTarget")), Texture);
// 			if (const auto RenderTargetTexture = Cast<UTextureRenderTarget2D>(Texture))
// 			{
// 				PRINTLOGNET(TEXT("페인트 텍스처 구조체 유효!"));
// 				
// 				TArray<uint8> RawData;
// 				UKismetRenderingLibrary::ReadRenderTargetRaw(this, RenderTargetTexture, RawData);
// 				PaintedTextureData.PersistantUniqueID = PersistantActorID;
// 				PaintedTextureData.Pixels = TextureColors;
// 				PaintedTextureData.TextureWidth = 1024;
// 				PaintedTextureData.TextureHeight = 1024;
//
// 				//유효한 구조체가 반환된다
// 				return PaintedTextureData;
// 			}
// 		}
// 	}
//
// 	PRINTLOGNET(TEXT("페인트 텍스처 구조체 에러!"));
//
// 	//모든 시도가 실패하면 빈 구조체가 반환된다
// 	return PaintedTextureData;
// }
//
// void ASQP_PaintableActor::LoadPaintData(const FPaintTextureData& PaintTextureData)
// {
// 	//페인트 월드 서브 시스템 획득
// 	const auto Subsystem = GetWorld()->GetSubsystem<USQPPaintWorldSubsystem>();
// 	
// 	//스태틱 메시 컴포넌트 검색
// 	if (const auto MeshComp = FindComponentByClass<UStaticMeshComponent>())
// 	{
// 		//메시 컴포넌트에 꽂아넣을 스플래툰 캔버스 MID를 생성한다
// 		UMaterialInstanceDynamic* CreatedMaterialInstanceDynamic;
// 		UTextureRenderTarget2D* OutColorRenderTarget;
// 		UTextureRenderTarget2D* OutNormalRenderTarget;
// 		Subsystem->CreateCanvasMaterialInstanceDynamic(CreatedMaterialInstanceDynamic, OutColorRenderTarget, OutNormalRenderTarget);
//
// 		//데이터로부터 텍스처를 재구축한다
// 		UTexture2D* Texture = Subsystem->CreateTextureFromPixelData(PaintTextureData.TextureWidth, PaintTextureData.TextureHeight, PaintTextureData.Pixels);
//
// 		//재구축한 텍스처를 MID 패러미터에 전달한다
// 		CreatedMaterialInstanceDynamic->SetTextureParameterValue(FName("ColorRenderTarget"), Texture);
//
// 		//저장 데이터를 적용한 MID를 적용
// 		MeshComp->SetMaterial(0, CreatedMaterialInstanceDynamic);
//
// 		PRINTLOGNET(TEXT("%s has been Complete Loading!"), *GetActorNameOrLabel());
// 	}
// }