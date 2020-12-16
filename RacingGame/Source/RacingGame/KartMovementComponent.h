// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartMovementComponent.generated.h"

USTRUCT()
struct FKartMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float Throttle;

	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Time;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACINGGAME_API UKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKartMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SimulateMove(const FKartMove& Move);
	FKartMove CreateMove(float DeltaTime);

	FVector GetVelocity() { return Velocity; };
	void SetVelocity(FVector value) { Velocity = value; };
	void SetThrottle(float value) { Throttle = value; };
	void SetSteeringThrow(float value) { SteeringThrow = value; };

private:
	FVector GetAirResistance();
	FVector GetRollingResistance();
	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime, float SteeringThrow);
	//Mass of car in Kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	//Force pplied to the car when throttle is fully down N
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	//radius for turning in turning circle at full lock m
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	//Higher mean more drag
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	//Higher mean more rollign resistence
	UPROPERTY(EditAnywhere)
	float RollingResistenceCoefficient = 0.015;

	FVector Velocity;

	float Throttle;
	float SteeringThrow;

		
};
