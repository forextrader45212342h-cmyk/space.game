// OrbitVehicleController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OrbitVehicleController.generated.h"

/**
 * A simple 3‑D vehicle controller that applies pitch, yaw, roll, gravity,
 * drag (resistance) and acceleration using the physics engine.
 */
UCLASS()
class ORBIT_API AOrbitVehicleController : public APawn
{
    GENERATED_BODY()

public:
    AOrbitVehicleController();

    /** Called every frame */
    virtual void Tick(float DeltaTime) override;

    /** Bind input actions */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;

private:
    /* Components ----------------------------------------------------------- */
    /** The physical body of the vehicle */
    UPROPERTY(VisibleAnywhere, Category = "Vehicle")
    UStaticMeshComponent* VehicleBody;

    /* Movement parameters --------------------------------------------------- */
    UPROPERTY(EditAnywhere, Category = "Movement")
    float MaxSpeed = 2000.f;          // Max linear speed (cm/s)

    UPROPERTY(EditAnywhere, Category = "Movement")
    float Acceleration = 5000.f;      // Force applied per unit input

    UPROPERTY(EditAnywhere, Category = "Movement")
    float DragCoefficient = 0.1f;     // Linear drag coefficient

    UPROPERTY(EditAnywhere, Category = "Rotation")
    float TurnRate = 45.f;            // Yaw rate (degrees per second)

    UPROPERTY(EditAnywhere, Category = "Rotation")
    float PitchRate = 30.f;           // Pitch rate (degrees per second)

    UPROPERTY(EditAnywhere, Category = "Rotation")
    float RollRate = 20.f;            // Roll rate (degrees per second)

    /* Input state ----------------------------------------------------------- */
    float ForwardInput = 0.f;
    float RightInput   = 0.f;
    float TurnInput    = 0.f;
    float PitchInput   = 0.f;
    float RollInput    = 0.f;

    /* Helper functions ----------------------------------------------------- */
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void LookUp(float Value);
    void Roll(float Value);
};

// OrbitVehicleController.cpp
#include "OrbitVehicleController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AOrbitVehicleController::AOrbitVehicleController()
{
    PrimaryActorTick.bCanEverTick = true;

    // Disable automatic controller rotation – we drive rotation manually
    bUseControllerRotationYaw   = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll  = false;

    // Create the vehicle body component
    VehicleBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleBody"));
    RootComponent = VehicleBody;
    VehicleBody->SetSimulatePhysics(true);
    VehicleBody->SetEnableGravity(true);
    VehicleBody->SetMassOverrideInKg(NAME_None, 1500.f); // Rough mass of a small car
}

void AOrbitVehicleController::BeginPlay()
{
    Super::BeginPlay();
}

void AOrbitVehicleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    /* --------------------------------------------------------------------
     * 1. Apply forward/backward and lateral forces
     * -------------------------------------------------------------------- */
    if (!FMath::IsNearlyZero(ForwardInput))
    {
        FVector Force = VehicleBody->GetForwardVector() * ForwardInput * Acceleration;
        VehicleBody->AddForce(Force);
    }

    if (!FMath::IsNearlyZero(RightInput))
    {
        FVector Force = VehicleBody->GetRightVector() * RightInput * Acceleration;
        VehicleBody->AddForce(Force);
    }

    /* --------------------------------------------------------------------
     * 2. Apply linear drag (resistance)
     * -------------------------------------------------------------------- */
    FVector Velocity = VehicleBody->GetPhysicsLinearVelocity();
    FVector Drag = -Velocity * DragCoefficient;
    VehicleBody->AddForce(Drag);

    /* --------------------------------------------------------------------
     * 3. Apply torque for yaw, pitch, roll
     * -------------------------------------------------------------------- */
    if (!FMath::IsNearlyZero(TurnInput))
    {
        FVector Torque = VehicleBody->GetUpVector() * TurnInput * FMath::DegreesToRadians(TurnRate) * 1000.f;
        VehicleBody->AddTorqueInRadians(Torque);
    }

    if (!FMath::IsNearlyZero(PitchInput))
    {
        FVector Torque = VehicleBody->GetRightVector() * PitchInput * FMath::DegreesToRadians(PitchRate) * 1000.f;
        VehicleBody->AddTorqueInRadians(Torque);
    }

    if (!FMath::IsNearlyZero(RollInput))
    {
        FVector Torque = VehicleBody->GetForwardVector() * RollInput * FMath::DegreesToRadians(RollRate) * 1000.f;
        VehicleBody->AddTorqueInRadians(Torque);
    }

    /* --------------------------------------------------------------------
     * 4. Clamp speed to MaxSpeed
     * -------------------------------------------------------------------- */
    if (Velocity.Size() > MaxSpeed)
    {
        FVector NewVel = Velocity.GetSafeNormal() * MaxSpeed;
        VehicleBody->SetPhysicsLinearVelocity(NewVel);
    }
}

void AOrbitVehicleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind movement axes (ensure these are defined in Project Settings → Input)
    PlayerInputComponent->BindAxis("MoveForward", this, &AOrbitVehicleController::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight",   this, &AOrbitVehicleController::MoveRight);
    PlayerInputComponent->BindAxis("Turn",        this, &AOrbitVehicleController::Turn);
    PlayerInputComponent->BindAxis("LookUp",      this, &AOrbitVehicleController::LookUp);
    PlayerInputComponent->BindAxis("Roll",        this, &AOrbitVehicleController::Roll);
}

void AOrbitVehicleController::MoveForward(float Value)
{
    ForwardInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::MoveRight(float Value)
{
    RightInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::Turn(float Value)
{
    TurnInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::LookUp(float Value)
{
    PitchInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::Roll(float Value)
{
    RollInput = FMath::Clamp(Value, -1.f, 1.f);
}
