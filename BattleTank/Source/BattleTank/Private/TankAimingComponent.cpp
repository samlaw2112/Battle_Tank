// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "Turret.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Barrel) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	if (UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	))
	{
		auto LaunchDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(LaunchDirection);
	}

	
}

void UTankAimingComponent::MoveBarrelTowards(FVector LaunchDirection)
{
	// Work out difference between current barrel rotation and aim direction
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimRotation = LaunchDirection.Rotation();
	auto DeltaRotation = AimRotation - BarrelRotation;
	Barrel->Elevate(DeltaRotation.Pitch);
	Turret->Rotate(DeltaRotation.Yaw);
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTurret * TurretToSet)
{
	Turret = TurretToSet;
}



