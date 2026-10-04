// OrbitVehicleController.cpp
// 3‑D pitch / yaw / roll controller with gravity, drag and acceleration
// -----------------------------------------------------------------

#include "OrbitVehicleController.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

namespace Orbit
{
    // -----------------------------------------------------------------
    // Helper constants
    // -----------------------------------------------------------------
    constexpr float GravityStrength   = 980.f;          // cm/s² (UE default)
    constexpr float DragCoefficient   = 0.1f;          // Simple linear drag
    constexpr float MaxSpeed          = 3000.f;        // cm/s
    constexpr float AccelRate         = 2000.f;        // cm/s²
    constexpr float TurnRate          = 90.f;          // degrees per second

    // -----------------------------------------------------------------
    // Constructor
    // -----------------------------------------------------------------
    OrbitVehicleController::OrbitVehicleController()
        : CurrentVelocity(FVector::ZeroVector)
        , CurrentAngularVelocity(FRotator::ZeroRotator)
    {
        PrimaryComponentTick.bCanEverTick = true;
    }

    // -----------------------------------------------------------------
    // Tick
    // -----------------------------------------------------------------
    void OrbitVehicleController::TickComponent(
        float DeltaTime,
        enum ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction)
    {
        Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

        if (!GetOwner() || !GetOwner()->IsValidLowLevelFast())
        {
            return;
        }

        // 1. Handle input
        HandleInput(DeltaTime);

        // 2. Apply physics
        ApplyGravity(DeltaTime);
        ApplyDrag(DeltaTime);
        ClampSpeed();

        // 3. Update transform
        UpdateTransform(DeltaTime);
    }

    // -----------------------------------------------------------------
    // Input handling
    // -----------------------------------------------------------------
    void OrbitVehicleController::HandleInput(float DeltaTime)
    {
        // Pitch / yaw / roll from input axes
        const float PitchInput = InputComponent->GetAxisValue(TEXT("Pitch"));
        const float YawInput   = InputComponent->GetAxisValue(TEXT("Yaw"));
        const float RollInput  = InputComponent->GetAxisValue(TEXT("Roll"));
        const float Throttle   = InputComponent->GetAxisValue(TEXT("Throttle"));

        // Angular velocity integration
        CurrentAngularVelocity.Pitch += PitchInput * TurnRate * DeltaTime;
        CurrentAngularVelocity.Yaw   += YawInput   * TurnRate * DeltaTime;
        CurrentAngularVelocity.Roll  += RollInput  * TurnRate * DeltaTime;

        // Acceleration in local forward direction
        const FVector Forward = GetOwner()->GetActorForwardVector();
        CurrentVelocity += Forward * Throttle * AccelRate * DeltaTime;
    }

    // -----------------------------------------------------------------
    // Gravity
    // -----------------------------------------------------------------
    void OrbitVehicleController::ApplyGravity(float DeltaTime)
    {
        // Simple constant gravity in world Z‑down direction
        CurrentVelocity.Z -= GravityStrength * DeltaTime;
    }

    // -----------------------------------------------------------------
    // Drag / resistance
    // -----------------------------------------------------------------
    void OrbitVehicleController::ApplyDrag(float DeltaTime)
    {
        // Linear drag proportional to velocity magnitude
        const float Speed = CurrentVelocity.Size();
        if (Speed > KINDA_SMALL_NUMBER)
        {
            const FVector Drag = -CurrentVelocity.GetSafeNormal() * DragCoefficient * Speed * DeltaTime;
            CurrentVelocity += Drag;
        }
    }

    // -----------------------------------------------------------------
    // Clamp speed
    // -----------------------------------------------------------------
    void OrbitVehicleController::ClampSpeed()
    {
        const float Speed = CurrentVelocity.Size();
        if (Speed > MaxSpeed)
        {
            CurrentVelocity = CurrentVelocity.GetSafeNormal() * MaxSpeed;
        }
    }

    // -----------------------------------------------------------------
    // Transform update
    // -----------------------------------------------------------------
    void OrbitVehicleController::UpdateTransform(float DeltaTime)
    {
        // Update location
        FVector NewLocation = GetOwner()->GetActorLocation() + CurrentVelocity * DeltaTime;

        // Update rotation
        FRotator CurrentRot = GetOwner()->GetActorRotation();
        FRotator DeltaRot = CurrentAngularVelocity * DeltaTime;
        FRotator NewRot = CurrentRot + DeltaRot;

        // Apply to actor
        GetOwner()->SetActorLocationAndRotation(NewLocation, NewRot, false, nullptr, ETeleportType::None);
    }

    // -----------------------------------------------------------------
    // Input binding
    // -----------------------------------------------------------------
    void OrbitVehicleController::SetupInputComponent()
    {
        Super::SetupInputComponent();

        InputComponent->BindAxis(TEXT("Pitch"));
        InputComponent->BindAxis(TEXT("Yaw"));
        InputComponent->BindAxis(TEXT("Roll"));
        InputComponent->BindAxis(TEXT("Throttle"));
    }
}
