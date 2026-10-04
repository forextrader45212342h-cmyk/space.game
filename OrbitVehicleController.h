// OrbitVehicleController.h
// UE5 C++ header for a futuristic spaceship flight controller.
// Handles thruster vectors, speed, and basic movement logic.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OrbitVehicleController.generated.h"

/**
 * A simple spaceship pawn that uses thruster vectors to control movement.
 * The class is fully Blueprint‑editable and supports network replication.
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class YOURGAME_API AOrbitVehicleController : public APawn
{
	GENERATED_BODY()

public:
	/** Constructor */
	AOrbitVehicleController();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Setup player input bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Current speed of the ship (m/s) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CurrentSpeed;

	/** Maximum speed the ship can reach (m/s) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MaxSpeed;

	/** Acceleration rate (m/s²) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float Acceleration;

	/** Deceleration rate (m/s²) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float Deceleration;

	/** Thruster vector for forward/backward motion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thrusters")
	FVector ForwardThruster;

	/** Thruster vector for lateral (right/left) motion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thrusters")
	FVector LateralThruster;

	/** Thruster vector for vertical (up/down) motion */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thrusters")
	FVector VerticalThruster;

	/** Apply thrust in the given direction (normalized) */
	UFUNCTION(BlueprintCallable, Category = "Thrusters")
	void ApplyThruster(const FVector& Direction, float ThrustMagnitude);

	/** Stop all thrusters (used when input is released) */
	UFUNCTION(BlueprintCallable, Category = "Thrusters")
	void StopAllThrusters();

protected:
	/** Root component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	/** Mesh representing the spaceship */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ShipMesh;

	/** Camera boom (spring arm) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FollowCamera;

	/** Current velocity vector */
	FVector CurrentVelocity;

	/** Input flags */
	bool bThrustForward;
	bool bThrustBackward;
	bool bThrustRight;
	bool bThrustLeft;
	bool bThrustUp;
	bool bThrustDown;

	/** Input handlers */
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
};
