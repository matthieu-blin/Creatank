// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h" // so we can implement OnDeath


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!GetPawn())
		return;
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(AimingComponent))
		return;

	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn())
		return;

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(AimingComponent))
		return;

	FVector HitLocation; //out parameter
	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);
	if (bGotHitLocation)
	{
		AimingComponent->AimAt(HitLocation);
	}
}

// Get a world location of linetrace through crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& _outHitLocation) const
{
	// Find the crosshair position in pixel coordinates
	int32 viewportSizeX, viewportSizeY;
	GetViewportSize(viewportSizeX, viewportSizeY);

	FVector2D ScreenLocation = FVector2D(viewportSizeX * CrossHairXLocation, viewportSizeY * CrosHairYLocation);
	FVector LookDirection;

	// de-project the screen position of the crosshair to a world direction
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		// line-trace along the lookdirection, and see what we hit (up to max range)	
		return GetLookVectorHitLocation(LookDirection, _outHitLocation);
	}

	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D _screenLocation, FVector& _outLookDirection) const
{
	FVector worldLocation;
	return DeprojectScreenPositionToWorld(_screenLocation.X, _screenLocation.Y, worldLocation, _outLookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector _lookDirection, FVector& _outHitLocation) const
{
	FHitResult hitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + _lookDirection * LineTraceRange;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Camera))
	{
		_outHitLocation = hitResult.Location;
		return true;
	}

	_outHitLocation = FVector(0.0f);
	return false;
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);

		if (!ensure(PossessedTank))
			return;

		// Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnTankDeath);
	}
}

void ATankPlayerController::OnTankDeath()
{
	StartSpectatingOnly(); //The player is now just a spectator of the game
}



