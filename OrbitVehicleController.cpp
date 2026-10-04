// OrbitVehicleController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OrbitVehicleController.generated.h"

UCLASS()
class ORBIT_API AOrbitVehicleController : public APawn
{
	GENERATED_BODY()

public:
	AOrbitVehicleController();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Setup player input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

private:
	/** Vehicle body */
	UPROPERTY(VisibleAnywhere, Category = "Vehicle")
	UStaticMeshComponent* VehicleBody;

	/** Current throttle value [-1, 1] */
	float Throttle;

	/** Current pitch input [-1, 1] */
	float PitchInput;

	/** Current yaw input [-1, 1] */
	float YawInput;

	/** Current roll input [-1, 1] */
	float RollInput;

	/** Max forward force (N) */
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle|Physics")
	float MaxForwardForce = 5000.f;

	/** Max torque (N·m) for pitch, yaw, roll */
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle|Physics")
	float MaxTorque = 2000.f;

	/** Linear drag coefficient (kg/s) */
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle|Physics")
	float LinearDrag = 50.f;

	/** Angular drag coefficient (kg·m²/s) */
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle|Physics")
	float AngularDrag = 10.f;

	/** Gravity override (m/s²). If zero, use world gravity */
	UPROPERTY(EditDefaultsOnly, Category = "Vehicle|Physics")
	FVector CustomGravity = FVector::ZeroVector;

	/** Input handlers */
	void MoveForward(float Value);
	void Pitch(float Value);
	void Yaw(float Value);
	void Roll(float Value);
	void Brake(float Value);
};
