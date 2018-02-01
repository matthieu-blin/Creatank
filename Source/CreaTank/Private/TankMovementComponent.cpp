// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"




void UTankMovementComponent::Initialise(UTankTrack* _leftTrackToSet, UTankTrack* _rightTrackToSet)
{
	LeftTrack = _leftTrackToSet;
	RightTrack = _rightTrackToSet;
}


void UTankMovementComponent::MoveForward(float _axisValue)
{
	if (!ensure(LeftTrack && RightTrack))
		return;

	LeftTrack->SetThrottle(_axisValue);
	RightTrack->SetThrottle(_axisValue);
}

void UTankMovementComponent::TurnRight(float _axisValue)
{
	if (!ensure(LeftTrack && RightTrack))
		return;

	LeftTrack->SetThrottle(_axisValue);
	RightTrack->SetThrottle(-_axisValue);
}

void UTankMovementComponent::RequestDirectMove(const FVector& _MoveVelocity, bool _bForceMaxSpeed)
{
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntention = _MoveVelocity.GetSafeNormal();

	auto forwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	MoveForward(forwardThrow);

	auto rightThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;
	TurnRight(rightThrow);
}



