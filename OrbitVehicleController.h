// OrbitVehicleController.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitVehicleController.generated.h"

/**
 *  UOrbitVehicleController
 *  -----------------------
 *  A lightweight component that drives a futuristic spaceship using
 *  thruster vectors.  It exposes a simple API for applying thrust,
 *  clamping speed, and querying the current velocity.  All values
 *  are exposed to the editor and Blueprint for rapid iteration.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ORBIT_API UOrbitVehicleController : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Default constructor */
	UOrbitVehicleController();

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Called every frame */
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Apply a thrust impulse in local space.
	 *
	 *  @param Direction  Local direction of the thrust (e.g. +X for forward).
	 *  @param Magnitude  Thrust magnitude in units per second squared.
	 */
	UFUNCTION(BlueprintCallable, Category="Orbit|Thrusters")
	void ApplyThrust(const FVector& Direction, float Magnitude);

	/** Set the maximum speed the vehicle can reach. */
	UFUNCTION(BlueprintCallable, Category="Orbit|Thrusters")
	void SetMaxSpeed(float NewMaxSpeed);

	/** Get the current speed (magnitude of velocity). */
	UFUNCTION(BlueprintPure, Category="Orbit|Thrusters")
	float GetCurrentSpeed() const;

	/** Get the full velocity vector in world space. */
	UFUNCTION(BlueprintPure, Category="Orbit|Thrusters")
	FVector GetVelocity() const;

	/** Set the orientation of the thruster relative to the owning actor. */
	UFUNCTION(BlueprintCallable, Category="Orbit|Thrusters")
	void SetThrusterOrientation(const FRotator& NewOrientation);

	/** Get the current thruster orientation. */
	UFUNCTION(BlueprintPure, Category="Orbit|Thrusters")
	FRotator GetThrusterOrientation() const;

protected:
	/** Maximum speed (units per second). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Orbit|Thrusters")
	float MaxSpeed = 2000.f;

	/** Acceleration when thrust is applied (units per second squared). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Orbit|Thrusters")
	float Acceleration = 500.f;

	/** Deceleration applied when no thrust is active (units per second squared). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Orbit|Thrusters")
	float Deceleration = 300.f;

	/** Current velocity in world space. */
	FVector CurrentVelocity = FVector::ZeroVector;

	/** Thruster orientation relative to the owning actor. */
	FRotator ThrusterOrientation = FRotator::ZeroRotator;

	/** Clamp the velocity to the configured MaxSpeed. */
	void ClampSpeed();
};
