// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"

// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("AI tank ticking"));
	auto MyTank = GetPawn();;
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(MyTank && PlayerTank)) { return; }
		
	// Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in cm
	UE_LOG(LogTemp, Warning, TEXT("AI tank moving"));

	// Aim towards the player
	auto AimingComponent = MyTank->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	UE_LOG(LogTemp, Warning, TEXT("AI tank aiming at: %s"), *PlayerTank->GetActorLocation().ToString())

	// TODO fix firing
	//MyTank->Fire();
}
