// OrbitVehicleController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "OrbitVehicleController.generated.h"

/**
 *  A futuristic spaceship controller that manages thruster vectors,
 *  speed, acceleration, and rotation in a zero‑gravity environment.
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
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Returns the current speed (m/s) */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Movement")
	float GetSpeed() const;

	/** Returns the current velocity vector */
	UFUNCTION(BlueprintCallable, Category = "Orbit|Movement")
	FVector GetVelocity() const;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

private:
	/** Static mesh representing the spaceship */
	UPROPERTY(VisibleAnywhere, Category = "Orbit|Components")
	UStaticMeshComponent* ShipMesh;

	/** Movement component that handles physics simulation */
	UPROPERTY(VisibleAnywhere, Category = "Orbit|Components")
	UFloatingPawnMovement* MovementComponent;

	/** Maximum linear speed (m/s) */
	UPROPERTY(EditDefaultsOnly, Category = "Orbit|Movement")
	float MaxSpeed = 2000.f;

	/** Maximum angular speed (deg/s) */
	UPROPERTY(EditDefaultsOnly, Category = "Orbit|Movement")
	float MaxAngularSpeed = 180.f;

	/** Drag coefficient applied to linear velocity */
	UPROPERTY(EditDefaultsOnly, Category = "Orbit|Movement")
	float LinearDrag = 0.1f;

	/** Drag coefficient applied to angular velocity */
	UPROPERTY(EditDefaultsOnly, Category = "Orbit|Movement")
	float AngularDrag = 0.05f;

	/** Thruster definition */
	struct FThruster
	{
		/** Direction of the thrust relative to the ship (local space) */
		FVector Direction;

		/** Maximum force the thruster can apply (N) */
		float MaxForce;

		/** Current input value [-1, 1] */
		float CurrentInput = 0.f;
	};

	/** Array of thrusters (e.g., forward, backward, left, right, up, down) */
	UPROPERTY(EditDefaultsOnly, Category = "Orbit|Thrusters")
	TArray<FThruster> Thrusters;

	/** Rotation input values [-1, 1] */
	FVector2D RotationInput; // Pitch (X), Yaw (Y)
	float RollInput = 0.f;

	/** Apply thruster forces based on current inputs */
	void ApplyThrusters(float DeltaTime);

	/** Apply rotation based on input */
	void ApplyRotation(float DeltaTime);

	/** Clamp the ship's speed to MaxSpeed */
	void ClampSpeed();

	/** Input handlers */
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void Pitch(float Value);
	void Yaw(float Value);
	void Roll(float Value);
};
