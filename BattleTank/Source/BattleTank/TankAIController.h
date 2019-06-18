// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 Controls AI tanks, passes instructions for movement and aiming towards player to movement and aiming components
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	// Override SetPawn to subscribe to death event
	virtual void SetPawn(APawn* InPawn) override;

protected:

	// How close AI tank can get to player
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float AcceptanceRadius = 8000.f; // default

public:
	// Functions called by delegates need to be a ufunction
	UFUNCTION()
	void OnTankDeath();
};
