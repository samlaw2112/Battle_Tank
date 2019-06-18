// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
	// subscribe to on hit event
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

// Called when tank track hits the ground
void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	// Drive track when on ground using current throttle
	DriveTrack();
	// Apply sideways force to mimic friction
	ApplySidewaysForce();
	// Reset throttle so that it doesn't keep building when key held down
	CurrentThrottle = 0;
}

// Apply sideways force to mimic friction
void UTankTrack::ApplySidewaysForce()
{
	// Calculate the slippage speed (Amount we are currently moving to the side)
	auto SlippageSpeed = FVector::DotProduct(GetComponentVelocity(), GetRightVector());

	// Work out the require acceleration this frame to correct (equal and opposite force for this frame)
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();

	// Caclulate and apply sideways force (F = m*a)
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; // Divide by 2 because 2 tracks
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle)
{
	// Between -1 and 1
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
}

// Apply force to track
void UTankTrack::DriveTrack()
{
	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}