// OrbitVehicleController.h
// ---------------
// UE5 C++ header for a futuristic spaceship flight controller.
// Handles thruster vectors, speed limits, and physics integration.
//
// Author: Principal UE5 C++ Engineer
// Date: 2026-10-04
// -------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OrbitVehicleController.generated.h"

/**
 *  A lightweight component that drives a spaceship using thruster vectors.
 *  The controller exposes a set of configurable parameters that can be tweaked
 *  in the editor or via C++ code.  It is intentionally lightweight so that
 *  it can be used on low‑end hardware or as a base for more complex
 *  flight systems.
 */
UCLASS(ClassGroup = (Orbit), meta = (BlueprintSpawnableComponent))
class ORBIT_API UOrbitVehicleController : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitVehicleController();

	/** Called every frame */
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Apply a thrust vector in local space.  The vector is interpreted as
	 *  a direction and magnitude.  The magnitude is clamped to 1.0f.
	 */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Flight")
	void ApplyLocalThrust(const FVector& LocalThrust);

	/** Apply a thrust vector in world space. */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Flight")
	void ApplyWorldThrust(const FVector& WorldThrust);

	/** Set the current velocity directly (e.g. for teleportation). */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Flight")
	void SetVelocity(const FVector& NewVelocity);

	/** Get the current velocity. */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Flight")
	FVector GetVelocity() const { return CurrentVelocity; }

	/** Set the current orientation directly (e.g. for instant rotation). */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Flight")
	void SetOrientation(const FRotator& NewOrientation);

	/** Get the current orientation. */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Flight")
	FRotator GetOrientation() const { return CurrentOrientation; }

	/** Reset the controller to its initial state. */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Flight")
	void Reset();

protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;

private:
	/** Current velocity in world space */
	FVector CurrentVelocity = FVector::ZeroVector;

	/** Current orientation (world space) */
	FRotator CurrentOrientation = FRotator::ZeroRotator;

	/** Accumulated thrust vector for this frame (world space) */
	FVector AccumulatedThrust = FVector::ZeroVector;

	/** Maximum speed the ship can reach (units per second) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Flight")
	float MaxSpeed = 3000.0f;

	/** Acceleration rate when thrust is applied (units per second squared) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Flight")
	float Acceleration = 1500.0f;

	/** Deceleration rate when no thrust is applied (units per second squared) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Flight")
	float Deceleration = 800.0f;

	/** Damping factor applied each frame to simulate space drag (0.0 = no drag) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Flight")
	float SpaceDrag = 0.01f;

	/** Helper to clamp velocity to MaxSpeed */
	void ClampSpeed();

	/** Helper to apply accumulated thrust and update velocity */
	void UpdatePhysics(float DeltaTime);

	/** Helper to apply space drag */
	void ApplySpaceDrag(float DeltaTime);
};
