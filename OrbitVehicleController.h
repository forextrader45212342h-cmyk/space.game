// OrbitVehicleController.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitVehicleController.generated.h"

/**
 *  OrbitVehicleController
 *  -----------------------
 *  A UE5 component that manages a futuristic spaceship's flight system.
 *  It handles multiple thruster vectors, acceleration, speed limits, and
 *  provides a simple API for applying thrust in arbitrary directions.
 *
 *  The component is ticked every frame and updates the owning actor's
 *  velocity accordingly.  It exposes properties that can be edited in
 *  the editor or overridden in Blueprints.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBIT_API UOrbitVehicleController : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Constructor */
	UOrbitVehicleController();

	/** Called every frame */
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Apply thrust in a given direction.  Amount is a scalar multiplier (0..1). */
	UFUNCTION(BlueprintCallable, Category="Orbit|Thrusters")
	void ApplyThrust(const FVector& Direction, float Amount = 1.0f);

	/** Set the maximum speed of the vehicle (units per second). */
	UFUNCTION(BlueprintCallable, Category="Orbit|Movement")
	void SetMaxSpeed(float NewMaxSpeed);

	/** Get the current speed of the vehicle. */
	UFUNCTION(BlueprintPure, Category="Orbit|Movement")
	float GetCurrentSpeed() const { return CurrentSpeed; }

	/** Called when the speed changes significantly (e.g. > 5% change). */
	UPROPERTY(BlueprintAssignable, Category="Orbit|Events")
	FOnSpeedChangedSignature OnSpeedChanged;

protected:
	/** Called when the component is initialized */
	virtual void BeginPlay() override;

private:
	/** Current velocity of the vehicle in world space */
	FVector CurrentVelocity = FVector::ZeroVector;

	/** Current speed magnitude (units per second) */
	float CurrentSpeed = 0.0f;

	/** Maximum allowed speed */
	UPROPERTY(EditAnywhere, Category="Orbit|Movement")
	float MaxSpeed = 3000.0f; // units/s

	/** Acceleration per second when full thrust is applied */
	UPROPERTY(EditAnywhere, Category="Orbit|Movement")
	float Acceleration = 1500.0f; // units/s^2

	/** Deceleration when no thrust is applied (drag) */
	UPROPERTY(EditAnywhere, Category="Orbit|Movement")
	float Deceleration = 800.0f; // units/s^2

	/** List of active thruster vectors relative to the actor */
	UPROPERTY(EditAnywhere, Category="Orbit|Thrusters")
	TArray<FVector> ThrusterVectors;

	/** Helper to clamp speed and update CurrentVelocity */
	void UpdateVelocity(float DeltaTime);

	/** Helper to broadcast speed change if needed */
	void BroadcastSpeedChange();

	/** Threshold for broadcasting speed changes (percentage of MaxSpeed) */
	UPROPERTY(EditDefaultsOnly, Category="Orbit|Events")
	float SpeedChangeThreshold = 0.05f; // 5%

	/** Last broadcasted speed for comparison */
	float LastBroadcastedSpeed = 0.0f;
};

/** Delegate for speed change events */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpeedChangedSignature, float, NewSpeed);
