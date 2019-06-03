// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "Public/Delegates/DelegateSignatureImpl.inl"

// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// subscribe to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
	}
}

void ATankAIController::OnTankDeath()
{
	GetPawn()->DetachFromControllerPendingDestroy();
}

// Called every frame
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (ensure(AimingComponent && PlayerTank))
	{
		// Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in cm

		// Aim towards the player
		AimingComponent->AimAt(PlayerTank->GetActorLocation());

		// If locked
		if (AimingComponent->GetFiringStatus() == EFiringStatus::Locked)
		{
			AimingComponent->Fire();
		}
	}
}
