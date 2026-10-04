// OrbitVehicleController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OrbitVehicleController.generated.h"

/**
 *  A futuristic spaceship controller that manages thruster vectors and speed.
 *  The controller exposes thruster force, maximum speed, and current velocity
 *  to the editor and allows runtime manipulation via input or AI.
 */
UCLASS()
class ORBIT_API AOrbitVehicleController : public APawn
{
	GENERATED_BODY()

public:
	AOrbitVehicleController();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Apply a thrust impulse in the given direction */
	UFUNCTION(BlueprintCallable, Category = "Thruster")
	void ApplyThrust(const FVector& Direction, float Magnitude);

	/** Set the current thruster direction (used for AI or scripted movement) */
	UFUNCTION(BlueprintCallable, Category = "Thruster")
	void SetThrusterDirection(const FVector& NewDirection);

	/** Get the current velocity of the vehicle */
	UFUNCTION(BlueprintPure, Category = "Thruster")
	FVector GetCurrentVelocity() const { return CurrentVelocity; }

	/** Get the current speed (magnitude of velocity) */
	UFUNCTION(BlueprintPure, Category = "Thruster")
	float GetCurrentSpeed() const { return CurrentVelocity.Size(); }

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

private:
	/** Static mesh representing the spaceship */
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* ShipMesh;

	/** Current velocity of the spaceship */
	FVector CurrentVelocity;

	/** Current thruster direction (normalized) */
	FVector ThrusterDirection;

	/** Force applied per second when thrusters are active */
	UPROPERTY(EditAnywhere, Category = "Thruster")
	float ThrusterForce = 5000.0f;

	/** Maximum speed the ship can reach */
	UPROPERTY(EditAnywhere, Category = "Thruster")
	float MaxSpeed = 3000.0f;

	/** Current throttle value (0.0 to 1.0) */
	float Throttle = 0.0f;

	/** Helper to clamp velocity to MaxSpeed */
	void ClampVelocity();

	/** Input handlers */
	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveUp(float Value);
	void IncreaseThrottle();
	void DecreaseThrottle();
};
