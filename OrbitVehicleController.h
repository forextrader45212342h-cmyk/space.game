// OrbitVehicleController.h
// ---------------
// UE5 C++ component that drives a futuristic spaceship using a set of
// thruster vectors.  The component exposes a simple API for applying
// thrust, yaw, pitch and roll, and keeps track of the current speed
// and velocity.  It is intended to be attached to an APawn or
// AActor that represents the ship.
//
//  Author: Principal UE5 C++ Engineer
//  Date:   2026‑10‑04
// -----------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitVehicleController.generated.h"

/**
 *  UOrbitVehicleController
 *
 *  A reusable component that manages the physics of a spaceship.
 *  It exposes thruster vectors (forward, right, up) and allows
 *  the user to apply throttle, yaw, pitch and roll.  The component
 *  automatically updates the ship's velocity and applies a simple
 *  drag model.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBIT_API UOrbitVehicleController : public UActorComponent
{
	GENERATED_BODY()

public:
	// ------------------------------------------------------------------
	// Construction / Lifecycle
	// ------------------------------------------------------------------
	UOrbitVehicleController();

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Called every frame */
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Request a shutdown of the controller (e.g. when the ship is destroyed) */
	void RequestShutdown();

	// ------------------------------------------------------------------
	// Public API – Input
	// ------------------------------------------------------------------
	/** Apply forward/backward throttle (-1.0 .. 1.0) */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Input")
	void ApplyThrottle(float Throttle);

	/** Apply yaw input (-1.0 .. 1.0) */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Input")
	void ApplyYaw(float YawInput);

	/** Apply pitch input (-1.0 .. 1.0) */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Input")
	void ApplyPitch(float PitchInput);

	/** Apply roll input (-1.0 .. 1.0) */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Input")
	void ApplyRoll(float RollInput);

	// ------------------------------------------------------------------
	// Public API – Query
	// ------------------------------------------------------------------
	/** Current speed in units per second */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Query")
	float GetCurrentSpeed() const { return CurrentSpeed; }

	/** Current velocity vector in world space */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Query")
	FVector GetVelocity() const { return Velocity; }

	/** Current acceleration vector in world space */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Query")
	FVector GetAcceleration() const { return Acceleration; }

	/** Current orientation of the ship */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Query")
	FRotator GetOrientation() const { return GetOwner()->GetActorRotation(); }

private:
	// ------------------------------------------------------------------
	// Internal state
	// ------------------------------------------------------------------
	/** Forward thruster vector (local space) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Thrusters")
	FVector ThrusterForward = FVector::ForwardVector;

	/** Right thruster vector (local space) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Thrusters")
	FVector ThrusterRight = FVector::RightVector;

	/** Up thruster vector (local space) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Thrusters")
	FVector ThrusterUp = FVector::UpVector;

	/** Maximum speed (units per second) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Physics")
	float MaxSpeed = 5000.0f;

	/** Maximum acceleration (units per second^2) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Physics")
	float MaxAcceleration = 2000.0f;

	/** Drag coefficient (0 = no drag, 1 = full drag) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Physics")
	float DragCoefficient = 0.05f;

	/** Current velocity in world space */
	FVector Velocity = FVector::ZeroVector;

	/** Current acceleration in world space */
	FVector Acceleration = FVector::ZeroVector;

	/** Current speed (magnitude of Velocity) */
	float CurrentSpeed = 0.0f;

	/** Current throttle value (-1 .. 1) */
	float CurrentThrottle = 0.0f;

	/** Current yaw input (-1 .. 1) */
	float CurrentYaw = 0.0f;

	/** Current pitch input (-1 .. 1) */
	float CurrentPitch = 0.0f;

	/** Current roll input (-1 .. 1) */
	float CurrentRoll = 0.0f;

	/** Helper to clamp a value between -1 and 1 */
	float ClampInput(float Value) const { return FMath::Clamp(Value, -1.0f, 1.0f); }

	/** Apply physics integration for the current frame */
	void ApplyPhysics(float DeltaTime);

	/** Apply rotation based on yaw/pitch/roll inputs */
	void ApplyRotation(float DeltaTime);
};
