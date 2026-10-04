// OrbitVehicleController.cpp
// 3‑D pitch / yaw / roll physics, gravity, resistance and acceleration
// ---------------------------------------------------------------------------

#include "OrbitVehicleController.h"
#include "OrbitVehicle.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

namespace Orbit
{
/* -----------------------------------------------------------------------
 *  Helper constants
 * ----------------------------------------------------------------------- */
constexpr float MaxPitchAngle   = 45.f;   // degrees
constexpr float MaxYawAngle     = 360.f;  // degrees
constexpr float MaxRollAngle    = 30.f;   // degrees

constexpr float GravityStrength = 980.f;  // cm/s² (UE default)
constexpr float DragCoefficient = 0.1f;   // arbitrary drag
constexpr float AccelForce      = 5000.f; // force applied when accelerating

/* -----------------------------------------------------------------------
 *  Constructor / Destructor
 * ----------------------------------------------------------------------- */
OrbitVehicleController::OrbitVehicleController()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

OrbitVehicleController::~OrbitVehicleController()
{
}

/* -----------------------------------------------------------------------
 *  BeginPlay
 * ----------------------------------------------------------------------- */
void OrbitVehicleController::BeginPlay()
{
    Super::BeginPlay();

    // Find the owning vehicle
    Vehicle = Cast<AOrbitVehicle>(GetOwner());
    if (!Vehicle)
    {
        UE_LOG(LogTemp, Error, TEXT("OrbitVehicleController must be attached to an AOrbitVehicle"));
        return;
    }

    // Cache the physics component
    PhysicsComp = Vehicle->GetRootComponent();
    if (!PhysicsComp)
    {
        UE_LOG(LogTemp, Error, TEXT("OrbitVehicleController: No root component found"));
    }
}

/* -----------------------------------------------------------------------
 *  TickComponent
 * ----------------------------------------------------------------------- */
void OrbitVehicleController::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!Vehicle || !PhysicsComp) return;

    // 1. Handle input
    HandleInput(DeltaTime);

    // 2. Apply physics forces
    ApplyPhysics(DeltaTime);
}

/* -----------------------------------------------------------------------
 *  HandleInput
 * ----------------------------------------------------------------------- */
void OrbitVehicleController::HandleInput(float DeltaTime)
{
    // Get the player controller that owns this vehicle
    APlayerController* PC = Cast<APlayerController>(Vehicle->GetController());
    if (!PC) return;

    // Pitch (look up/down)
    float PitchInput = 0.f;
    PC->GetInputAxisValue(TEXT("Pitch"), PitchInput);

    // Yaw (turn left/right)
    float YawInput = 0.f;
    PC->GetInputAxisValue(TEXT("Yaw"), YawInput);

    // Roll (tilt left/right)
    float RollInput = 0.f;
    PC->GetInputAxisValue(TEXT("Roll"), RollInput);

    // Acceleration (forward/backward)
    float ThrottleInput = 0.f;
    PC->GetInputAxisValue(TEXT("Throttle"), ThrottleInput);

    // Store for physics step
    DesiredPitchDelta = PitchInput * MaxPitchAngle * DeltaTime;
    DesiredYawDelta   = YawInput   * MaxYawAngle   * DeltaTime;
    DesiredRollDelta  = RollInput  * MaxRollAngle  * DeltaTime;
    DesiredThrottle   = ThrottleInput;
}

/* -----------------------------------------------------------------------
 *  ApplyPhysics
 * ----------------------------------------------------------------------- */
void OrbitVehicleController::ApplyPhysics(float DeltaTime)
{
    // 1. Orientation update (pitch/yaw/roll)
    FRotator CurrentRot = Vehicle->GetActorRotation();

    // Clamp the pitch/roll to avoid gimbal lock
    float NewPitch = FMath::Clamp(
        CurrentRot.Pitch + DesiredPitchDelta,
        -MaxPitchAngle,
        MaxPitchAngle);

    float NewYaw   = FMath::Clamp(
        CurrentRot.Yaw   + DesiredYawDelta,
        -MaxYawAngle,
        MaxYawAngle);

    float NewRoll  = FMath::Clamp(
        CurrentRot.Roll  + DesiredRollDelta,
        -MaxRollAngle,
        MaxRollAngle);

    Vehicle->SetActorRotation(FRotator(NewPitch, NewYaw, NewRoll));

    // 2. Acceleration force
    FVector Forward = Vehicle->GetActorForwardVector();
    FVector AccelForceVec = Forward * DesiredThrottle * AccelForce;

    // 3. Gravity (always downwards in world space)
    FVector GravityForce = FVector(0.f, 0.f, -GravityStrength) * Vehicle->GetMass();

    // 4. Drag / resistance (opposes velocity)
    FVector CurrentVelocity = PhysicsComp->GetPhysicsLinearVelocity();
    FVector DragForce = -DragCoefficient * CurrentVelocity;

    // 5. Sum forces
    FVector TotalForce = AccelForceVec + GravityForce + DragForce;

    // 6. Apply to physics component
    PhysicsComp->AddForce(TotalForce);

    // 7. Optional: debug visualization
    #if WITH_EDITOR
    DrawDebugLine(
        GetWorld(),
        Vehicle->GetActorLocation(),
        Vehicle->GetActorLocation() + Forward * 200.f,
        FColor::Green,
        false,
        -1.f,
        0,
        2.f);
    #endif
}

/* -----------------------------------------------------------------------
 *  Public API: SetThrottle
 * ----------------------------------------------------------------------- */
void OrbitVehicleController::SetThrottle(float Value)
{
    DesiredThrottle = FMath::Clamp(Value, -1.f, 1.f);
}

/* -----------------------------------------------------------------------
 *  Public API: SetPitchYawRoll
 * ----------------------------------------------------------------------- */
void OrbitVehicleController::SetPitchYawRoll(float Pitch, float Yaw, float Roll)
{
    DesiredPitchDelta = Pitch * MaxPitchAngle;
    DesiredYawDelta   = Yaw   * MaxYawAngle;
    DesiredRollDelta  = Roll  * MaxRollAngle;
}
} // namespace Orbit
