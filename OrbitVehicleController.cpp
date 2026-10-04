// OrbitVehicleController.cpp
// 3‑D pitch / yaw / roll physics with gravity resistance and acceleration
// UE5 C++ – only valid code

#include "OrbitVehicleController.h"
#include "GameFramework/PlayerController.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AOrbitVehicleController::AOrbitVehicleController()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create a simple capsule component as the root
    CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
    RootComponent = CapsuleComp;
    CapsuleComp->InitCapsuleSize(42.f, 96.f);

    // Create a static mesh for visual representation
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetupAttachment(RootComponent);

    // Default physics values
    MaxSpeed = 2000.f;
    Acceleration = 5000.f;
    TurnSpeed = 120.f;          // degrees per second
    RollSpeed = 120.f;
    Gravity = FVector(0.f, 0.f, -980.f); // Unreal units: cm/s²
    DragCoefficient = 0.1f;    // simple linear drag
    bUseGravity = true;
}

void AOrbitVehicleController::BeginPlay()
{
    Super::BeginPlay();

    // Disable default pawn movement
    GetMovementComponent()->SetActive(false);
}

void AOrbitVehicleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Pitch / Yaw / Roll
    PlayerInputComponent->BindAxis("Pitch", this, &AOrbitVehicleController::PitchInput);
    PlayerInputComponent->BindAxis("Yaw", this, &AOrbitVehicleController::YawInput);
    PlayerInputComponent->BindAxis("Roll", this, &AOrbitVehicleController::RollInput);

    // Throttle
    PlayerInputComponent->BindAxis("Throttle", this, &AOrbitVehicleController::ThrottleInput);
}

void AOrbitVehicleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update orientation based on input
    UpdateRotation(DeltaTime);

    // Update velocity based on throttle and drag
    UpdateVelocity(DeltaTime);

    // Apply gravity if enabled
    if (bUseGravity)
    {
        Velocity += Gravity * DeltaTime;
    }

    // Clamp speed
    if (Velocity.Size() > MaxSpeed)
    {
        Velocity = Velocity.GetSafeNormal() * MaxSpeed;
    }

    // Move actor
    FVector NewLocation = GetActorLocation() + Velocity * DeltaTime;
    SetActorLocation(NewLocation, true);

    // Debug: draw velocity vector
    DrawDebugLine(GetWorld(), NewLocation, NewLocation + Velocity * 0.1f, FColor::Green, false, -1.f, 0, 2.f);
}

void AOrbitVehicleController::PitchInput(float Value)
{
    PitchInputValue = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::YawInput(float Value)
{
    YawInputValue = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::RollInput(float Value)
{
    RollInputValue = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::ThrottleInput(float Value)
{
    ThrottleInputValue = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::UpdateRotation(float DeltaTime)
{
    // Calculate desired rotation change
    FRotator DeltaRot = FRotator(
        PitchInputValue * TurnSpeed * DeltaTime,
        YawInputValue * TurnSpeed * DeltaTime,
        RollInputValue * RollSpeed * DeltaTime
    );

    // Apply rotation
    FRotator NewRot = GetActorRotation() + DeltaRot;
    SetActorRotation(NewRot);
}

void AOrbitVehicleController::UpdateVelocity(float DeltaTime)
{
    // Forward vector in world space
    FVector Forward = GetActorForwardVector();

    // Acceleration in forward direction
    FVector Accel = Forward * ThrottleInputValue * Acceleration;

    // Simple linear drag opposing velocity
    FVector Drag = -DragCoefficient * Velocity;

    // Net acceleration
    FVector NetAccel = Accel + Drag;

    // Update velocity
    Velocity += NetAccel * DeltaTime;
}
