// OrbitVehicleController.cpp
// 3‑D pitch / yaw / roll controller with gravity, drag and acceleration
// -----------------------------------------------------------------

#include "OrbitVehicleController.h"
#include "OrbitPhysicsSimulation.h"
#include "OrbitRenderCore.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

#define DRAG_COEFFICIENT 0.1f   // Simple linear drag
#define MAX_THRUST 5000.0f      // Max forward force
#define MAX_TORQUE 2000.0f      // Max torque for pitch/yaw/roll

// ---------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------
AOrbitVehicleController::AOrbitVehicleController()
{
    PrimaryActorTick.bCanEverTick = true;

    // Root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Vehicle body
    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
    Body->SetupAttachment(RootComponent);
    Body->SetSimulatePhysics(true);
    Body->SetEnableGravity(true);
    Body->SetMassOverrideInKg(NAME_None, 1500.0f); // Example mass

    // Camera
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(Body);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 3.0f;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

// ---------------------------------------------------------------------
// Input binding
// ---------------------------------------------------------------------
void AOrbitVehicleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AOrbitVehicleController::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AOrbitVehicleController::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &AOrbitVehicleController::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &AOrbitVehicleController::LookUp);
    PlayerInputComponent->BindAxis("Roll", this, &AOrbitVehicleController::Roll);
}

// ---------------------------------------------------------------------
// Tick – apply physics each frame
// ---------------------------------------------------------------------
void AOrbitVehicleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Apply drag (gravity resistance)
    FVector Velocity = Body->GetPhysicsLinearVelocity();
    FVector DragForce = -Velocity.GetSafeNormal() * DRAG_COEFFICIENT * Velocity.SizeSquared();
    Body->AddForce(DragForce, NAME_None, true);

    // Optional: debug draw velocity
    // DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Velocity * 0.1f, FColor::Green, false, -1, 0, 2);
}

// ---------------------------------------------------------------------
// Movement input handlers
// ---------------------------------------------------------------------
void AOrbitVehicleController::MoveForward(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    // Forward thrust in local X direction
    FVector Force = Body->GetForwardVector() * Value * MAX_THRUST;
    Body->AddForce(Force, NAME_None, true);
}

void AOrbitVehicleController::MoveRight(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    // Lateral thrust in local Y direction
    FVector Force = Body->GetRightVector() * Value * MAX_THRUST;
    Body->AddForce(Force, NAME_None, true);
}

// ---------------------------------------------------------------------
// Rotation input handlers – pitch, yaw, roll
// ---------------------------------------------------------------------
void AOrbitVehicleController::Turn(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    // Yaw – torque around local Z axis
    FVector Torque = Body->GetUpVector() * Value * MAX_TORQUE;
    Body->AddTorqueInRadians(Torque, NAME_None, true);
}

void AOrbitVehicleController::LookUp(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    // Pitch – torque around local Y axis
    FVector Torque = Body->GetRightVector() * Value * MAX_TORQUE;
    Body->AddTorqueInRadians(Torque, NAME_None, true);
}

void AOrbitVehicleController::Roll(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    // Roll – torque around local X axis
    FVector Torque = Body->GetForwardVector() * Value * MAX_TORQUE;
    Body->AddTorqueInRadians(Torque, NAME_None, true);
}

// ---------------------------------------------------------------------
// Optional: expose physics simulation to the engine
// ---------------------------------------------------------------------
void AOrbitVehicleController::GetPhysicsState(FVector& OutVelocity, FVector& OutAngularVelocity) const
{
    OutVelocity = Body->GetPhysicsLinearVelocity();
    OutAngularVelocity = Body->GetPhysicsAngularVelocityInRadians();
}
