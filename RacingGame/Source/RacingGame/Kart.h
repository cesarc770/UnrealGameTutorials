// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kart.generated.h"

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

USTRUCT()
struct FKartState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FKartMove LastMove;
};

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
	void Server_SendMove(FKartMove Move);

	bool Server_SendMove_Validate(FKartMove Move);

	void SimulateMove(const FKartMove& Move);
	FKartMove CreateMove(float DeltaTime);
	void ClearAcknowledgedMoves(FKartMove LastMove);
	FVector GetAirResistance();
	FVector GetRollingResistance();
	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime, float SteeringThrow);

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FKartState ServerState;

	UFUNCTION()
	void OnRep_ServerState();


	FVector Velocity;

	float Throttle;
	float SteeringThrow;

	TArray<FKartMove> UnacknowledgeMoves;

};
