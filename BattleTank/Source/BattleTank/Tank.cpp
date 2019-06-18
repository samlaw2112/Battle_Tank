// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	// Start with full health
	Health = StartHealth;
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// Convert to int
	int32 DamagePoints = FMath::RoundToInt(DamageAmount);

	// Clamp between 0 and current health
	int32 DamageToApply = FMath::Clamp<float>(DamagePoints, 0, Health);

	// Apply
	Health -= DamageToApply;

	// If run out of health broadcast death event
	if (Health <= 0)
	{
		OnDeath.Broadcast();
	}

	return DamageToApply;
}

// Return current health as % of max health
float ATank::GetPercentHealth() const
{
	return (float)Health / (float)StartHealth;
}


