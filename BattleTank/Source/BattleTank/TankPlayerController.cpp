// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay() 
{
	Super::BeginPlay();

}

// Called every frame
void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation; // OUT parameter

	// if it hits the landscape
	if (GetSightRayHitLocation(HitLocation)) // Has side effect of raytracing
	{
		GetControlledTank()->AimAt(HitLocation);
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
	bool HitLandscape = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, Params);
	if (HitLandscape)
	{
		OUTHitLocation = HitResult.Location;
		return true;
	}
	else { OUTHitLocation = FVector(0); return false; }
}

ATank* ATankPlayerController::GetControlledTank() const
{
	
	return Cast<ATank>(GetPawn());
}
