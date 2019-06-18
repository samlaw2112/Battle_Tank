// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

/**
 Handles moving turret and barrel to aim towards a target and firing of projectiles
 */

// Enum for aiming state
UENUM()
enum class EFiringStatus : uint8
{
	Reloading,
	Aiming,
	Locked,
	NoAmmo
};

// Forward declaration
class UTankBarrel; 
class UTurret;
class AProjectile;

// Holds barrel properties and elevate method
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	// Moves turret + barrel towards player
	void AimAt(FVector HitLocation);

	// Initalises barrel and turret
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrel* BarrelToSet, UTurret* TurretToSet);

	// Called on mouse click (from Blueprint)
	UFUNCTION(BlueprintCallable)
	void Fire();

	// Getters
	EFiringStatus GetFiringStatus();

	UFUNCTION(BlueprintCallable)
	int32 GetRoundsLeft() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus FiringStatus = EFiringStatus::Aiming;

private:
	

	void MoveBarrelTowards(FVector LaunchDirection);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void BeginPlay() override;

	bool IsBarrelMoving();

	UTankBarrel* Barrel = nullptr;
	UTurret* Turret = nullptr;

	// Projectile launch speed
	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 4000; // Sensible default

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 2.f; // Sensible default

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	int32 RoundsLeft = 10; // Default starting number of rounds

	// Hooked up to projectile BP in blueprint
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBP;

	// keeps track of when gun was last fired
	double LastFireTime = 0;

	// Direction to launch projectile
	FVector LaunchDirection;

	

		
};
