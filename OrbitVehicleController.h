// OrbitVehicleController.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitVehicleController.generated.h"

/**
 *  A single thruster definition.
 *  Direction is defined in local space of the owning actor.
 *  MaxForce is the maximum force (in Newtons) that this thruster can apply.
 *  bEnabled allows the thruster to be toggled on/off at runtime.
 */
USTRUCT(BlueprintType)
struct FThruster
{
	GENERATED_BODY()

	/** Direction of thrust in local space (should be normalized). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thruster")
	FVector Direction = FVector::ForwardVector;

	/** Maximum force this thruster can produce (N). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thruster")
	float MaxForce = 1000.f;

	/** Whether this thruster is currently active. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thruster")
	bool bEnabled = true;
};

/**
 *  OrbitVehicleController
 *  Handles spaceship flight using a set of thrusters.
 *  Supports forward/backward, lateral, vertical, and rotational thrust.
 *  Speed is clamped to MaxSpeed and updated each tick.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ORBIT_API UOrbitVehicleController : public UActorComponent
{
	GENERATED_BODY()

public:
	UOrbitVehicleController();

	/** Called every frame. */
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Called when the game starts. */
	virtual void BeginPlay() override;

	/** Apply thrust to a specific thruster. Input is 0.0 .. 1.0. */
	UFUNCTION(BlueprintCallable, Category = "Thruster")
	void ApplyThruster(int32 ThrusterIndex, float Input);

	/** Apply rotational thrust around local axes. */
	UFUNCTION(BlueprintCallable, Category = "Thruster")
	void ApplyRotationalThrust(const FVector& RotationInput, float DeltaTime);

	/** Set the maximum speed of the vehicle. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMaxSpeed(float NewMaxSpeed);

	/** Get the current speed (magnitude of velocity). */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetSpeed() const { return CurrentVelocity.Size(); }

	/** Get the current velocity vector in world space. */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetVelocity() const { return CurrentVelocity; }

	/** Array of thrusters that drive the vehicle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thrusters")
	TArray<FThruster> Thrusters;

	/** Maximum speed (m/s). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxSpeed = 3000.f;

	/** Acceleration rate (m/s^2). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Acceleration = 500.f;

	/** Deceleration rate when no thrust is applied (m/s^2). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Deceleration = 200.f;

	/** Whether to use the physics engine for movement. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bUsePhysics = true;

protected:
	/** Current velocity in world space. */
	FVector CurrentVelocity = FVector::ZeroVector;

	/** Cached reference to the root physics component. */
	UPrimitiveComponent* PhysicsComponent = nullptr;

	/** Compute the net thrust vector from all active thrusters. */
	FVector ComputeNetThrust() const;

	/** Apply the computed thrust to the physics component or directly to velocity. */
	void ApplyThrust(const FVector& Thrust, float DeltaTime);

	/** Clamp the velocity to MaxSpeed. */
	void ClampSpeed();
};
