// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"
#include "Tank.h" // so we can implement OnDeath



void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}


void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto playerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto controlledTank = GetPawn();

	if (!ensure(playerTank && controlledTank))
		return;

	// Move towards the player
	MoveToActor(playerTank, AcceptanceRadius);

	auto AimingComponent = controlledTank->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(AimingComponent))
		return;

	// Aim towards the player
	AimingComponent->AimAt(playerTank->GetActorLocation());

	AimingComponent->Fire();
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);

		if (!ensure(PossessedTank))
			return;

		// Subscribe our local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
	}
}

void ATankAIController::OnTankDeath()
{
	if (!GetPawn())
		return;

	GetPawn()->DetachFromControllerPendingDestroy();
}