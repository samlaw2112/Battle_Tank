// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 Handles mouse control of tank by the player. Determines if player is aiming towards a valid target and tells aiming component to aim there
 (Other player tank controls are handled in blueprint and passed to the tank's movement component)
 */

// Forward declaration
class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Functions called by delegates need to be a ufunction
	UFUNCTION()
	void OnTankDeath();

protected:
	// To provide reference to tank aiming component in blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimCompRef);

private:

	// Override SetPawn to subscribe to death event
	virtual void SetPawn(APawn* InPawn) override;

	// Start the tank moving the barrel so that a shot would hit where
	// the crosshair intersects the world
	void AimTowardsCrosshair();

	// Return an OUT parameter, true if hit landscape
	bool GetSightRayHitLocation(FVector& OUTHitLocation) const;

	// Return an OUT parameter, true if hit landscape
	bool GetLookVectorHitLocation(FVector WorldDirection, FVector& OUTHitLocation) const;

	// Position of crosshair on the screen (between 0-1)
	UPROPERTY(EditDefaultsOnly)
	float CrosshairLocationX = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float CrosshairLocationY = 0.33333f;

	// How far line tracing for player aiming extends
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.f; // 10km

	
};
