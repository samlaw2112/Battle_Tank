// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Turret.generated.h"

/**
 Controls rotation of the turret
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTurret : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// -1 is max downward movement, 1 is max upward
	void Rotate(float RelativeSpeed);

private:
	// Maximum rotation speed
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxDegreesPerSecond = 15.f; // Sensible default
	
};
