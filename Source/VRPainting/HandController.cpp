// Fill out your copyright notice in the Description page of Project Settings.


#include "HandController.h"
#include "Engine/World.h"

// Sets default values
AHandController::AHandController()
{
	PrimaryActorTick.bCanEverTick = true;
	MotionController = CreateDefaultSubobject <UMotionControllerComponent>(TEXT("MotionController"));
	SetRootComponent(MotionController);
	MotionController->SetTrackingSource(EControllerHand::Right);
	MotionController->SetShowDeviceModel(true);
}

void AHandController::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHandController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentStroke) 
	{

		CurrentStroke->Update(GetActorLocation());
	}
}

void AHandController::TriggerPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Trigger Press Check"));
	CurrentStroke = GetWorld()->SpawnActor<AStroke>(StrokeClass);
	CurrentStroke->SetActorLocation(GetActorLocation());

}

void AHandController::TriggerReleased()
{
	CurrentStroke = nullptr;
}