// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

// Left and right track to set defined in blueprint
void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

// Move tank forwards
void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}

// Turn tank
void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }
	// Apply throttle to left track an -throttle to right track to turn right
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);

}

// Called form path finding by ai tanks
void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	// Get forward direction
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	// Direction AI wants to move
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();
	// Go forwards faster if tank is facing desired movement direction
	auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	
	// Move tank forwards
	IntendMoveForward(ForwardThrow);

	// Turn tank faster if facing away from desired movement direction
	auto RightThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;
	IntendTurnRight(RightThrow);

}