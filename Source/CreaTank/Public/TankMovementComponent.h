// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"


class UTankTrack;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CREATANK_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	void MoveForward(float _axisValue);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void TurnRight(float _axisValue);

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankTrack* _leftTrackToSet, UTankTrack* _rightTrackToSet);


private:
	UTankTrack* LeftTrack = nullptr;
	UTankTrack* RightTrack = nullptr;

	// Called from the pathfinding logic by the AI controllers
	virtual void RequestDirectMove(const FVector& _MoveVelocity, bool _bForceMaxSpeed) override;
	
};
