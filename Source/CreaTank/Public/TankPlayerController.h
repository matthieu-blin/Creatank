// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CREATANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* _aimingCompRef);

private:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Start the tank moving the barrel so that a shot would it wher the crosshair intersects the world
	void AimTowardsCrosshair();

	// Return an OUT parameter, true if hit landscape
	bool GetSightRayHitLocation(FVector& _outHitLocation) const;

	bool GetLookDirection(FVector2D _screenLocation, FVector& _outLookDirection) const;

	bool GetLookVectorHitLocation(FVector _lookDirection, FVector& _outHitLocation) const;

	UPROPERTY(EditAnywhere)
	float CrossHairXLocation = 0.5f;

	UPROPERTY(EditAnywhere)
	float CrosHairYLocation = 0.3333f;

	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000.f; //10km

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnTankDeath();
	
};
