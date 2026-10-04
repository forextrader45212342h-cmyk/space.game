// OrbitVehicleController.h
// ---------------
// UE5 C++ header for a futuristic spaceship flight controller.
// Handles thruster vectors, acceleration, and speed limits.
//
// Author: Principal UE5 C++ Engineer
// Date:   2026-10-04
// -------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitVehicleController.generated.h"

/**
 *  Struct that represents a single thruster on the ship.
 *  - Direction is always in local space.
 *  - Throttle ranges from 0.0 to 1.0.
 *  - MaxForce is the maximum force this thruster can apply.
 */
USTRUCT(BlueprintType)
struct FOrbitThruster
{
	GENERATED_BODY()

public:
	/** Local direction of the thrust vector (normalized). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thruster")
	FVector Direction = FVector::ForwardVector;

	/** Current throttle value (0.0 – 1.0). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thruster")
	float Throttle = 0.0f;

	/** Maximum force (in Newtons) that this thruster can produce. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thruster")
	float MaxForce = 1000.0f;

	/** Current force being applied (computed each tick). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Thruster")
	float CurrentForce = 0.0f;

	/** Helper to compute the force vector for this thruster. */
	FORCEINLINE FVector GetForceVector() const
	{
		return Direction * CurrentForce;
	}
};

/**
 *  Component that controls a spaceship's flight using multiple thrusters.
 *  It handles acceleration, speed limiting, and basic physics integration
 *  (using the owning actor's RootComponent as the physics body).
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORBIT_API UOrbitVehicleController : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Default constructor. */
	UOrbitVehicleController();

	/** Called every frame. */
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Apply a throttle value to a specific thruster by index. */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Thrusters")
	void SetThrusterThrottle(int32 ThrusterIndex, float Throttle);

	/** Add a new thruster to the ship. */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Thrusters")
	void AddThruster(const FOrbitThruster& Thruster);

	/** Clear all thrusters. */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Thrusters")
	void ResetThrusters();

	/** Get the current linear velocity of the ship in world space. */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Movement")
	FVector GetVelocity() const;

	/** Get the current speed (magnitude of velocity). */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Movement")
	float GetSpeed() const;

protected:
	/** Called when the game starts. */
	virtual void BeginPlay() override;

private:
	/** All thrusters attached to this ship. */
	UPROPERTY(EditAnywhere, Category = "Orbit|Thrusters")
	TArray<FOrbitThruster> Thrusters;

	/** Maximum allowed speed (m/s). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Movement", meta = (ClampMin = "0.0"))
	float MaxSpeed = 3000.0f;

	/** Maximum acceleration (m/s^2) when all thrusters are at full throttle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orbit|Movement", meta = (ClampMin = "0.0"))
	float MaxAcceleration = 500.0f;

	/** Current velocity of the ship (world space). */
	FVector CurrentVelocity = FVector::ZeroVector;

	/** Helper to compute the net thrust force from all thrusters. */
	FVector ComputeNetThrust() const;

	/** Helper to clamp velocity to MaxSpeed. */
	void ClampVelocity();

	/** Reference to the physics body (RootComponent). */
	UPrimitiveComponent* PhysicsBody = nullptr;
};
