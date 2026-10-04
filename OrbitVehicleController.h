// OrbitVehicleController.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitVehicleController.generated.h"

/**
 *  UOrbitVehicleController
 *  -----------------------
 *  A component that drives a futuristic spaceship using thruster vectors.
 *  It exposes a simple API for applying thrust in the local forward, right
 *  and up directions, and automatically integrates the resulting velocity
 *  and orientation each frame.
 *
 *  The component is intentionally lightweight – it does not depend on
 *  any external physics engine, but instead uses the built‑in
 *  FVector/FRotator math to update the owning actor.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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

	/** Apply a thrust vector in local space (normalized). */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Thrusters")
	void ApplyThrust(const FVector& LocalDirection, float Magnitude);

	/** Set the maximum speed the vehicle can reach. */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Thrusters")
	void SetMaxSpeed(float NewMaxSpeed);

	/** Get the current speed magnitude. */
	UFUNCTION(BlueprintPure, Category = "Orbit|Thrusters")
	float GetCurrentSpeed() const;

	/** Get the current velocity vector in world space. */
	UFUNCTION(BlueprintPure, Category = "Orbit|Thrusters")
	FVector GetVelocity() const;

	/** Get the current orientation of the vehicle. */
	UFUNCTION(BlueprintPure, Category = "Orbit|Thrusters")
	FRotator GetOrientation() const;

protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;

private:
	/** Current velocity in world space */
	FVector Velocity = FVector::ZeroVector;

	/** Current orientation (world space) */
	FRotator Orientation = FRotator::ZeroRotator;

	/** Maximum speed (units per second) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Thrusters")
	float MaxSpeed = 3000.f;

	/** Acceleration per second when full thrust is applied */
	UPROPERTY(EditAnywhere, Category = "Orbit|Thrusters")
	float MaxAcceleration = 2000.f;

	/** Drag coefficient (fraction of velocity lost per second) */
	UPROPERTY(EditAnywhere, Category = "Orbit|Thrusters")
	float DragCoefficient = 0.1f;

	/** Helper to clamp velocity to MaxSpeed */
	void ClampVelocity();

	/** Helper to apply drag */
	void ApplyDrag(float DeltaTime);
};
