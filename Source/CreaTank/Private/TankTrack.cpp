// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"




UTankTrack::UTankTrack()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay()
{
	Super::BeginPlay();
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::SetThrottle(float _throttle)
{
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + _throttle, -1.0f, 1.0f);

	// deprecated : 1st system, OnHit to handle control + fly
	//DriveTrack();
	//CurrentThrottle = 0.0f;
}

void UTankTrack::DriveTrack()
{
	auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpule, const FHitResult& Hit)
{
	// Drive the track
	DriveTrack();

	// Apply sideways force
	ApplySidewaysForce();

	// reset throttle
	CurrentThrottle = 0.0f;
}

void UTankTrack::ApplySidewaysForce()
{
	// Work out the required acceleration this frame to correct
	auto slippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	auto correctionAcceleration = -slippageSpeed / DeltaTime * GetRightVector();

	// Calculate and apply sideways force (F = m a)
	auto tankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto correctionForce = (tankRoot->GetMass() * correctionAcceleration) / 2.0f; // / 2 because there are 2 tracks
	tankRoot->AddForce(correctionForce);
}