// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
class ATank;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	

private:
	ATank* GetControlledTank() const;

	// Start the tank moving the barrel so that a shot would hit where
	// the crosshair intersects the world
	void AimTowardsCrosshair();

	// Return an OUT parameter, true if hit landscape
	bool GetSightRayHitLocation(FVector& OUTHitLocation) const;

	// Return an OUT parameter, true if hit landscape
	bool GetLookVectorHitLocation(FVector WorldDirection, FVector& OUTHitLocation) const;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairLocationX = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float CrosshairLocationY = 0.33333f;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.f; // 10km
};
