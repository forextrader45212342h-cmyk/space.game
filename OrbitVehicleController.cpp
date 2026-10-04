// OrbitVehicleController.cpp
// 3‑D pitch / yaw / roll physics with gravity, drag and acceleration
// -----------------------------------------------------------------

#include "OrbitVehicleController.h"
#include "OrbitPhysicsSimulation.h"
#include "OrbitTypes.h"

#include <cmath>

namespace Orbit
{
    // -----------------------------------------------------------------
    // Helper constants
    // -----------------------------------------------------------------
    constexpr float GravityMagnitude = 9.81f;          // m/s²
    constexpr float DragCoefficient  = 0.1f;           // simple linear drag
    constexpr float MaxSpeed          = 200.0f;        // m/s
    constexpr float TurnSpeed         = 90.0f;         // degrees per second
    constexpr float AccelRate         = 50.0f;         // m/s²

    // -----------------------------------------------------------------
    // Constructor / Destructor
    // -----------------------------------------------------------------
    OrbitVehicleController::OrbitVehicleController()
        : CurrentVelocity(FVector::ZeroVector)
        , CurrentAngularVelocity(FVector::ZeroVector)
        , CurrentRotation(FRotator::ZeroRotator)
        , bIsAccelerating(false)
        , bIsBraking(false)
    {
    }

    OrbitVehicleController::~OrbitVehicleController()
    {
    }

    // -----------------------------------------------------------------
    // Public API – called by the input system
    // -----------------------------------------------------------------
    void OrbitVehicleController::SetAccelerationInput(bool bAccelerate)
    {
        bIsAccelerating = bAccelerate;
    }

    void OrbitVehicleController::SetBrakeInput(bool bBrake)
    {
        bIsBraking = bBrake;
    }

    void OrbitVehicleController::SetPitchInput(float PitchDelta)
    {
        DesiredPitchDelta = PitchDelta;
    }

    void OrbitVehicleController::SetYawInput(float YawDelta)
    {
        DesiredYawDelta = YawDelta;
    }

    void OrbitVehicleController::SetRollInput(float RollDelta)
    {
        DesiredRollDelta = RollDelta;
    }

    // -----------------------------------------------------------------
    // Main physics update – called once per frame
    // -----------------------------------------------------------------
    void OrbitVehicleController::Update(double DeltaSeconds)
    {
        // 1. Handle linear acceleration / braking
        HandleLinearMovement(DeltaSeconds);

        // 2. Apply gravity (always downwards in world space)
        ApplyGravity(DeltaSeconds);

        // 3. Apply drag / resistance
        ApplyDrag(DeltaSeconds);

        // 4. Clamp speed
        ClampSpeed();

        // 5. Update position (not stored here – the owning actor would use CurrentVelocity)
        //    Position update would be handled by the owning component / actor.

        // 6. Handle angular motion (pitch / yaw / roll)
        HandleAngularMovement(DeltaSeconds);
    }

    // -----------------------------------------------------------------
    // Internal helpers
    // -----------------------------------------------------------------
    void OrbitVehicleController::HandleLinearMovement(double DeltaSeconds)
    {
        // Forward vector in world space
        const FVector Forward = CurrentRotation.Vector();

        // Acceleration vector
        FVector Accel = FVector::ZeroVector;

        if (bIsAccelerating)
        {
            Accel += Forward * AccelRate;
        }

        if (bIsBraking)
        {
            // Simple braking – apply opposite acceleration
            Accel -= Forward * AccelRate;
        }

        // Update velocity
        CurrentVelocity += Accel * static_cast<float>(DeltaSeconds);
    }

    void OrbitVehicleController::ApplyGravity(double DeltaSeconds)
    {
        // Gravity acts in negative Z direction in UE world space
        const FVector Gravity = FVector(0.0f, 0.0f, -GravityMagnitude);
        CurrentVelocity += Gravity * static_cast<float>(DeltaSeconds);
    }

    void OrbitVehicleController::ApplyDrag(double DeltaSeconds)
    {
        // Simple linear drag proportional to velocity
        CurrentVelocity -= CurrentVelocity * DragCoefficient * static_cast<float>(DeltaSeconds);
    }

    void OrbitVehicleController::ClampSpeed()
    {
        const float SpeedSq = CurrentVelocity.SizeSquared();
        if (SpeedSq > MaxSpeed * MaxSpeed)
        {
            CurrentVelocity = CurrentVelocity.GetSafeNormal() * MaxSpeed;
        }
    }

    void OrbitVehicleController::HandleAngularMovement(double DeltaSeconds)
    {
        // Convert desired deltas to angular velocity
        CurrentAngularVelocity.X = DesiredPitchDelta * TurnSpeed; // Pitch
        CurrentAngularVelocity.Y = DesiredYawDelta   * TurnSpeed; // Yaw
        CurrentAngularVelocity.Z = DesiredRollDelta  * TurnSpeed; // Roll

        // Update rotation
        FRotator DeltaRot = FRotator(
            CurrentAngularVelocity.X * static_cast<float>(DeltaSeconds),
            CurrentAngularVelocity.Y * static_cast<float>(DeltaSeconds),
            CurrentAngularVelocity.Z * static_cast<float>(DeltaSeconds)
        );

        CurrentRotation += DeltaRot;
        CurrentRotation.Normalize();

        // Reset desired deltas after applying
        DesiredPitchDelta = 0.0f;
        DesiredYawDelta   = 0.0f;
        DesiredRollDelta  = 0.0f;
    }

    // -----------------------------------------------------------------
    // Accessors – used by the owning actor / component
    // -----------------------------------------------------------------
    const FVector& OrbitVehicleController::GetVelocity() const
    {
        return CurrentVelocity;
    }

    const FRotator& OrbitVehicleController::GetRotation() const
    {
        return CurrentRotation;
    }
} // namespace Orbit
