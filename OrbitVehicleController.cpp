// OrbitVehicleController.cpp
// Implements 3‑D pitch/yaw/roll physics, gravity, drag and acceleration for a vehicle actor.

#include "OrbitVehicleController.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "Math/UnrealMathUtility.h"

UOrbitVehicleController::UOrbitVehicleController()
{
    PrimaryComponentTick.bCanEverTick = true;

    // Default configuration – these can be overridden in the editor or via code
    MaxSpeed          = 3000.f;   // Unreal units per second
    Acceleration      = 2000.f;   // Unreal units per second²
    TurnSpeed         = 90.f;     // Degrees per second
    RollSpeed         = 120.f;    // Degrees per second
    DragCoefficient   = 0.1f;     // 10% per second
    GravityStrength   = 980.f;    // Unreal units per second² (≈ 1g)
}

void UOrbitVehicleController::BeginPlay()
{
    Super::BeginPlay();

    // Ensure the owning actor has a physics body if we want to use the physics engine
    if (AActor* Owner = GetOwner())
    {
        if (UPrimitiveComponent* Root = Cast<UPrimitiveComponent>(Owner->GetRootComponent()))
        {
            Root->SetSimulatePhysics(false); // We handle physics manually
        }
    }
}

void UOrbitVehicleController::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // 1. Apply user input to rotation
    ApplyRotation(DeltaTime);

    // 2. Apply physics forces
    ApplyPhysics(DeltaTime);

    // 3. Update actor transform
    UpdateActorTransform(DeltaTime);
}

void UOrbitVehicleController::SetInputPitch(float Value)
{
    PitchInput = FMath::Clamp(Value, -1.f, 1.f);
}

void UOrbitVehicleController::SetInputYaw(float Value)
{
    YawInput = FMath::Clamp(Value, -1.f, 1.f);
}

void UOrbitVehicleController::SetInputRoll(float Value)
{
    RollInput = FMath::Clamp(Value, -1.f, 1.f);
}

void UOrbitVehicleController::SetInputThrottle(float Value)
{
    ThrottleInput = FMath::Clamp(Value, 0.f, 1.f);
}

void UOrbitVehicleController::ApplyRotation(float DeltaTime)
{
    if (!GetOwner())
        return;

    // Compute rotation delta from input
    const FRotator DeltaRot(
        PitchInput * TurnSpeed * DeltaTime,   // Pitch
        YawInput   * TurnSpeed * DeltaTime,   // Yaw
        RollInput  * RollSpeed * DeltaTime    // Roll
    );

    // Apply local rotation
    GetOwner()->AddActorLocalRotation(DeltaRot);
}

void UOrbitVehicleController::ApplyPhysics(float DeltaTime)
{
    // 1. Gravity
    Velocity += FVector(0.f, 0.f, -GravityStrength) * DeltaTime;

    // 2. Drag (linear)
    const float DragFactor = 1.f - DragCoefficient * DeltaTime;
    Velocity *= FMath::Clamp(DragFactor, 0.f, 1.f);

    // 3. Throttle acceleration in forward direction
    if (ThrottleInput > KINDA_SMALL_NUMBER)
    {
        const FVector Forward = GetOwner()->GetActorForwardVector();
        Velocity += Forward * ThrottleInput * Acceleration * DeltaTime;
    }

    // 4. Clamp speed
    if (Velocity.SizeSquared() > MaxSpeed * MaxSpeed)
    {
        Velocity = Velocity.GetSafeNormal() * MaxSpeed;
    }
}

void UOrbitVehicleController::UpdateActorTransform(float DeltaTime)
{
    if (!GetOwner())
        return;

    // Move actor by velocity
    const FVector NewLocation = GetOwner()->GetActorLocation() + Velocity * DeltaTime;
    GetOwner()->SetActorLocation(NewLocation, true);
}
