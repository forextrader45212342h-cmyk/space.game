// OrbitVehicleController.cpp
// 3‑D pitch / yaw / roll physics with gravity, resistance and acceleration
// -------------------------------------------------------------------------

#include "OrbitVehicleController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// Helper constants
//////////////////////////////////////////////////////////////////////////

// Default physical constants
static constexpr float DefaultGravity = 980.0f;          // cm/s²
static constexpr float DefaultResistance = 0.1f;         // 10% per second
static constexpr float DefaultAcceleration = 2000.0f;    // cm/s²
static constexpr float MaxSpeed = 4000.0f;              // cm/s

//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////

AOrbitVehicleController::AOrbitVehicleController()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    // Create a simple root component that will be used for physics
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent->SetMobility(EComponentMobility::Movable);

    // Enable physics simulation on the root component
    RootComponent->SetSimulatePhysics(true);
    RootComponent->SetEnableGravity(false); // We'll apply custom gravity

    // Initialise state
    CurrentVelocity = FVector::ZeroVector;
    CurrentAngularVelocity = FVector::ZeroVector;
    bIsAccelerating = false;
}

//////////////////////////////////////////////////////////////////////////
// Input binding
//////////////////////////////////////////////////////////////////////////

void AOrbitVehicleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Pitch / yaw / roll
    PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AOrbitVehicleController::PitchInput);
    PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AOrbitVehicleController::YawInput);
    PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AOrbitVehicleController::RollInput);

    // Acceleration
    PlayerInputComponent->BindAction(TEXT("Accelerate"), IE_Pressed, this, &AOrbitVehicleController::StartAccelerating);
    PlayerInputComponent->BindAction(TEXT("Accelerate"), IE_Released, this, &AOrbitVehicleController::StopAccelerating);
}

//////////////////////////////////////////////////////////////////////////
// Tick
//////////////////////////////////////////////////////////////////////////

void AOrbitVehicleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 1. Apply custom gravity
    ApplyGravity(DeltaTime);

    // 2. Apply resistance (drag)
    ApplyResistance(DeltaTime);

    // 3. Apply acceleration if requested
    if (bIsAccelerating)
    {
        ApplyAcceleration(DeltaTime);
    }

    // 4. Update orientation based on angular velocity
    UpdateOrientation(DeltaTime);

    // 5. Clamp speed
    ClampSpeed();

    // 6. Apply the updated velocity to the physics body
    RootComponent->SetPhysicsLinearVelocity(CurrentVelocity, true);
}

//////////////////////////////////////////////////////////////////////////
// Input handlers
//////////////////////////////////////////////////////////////////////////

void AOrbitVehicleController::PitchInput(float Value)
{
    // Positive pitch rotates nose up
    CurrentAngularVelocity.X = FMath::Clamp(Value, -1.0f, 1.0f) * MaxAngularSpeed;
}

void AOrbitVehicleController::YawInput(float Value)
{
    // Positive yaw rotates right
    CurrentAngularVelocity.Y = FMath::Clamp(Value, -1.0f, 1.0f) * MaxAngularSpeed;
}

void AOrbitVehicleController::RollInput(float Value)
{
    // Positive roll rotates clockwise
    CurrentAngularVelocity.Z = FMath::Clamp(Value, -1.0f, 1.0f) * MaxAngularSpeed;
}

void AOrbitVehicleController::StartAccelerating()
{
    bIsAccelerating = true;
}

void AOrbitVehicleController::StopAccelerating()
{
    bIsAccelerating = false;
}

//////////////////////////////////////////////////////////////////////////
// Physics helpers
//////////////////////////////////////////////////////////////////////////

void AOrbitVehicleController::ApplyGravity(float DeltaTime)
{
    // Custom gravity vector (downwards in world space)
    const FVector GravityVector = FVector(0.0f, 0.0f, -DefaultGravity);
    CurrentVelocity += GravityVector * DeltaTime;
}

void AOrbitVehicleController::ApplyResistance(float DeltaTime)
{
    // Simple linear drag: v = v * (1 - k * dt)
    const float DragFactor = 1.0f - DefaultResistance * DeltaTime;
    CurrentVelocity *= DragFactor;
}

void AOrbitVehicleController::ApplyAcceleration(float DeltaTime)
{
    // Accelerate in the forward direction of the actor
    const FVector Forward = GetActorForwardVector();
    CurrentVelocity += Forward * DefaultAcceleration * DeltaTime;
}

void AOrbitVehicleController::UpdateOrientation(float DeltaTime)
{
    // Convert angular velocity (deg/s) to a rotation delta
    const FRotator DeltaRot = FRotator(
        CurrentAngularVelocity.X * DeltaTime,
        CurrentAngularVelocity.Y * DeltaTime,
        CurrentAngularVelocity.Z * DeltaTime
    );

    // Apply rotation to the actor
    AddActorLocalRotation(DeltaRot);
}

void AOrbitVehicleController::ClampSpeed()
{
    if (CurrentVelocity.Size() > MaxSpeed)
    {
        CurrentVelocity = CurrentVelocity.GetSafeNormal() * MaxSpeed;
    }
}

//////////////////////////////////////////////////////////////////////////
// Debug helpers
//////////////////////////////////////////////////////////////////////////

void AOrbitVehicleController::DrawDebugInfo()
{
    if (!RootComponent) return;

    // Draw velocity vector
    const FVector Start = RootComponent->GetComponentLocation();
    const FVector End = Start + CurrentVelocity * 0.1f; // scale for visibility
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1.0f, 0, 2.0f);

    // Draw angular velocity as a small arrow
    const FVector AngStart = Start + FVector(0, 0, 50);
    const FVector AngEnd = AngStart + CurrentAngularVelocity * 0.1f;
    DrawDebugLine(GetWorld(), AngStart, AngEnd, FColor::Blue, false, -1.0f, 0, 2.0f);
}
