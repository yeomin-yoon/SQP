// Fill out your copyright notice in the Description page of Project Settings.


#include "CanvasActor.h"


// Sets default values
ACanvasActor::ACanvasActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACanvasActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACanvasActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

