// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// -1 is max downward movement, 1 is max upward
	void Elevate(float RelativeSpeed);

private:
	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxDegreesPerSecond = 10.f; // Sensible default

	UPROPERTY(EditAnywhere, Category = Setup)
	float MinElevation = 0.f; // Sensible default

	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxElevation = 40.f; // Sensible default
	
};
