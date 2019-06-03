// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay() 
{
	Super::BeginPlay();
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (ensure(AimingComponent))
	{
		FoundAimingComponent(AimingComponent);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("Player controlled can't find aiming component at being play")); }

}

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// subscribe to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnTankDeath);
	}
}

void ATankPlayerController::OnTankDeath()
{
	StartSpectatingOnly();
}

// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; } // e.g. If not possessing
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	FVector HitLocation; // OUT parameter

	// if it hits the landscape
	if (GetSightRayHitLocation(HitLocation)) // Has side effect of raytracing
	{
		AimingComponent->AimAt(HitLocation);
	}
}

// Get world location from line trace through crosshair, return true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OUTHitLocation) const
{
	
	// Find crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairLocationX, ViewportSizeY * CrosshairLocationY);

	// De-project screen pos of crosshair to world direction
	FVector CameraWorldLocation, WorldDirection;
	DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, WorldDirection);

	// Line trace along that direction, return location if hit landscape
	if (GetLookVectorHitLocation(WorldDirection, OUTHitLocation)) {
		return true;
	} else { return false; }
}

// Ray trace along world direction and return hit location if hits landscape
bool ATankPlayerController::GetLookVectorHitLocation(FVector WorldDirection, FVector & OUTHitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (WorldDirection * LineTraceRange);
	FCollisionQueryParams Params(NAME_None, false, GetPawn());
	bool HitLandscape = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Camera, Params);
	if (HitLandscape)
	{
		OUTHitLocation = HitResult.Location;
		return true;
	}
	else { OUTHitLocation = FVector(0); return false; }
}
