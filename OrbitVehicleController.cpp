// OrbitVehicleController.cpp
// 3‑D pitch / yaw / roll controller with gravity, drag and acceleration
// --------------------------------------------------------------------

#include "OrbitVehicleController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// Constructor / Setup

AOrbitVehicleController::AOrbitVehicleController()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    // Root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Vehicle body
    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
    Body->SetupAttachment(RootComponent);
    Body->SetSimulatePhysics(true);
    Body->SetEnableGravity(true);
    Body->SetMassOverrideInKg(NAME_None, 1500.f); // typical car mass

    // Camera boom (optional)
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(Body);
    SpringArm->TargetArmLength = 300.f;
    SpringArm->bUsePawnControlRotation = true;

    // Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    // Input flags
    bUseControllerRotationYaw = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
}

//////////////////////////////////////////////////////////////////////////
// Input binding

void AOrbitVehicleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Axis bindings
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AOrbitVehicleController::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AOrbitVehicleController::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AOrbitVehicleController::Pitch);
    PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AOrbitVehicleController::Yaw);
    PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AOrbitVehicleController::Roll);

    // Action bindings
    PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AOrbitVehicleController::Jump);
}

//////////////////////////////////////////////////////////////////////////
// Tick – physics integration

void AOrbitVehicleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!Body) return;

    // 1. Apply gravity (already handled by physics engine)
    // 2. Apply drag (resistance)
    ApplyDrag(DeltaTime);

    // 3. Apply user‑controlled torque for pitch/yaw/roll
    ApplyTorque(DeltaTime);

    // 4. Apply forward/backward acceleration
    ApplyThrottle(DeltaTime);
}

//////////////////////////////////////////////////////////////////////////
// Input handlers

void AOrbitVehicleController::MoveForward(float Value)
{
    ThrottleInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::MoveRight(float Value)
{
    // Optional lateral movement (e.g., strafing)
    // Not used in this example but kept for completeness
    StrafeInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::Pitch(float Value)
{
    PitchInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::Yaw(float Value)
{
    YawInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::Roll(float Value)
{
    RollInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::Jump()
{
    // Simple upward impulse
    if (Body)
    {
        Body->AddImpulse(FVector::UpVector * JumpImpulse, NAME_None, true);
    }
}

//////////////////////////////////////////////////////////////////////////
// Physics helpers

void AOrbitVehicleController::ApplyDrag(float DeltaTime)
{
    if (!Body) return;

    // Linear drag proportional to velocity squared
    FVector Velocity = Body->GetPhysicsLinearVelocity();
    FVector DragForce = -DragCoefficient * Velocity.SizeSquared() * Velocity.GetSafeNormal();
    Body->AddForce(DragForce, NAME_None, true);
}

void AOrbitVehicleController::ApplyTorque(float DeltaTime)
{
    if (!Body) return;

    // Convert input to torque in world space
    FVector Torque = FVector::ZeroVector;
    Torque.X = PitchInput * PitchTorque;   // Pitch around local X
    Torque.Y = RollInput * RollTorque;     // Roll around local Y
    Torque.Z = YawInput * YawTorque;       // Yaw around local Z

    // Transform torque to world space
    FVector WorldTorque = Body->GetComponentTransform().TransformVectorNoScale(Torque);

    Body->AddTorqueInRadians(WorldTorque, NAME_None, true);
}

void AOrbitVehicleController::ApplyThrottle(float DeltaTime)
{
    if (!Body) return;

    // Forward thrust along vehicle's local X axis
    FVector Forward = Body->GetForwardVector();
    FVector Thrust = Forward * ThrottleInput * MaxThrust;

    Body->AddForce(Thrust, NAME_None, true);
}

//////////////////////////////////////////////////////////////////////////
// Debug drawing (optional)

void AOrbitVehicleController::DrawDebugInfo()
{
    if (!Body) return;

    FVector Location = Body->GetComponentLocation();
    FVector Velocity = Body->GetPhysicsLinearVelocity();

    // Draw velocity vector
    DrawDebugLine(
        GetWorld(),
        Location,
        Location + Velocity,
        FColor::Green,
        false,
        -1.f,
        0,
        2.f
    );

    // Draw drag force
    FVector Drag = -DragCoefficient * Velocity.SizeSquared() * Velocity.GetSafeNormal();
    DrawDebugLine(
        GetWorld(),
        Location,
        Location + Drag,
        FColor::Red,
        false,
        -1.f,
        0,
        2.f
    );
}
