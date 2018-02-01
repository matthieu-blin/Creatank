// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Engine/World.h"

void UTankTurret::Rotate(float _relativeSpeed)
{
	_relativeSpeed = FMath::Clamp<float>(_relativeSpeed, -1, 1);
	float rotationChange = _relativeSpeed * MaxDegreesPerSecond * GetWorld()->GetDeltaSeconds();
	auto Rotation = RelativeRotation.Yaw + rotationChange;

	SetRelativeRotation(FRotator(0, Rotation, 0));
}

