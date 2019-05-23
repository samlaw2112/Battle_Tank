// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * Used to set maximum driving force and apply force to the tank
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()

public:

	// Sets a throttle between -1 and 1
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);

	// Max force per track in newtons
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float TrackMaxDrivingForce = 400000; // Sensible default assuming 40 tonne tank and 1g acceleration (10ms^-2)

private:

	UTankTrack();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
	void ApplySidewaysForce();

	void DriveTrack();

	float CurrentThrottle = 0;
};
