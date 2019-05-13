// Fill out your copyright notice in the Description page of Project Settings.

#include "Turret.h"

void UTurret::Rotate(float RelativeSpeed)
{
	// RelativeSpeed should be between -1 and 1
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.f, 1.f);
	// Rotate turret to face launch direction
	// Move the right amount this frame given max rotation speed and frame time
	auto RotationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto NewRotation = RelativeRotation.Yaw + RotationChange;

	SetRelativeRotation(FRotator(0.f, NewRotation, 0.f));


}