// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Engine/World.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (RoundsLeft <= 0)
		FiringState = EFiringState::OutOfAmmo;
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
		FiringState = EFiringState::Reloading;
	else if (IsBarrelMoving())
		FiringState = EFiringState::Aiming;
	else
		FiringState = EFiringState::Locked;
}

void UTankAimingComponent::Initialise(UTankBarrel* _barrelToSet, UTankTurret* _turretToSet)
{
	Barrel = _barrelToSet;
	Turret = _turretToSet;
}

void UTankAimingComponent::AimAt(FVector _hitLocation)
{
	if (!ensure(Barrel))
		return;

	FVector outLaunchVelocity;
	FVector startLocation = Barrel->GetSocketLocation(FName("ProjectileLocation"));
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		outLaunchVelocity,
		startLocation,
		_hitLocation,
		LaunchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (bHaveAimSolution) // Calculate the out launch velocity	
	{
		AimDirection = outLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		RotateTurret(AimDirection);
	}

	// if no solution found do nothing
}

void UTankAimingComponent::MoveBarrelTowards(FVector _aimDirection)
{
	if (!ensure(Barrel))
		return;

	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = _aimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
}

void UTankAimingComponent::RotateTurret(FVector _aimDirection)
{
	if (!ensure(Turret))
		return;

	auto TurretRotator = Turret->GetForwardVector().Rotation();
	auto AimAsRotator = _aimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - TurretRotator;

	// Always yaw the shortest way
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
		Turret->Rotate(DeltaRotator.Yaw);
	else
		Turret->Rotate(-DeltaRotator.Yaw);
}

void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming)
	{
		if (!ensure(Barrel))
			return;

		if (!ensure(ProjectileBlueprint))
			return;

		//This if statement is now applied in TickComponent method
		//if (FPlatformTime::Seconds() - LastFireTime > ReloadTimeInSeconds)
		//{
		// Spawn projectile at the socket location on the barrel
		auto projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, Barrel->GetSocketLocation("ProjectileLocation"), Barrel->GetSocketRotation("ProjectileLocation"));
		projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
		//}
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel))
		return false;

	auto BarrelForward = Barrel->GetForwardVector();

	return !BarrelForward.Equals(AimDirection, 0.01f);
}


