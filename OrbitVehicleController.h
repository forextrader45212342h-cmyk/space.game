// OrbitVehicleController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OrbitVehicleController.generated.h"

/**
 *  A futuristic spaceship controller that uses a set of thruster vectors
 *  to compute the ship's velocity and orientation.  The controller
 *  exposes a simple API for applying thrust in any of the defined
 *  directions and automatically clamps the speed to a configurable
 *  maximum.
 */
UCLASS()
class ORBIT_API AOrbitVehicleController : public APawn
{
	GENERATED_BODY()

public:
	AOrbitVehicleController();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Setup player input bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Apply thrust in the specified direction index (0..ThrusterVectors.Num()-1) */
	UFUNCTION(BlueprintCallable, Category = "Thrusters")
	void ApplyThruster(int32 DirectionIndex, float ThrustMagnitude);

	/** Returns the current velocity of the ship */
	UFUNCTION(BlueprintPure, Category = "Thrusters")
	FVector GetVelocity() const { return CurrentVelocity; }

	/** Returns the current speed (magnitude of velocity) */
	UFUNCTION(BlueprintPure, Category = "Thrusters")
	float GetSpeed() const { return CurrentVelocity.Size(); }

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

private:
	/** Static mesh representing the ship */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* ShipMesh;

	/** Array of thruster direction vectors in local space */
	UPROPERTY(EditAnywhere, Category = "Thrusters")
	TArray<FVector> ThrusterVectors;

	/** Acceleration per unit thrust (m/s^2) */
	UPROPERTY(EditAnywhere, Category = "Thrusters")
	float AccelerationPerThrust = 200.0f;

	/** Maximum speed the ship can reach (m/s) */
	UPROPERTY(EditAnywhere, Category = "Thrusters")
	float MaxSpeed = 1200.0f;

	/** Current velocity in world space */
	FVector CurrentVelocity;

	/** Helper to clamp velocity to MaxSpeed */
	void ClampSpeed();

	/** Helper to convert local thruster vector to world space */
	FVector GetWorldThrusterVector(int32 Index) const;
};
