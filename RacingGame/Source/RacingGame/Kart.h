// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kart.generated.h"

UCLASS()
class RACINGGAME_API AKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
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

	void MoveForward(float value);
	void MoveRight(float value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(float value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float value);

	bool Server_MoveForward_Validate(float value);

	bool Server_MoveRight_Validate(float value);


	FVector GetAirResistance();
	FVector GetRollingResistance();
	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime);

	FVector Velocity;

	UPROPERTY(Replicated)
	FVector ReplicatedLocation;

	UPROPERTY(Replicated)
	FRotator ReplicatedRotation;

	float Throttle;
	float SteeringThrow;

};
