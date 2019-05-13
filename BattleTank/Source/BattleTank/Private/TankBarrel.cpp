// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "Math/UnrealMathUtility.h"

void UTankBarrel::Elevate(float RelativeSpeed) {
	// RelativeSpeed should be between -1 and 1
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.f, 1.f);
	// Rotate barrel to face launch direction
	// Move the right amount this frame given max elevation speed and frame time
	
	//auto Time = GetWorld()->TimeSeconds;
	//UE_LOG(LogTemp, Warning, TEXT("%f: Elevating barrel"), Time);

	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	auto NewElevation = FMath::Clamp<float>(RawNewElevation, MinElevation, MaxElevation);

	SetRelativeRotation(FRotator(NewElevation, 0.f, 0.f));


}