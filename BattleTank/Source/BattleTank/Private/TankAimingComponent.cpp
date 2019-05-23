// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "Turret.h"
#include "Projectile.h"

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
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	else
	{
		FiringStatus = EFiringStatus::Locked;
	}

	// TODO handle aiming and locked states
}


void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) { return; }

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
		LaunchDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(LaunchDirection);
	}

	
}

void UTankAimingComponent::MoveBarrelTowards(FVector LaunchDirection)
{
	if (!ensure(Barrel && Turret)) { return; }

	// Work out difference between current barrel rotation and aim direction
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimRotation = LaunchDirection.Rotation();
	auto DeltaRotation = AimRotation - BarrelRotation;
	Barrel->Elevate(DeltaRotation.Pitch);

	// Always yaw the shortest way
	if (FMath::Abs(DeltaRotation.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotation.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotation.Yaw);
	}
	
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }

	auto BarrelDirection = Barrel->GetForwardVector();
	if (BarrelDirection.Equals(LaunchDirection, 0.01f))
	{
		return false;
	}
	else
	{
		return true;
	}
}

void UTankAimingComponent::Fire()
{
	
	if (FiringStatus != EFiringStatus::Reloading)
	{
		// Spawn a projectile at the socket location
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBP)) { return; }
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBP,
			Barrel->GetSocketLocation("Projectile"),
			Barrel->GetSocketRotation("Projectile")
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
	

}

EFiringStatus UTankAimingComponent::GetFiringStatus()
{
	return FiringStatus;
}




