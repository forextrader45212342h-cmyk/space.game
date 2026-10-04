// OrbitVehicleController.h
// 2026-10-04
// UE5 C++ header for a futuristic spaceship flight controller
// Handles thruster vectors, acceleration, and speed clamping.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitVehicleController.generated.h"

/**
 *  Struct that defines a single thruster.
 *  - Direction is relative to the ship's local space.
 *  - MaxThrust is the maximum force (in Newtons) this thruster can apply.
 *  - bIsActive indicates whether the thruster is currently firing.
 */
USTRUCT(BlueprintType)
struct FThrusterConfig
{
	GENERATED_BODY()

	/** Local space direction of the thruster */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thruster")
	FVector Direction = FVector::ForwardVector;

	/** Maximum thrust force (N) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thruster")
	float MaxThrust = 1000.f;

	/** Is this thruster currently active? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thruster")
	bool bIsActive = false;
};

/**
 *  Component that controls a spaceship's flight using thrusters.
 *  It calculates the net thrust vector, applies it to the physics body,
 *  and clamps the ship's speed to a configurable maximum.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORBIT_API UOrbitVehicleController : public UActorComponent
{
	GENERATED_BODY()

public:
	/** Constructor */
	UOrbitVehicleController();

	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Called every frame */
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Apply thrust in a given local direction with a specified magnitude (0-1). */
	UFUNCTION(BlueprintCallable, Category = "Flight")
	void ApplyThrust(const FVector& LocalDirection, float Magnitude);

	/** Activate or deactivate a specific thruster by index. */
	UFUNCTION(BlueprintCallable, Category = "Thruster")
	void SetThrusterActive(int32 ThrusterIndex, bool bActive);

	/** Get the current speed of the ship (m/s). */
	UFUNCTION(BlueprintPure, Category = "Flight")
	float GetCurrentSpeed() const;

	/** Get the net thrust vector in world space. */
	UFUNCTION(BlueprintPure, Category = "Flight")
	FVector GetNetThrustWorld() const;

	/** Clamp the ship's velocity to MaxSpeed. */
	void ClampSpeed();

	/** Set the maximum allowed speed (m/s). */
	UFUNCTION(BlueprintCallable, Category = "Flight")
	void SetMaxSpeed(float NewMaxSpeed);

	/** Set the acceleration multiplier (used when thrusters are active). */
	UFUNCTION(BlueprintCallable, Category = "Flight")
	void SetAccelerationMultiplier(float NewMultiplier);

protected:
	/** Array of thrusters attached to the ship. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thruster")
	TArray<FThrusterConfig> Thrusters;

	/** Maximum speed (m/s). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float MaxSpeed = 3000.f;

	/** Current speed (m/s). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight")
	float CurrentSpeed = 0.f;

	/** Acceleration multiplier applied when thrusters are active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
	float AccelerationMultiplier = 1.f;

	/** Reference to the physics component (usually the root component). */
	UPROPERTY()
	UPrimitiveComponent* PhysicsComponent = nullptr;

private:
	/** Compute the net thrust vector in world space based on active thrusters. */
	FVector ComputeNetThrustWorld() const;

	/** Apply the computed thrust to the physics body. */
	void ApplyThrustToPhysics(float DeltaTime);

	/** Helper to get the ship's velocity in world space. */
	FVector GetVelocityWorld() const;
};
