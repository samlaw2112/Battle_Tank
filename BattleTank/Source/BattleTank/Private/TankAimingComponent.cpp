// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "Turret.h"
#include "Projectile.h"

// Getters
int32 UTankAimingComponent::GetRoundsLeft() const {	return RoundsLeft; }
EFiringStatus UTankAimingComponent::GetFiringStatus() {	return FiringStatus; }


// Barrel and turret to set defined in Blueprint
void UTankAimingComponent::Initialise(UTankBarrel* BarrelToSet, UTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// So that first fire is after game load
	// This has the result of having to wait for reload time before can fire at start of game
	LastFireTime = FPlatformTime::Seconds();
}

// Check firing status every frame
void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// Cannot fire if no ammo left
	if (RoundsLeft <= 0)
	{
		FiringStatus = EFiringStatus::NoAmmo;
	}
	// Cannot fire if still reloading
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	// Can fire if barrel is reloaded and moving
	else if (IsBarrelMoving())
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	// Can fire if barrel is reloaded and aiming at target
	else
	{
		FiringStatus = EFiringStatus::Locked;
	}

}

// Move turret and barrel to aim towards target
void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) { return; }

	FVector OutLaunchVelocity; // Out parameter
	// Projectile will be launched from "Projectile" socket on barrel mesh
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	// Get required projectile velocity given projectile start location, target location and launch speed
	if (UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false, // Use low arc, not high arc
		0, // No collision radius
		0, // Don't override gravity
		ESuggestProjVelocityTraceOption::DoNotTrace
	))
	{
		// Get direction from launch velocity
		LaunchDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(LaunchDirection);
	}

	
}

// Aim barrel towards launch direction
void UTankAimingComponent::MoveBarrelTowards(FVector LaunchDirection)
{
	if (!ensure(Barrel && Turret)) { return; }

	// Work out difference between current barrel rotation and launch direction
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimRotation = LaunchDirection.Rotation();
	auto DeltaRotation = AimRotation - BarrelRotation;

	// Rotate the turret
	// Always yaw the shortest way
	if (FMath::Abs(DeltaRotation.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotation.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotation.Yaw);
	}

	// Elevate the barrel
	Barrel->Elevate(DeltaRotation.Pitch);
	
}

// Returns true if barrel is currently moving (meaning tank is aiming)
bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }

	// Compare current barrel direction against launch direction
	auto BarrelDirection = Barrel->GetForwardVector();
	// Use equals method with some tolerance, can't ask if two floats are precisely the same
	if (BarrelDirection.Equals(LaunchDirection, 0.01f))
	{
		return false;
	}
	else
	{
		return true;
	}
}

// Launches projectile
void UTankAimingComponent::Fire()
{
	// Can only fire if tanks is current aiming or locked on target
	if (FiringStatus == EFiringStatus::Locked || FiringStatus == EFiringStatus::Aiming)
	{
		// Spawn a projectile at the socket location
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBP)) { return; }
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBP,
			Barrel->GetSocketLocation("Projectile"),
			Barrel->GetSocketRotation("Projectile")
			);

		// Launch forwards with launch speed
		Projectile->LaunchProjectile(LaunchSpeed);
		// Log fire time for reload
		LastFireTime = FPlatformTime::Seconds();
		// Decrement ammo
		RoundsLeft--;
		}
	

}






