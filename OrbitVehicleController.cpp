// OrbitVehicleController.cpp
// 3‑D pitch / yaw / roll controller with gravity, drag and acceleration
// -----------------------------------------------------------------

#include "OrbitVehicleController.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "OrbitPhysicsSimulation.h"

#define LOCTEXT_NAMESPACE "OrbitVehicleController"

//////////////////////////////////////////////////////////////////////////
// AOrbitVehicleController

AOrbitVehicleController::AOrbitVehicleController()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create a simple physics body for the vehicle
    VehicleBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleBody"));
    RootComponent = VehicleBody;
    VehicleBody->SetSimulatePhysics(true);
    VehicleBody->SetEnableGravity(false); // We handle gravity manually
    VehicleBody->SetMassOverrideInKg(NAME_None, 1500.f); // Rough car mass

    // Default values
    MaxSpeed = 2000.f;          // Unreal units per second
    Acceleration = 5000.f;      // Unreal units per second^2
    TurnSpeed = 120.f;          // Degrees per second
    DragCoefficient = 0.1f;     // Simple linear drag
    Gravity = FVector(0.f, 0.f, -980.f); // Unreal units per second^2
}

void AOrbitVehicleController::BeginPlay()
{
    Super::BeginPlay();

    // Bind input
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        InputComponent = NewObject<UInputComponent>(this);
        InputComponent->RegisterComponent();

        InputComponent->BindAxis(TEXT("MoveForward"), this, &AOrbitVehicleController::MoveForward);
        InputComponent->BindAxis(TEXT("MoveRight"), this, &AOrbitVehicleController::MoveRight);
        InputComponent->BindAxis(TEXT("Turn"), this, &AOrbitVehicleController::Turn);
        InputComponent->BindAxis(TEXT("LookUp"), this, &AOrbitVehicleController::LookUp);
        InputComponent->BindAxis(TEXT("Roll"), this, &AOrbitVehicleController::Roll);

        PC->PushInputComponent(InputComponent);
    }
}

void AOrbitVehicleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 1. Apply gravity
    FVector GravityForce = Gravity * VehicleBody->GetMass();
    VehicleBody->AddForce(GravityForce);

    // 2. Apply drag (simple linear drag)
    FVector Velocity = VehicleBody->GetPhysicsLinearVelocity();
    FVector DragForce = -DragCoefficient * Velocity;
    VehicleBody->AddForce(DragForce);

    // 3. Clamp speed
    if (Velocity.Size() > MaxSpeed)
    {
        FVector NewVel = Velocity.GetSafeNormal() * MaxSpeed;
        VehicleBody->SetPhysicsLinearVelocity(NewVel);
    }

    // 4. Update orientation based on input
    UpdateOrientation(DeltaTime);
}

//////////////////////////////////////////////////////////////////////////
// Input handlers

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
    YawInput = Value;
}

void AOrbitVehicleController::LookUp(float Value)
{
    PitchInput = Value;
}

void AOrbitVehicleController::Roll(float Value)
{
    RollInput = Value;
}

//////////////////////////////////////////////////////////////////////////
// Physics helpers

void AOrbitVehicleController::UpdateOrientation(float DeltaTime)
{
    // Desired forward direction in world space
    const FRotator CurrentRot = VehicleBody->GetComponentRotation();
    const FVector ForwardDir = CurrentRot.Vector();
    const FVector RightDir = CurrentRot.RotateVector(FVector::RightVector);

    // Apply forward/backward acceleration
    if (!FMath::IsNearlyZero(ForwardInput))
    {
        FVector Force = ForwardDir * ForwardInput * Acceleration * VehicleBody->GetMass();
        VehicleBody->AddForce(Force);
    }

    // Apply lateral acceleration (for drifting / turning)
    if (!FMath::IsNearlyZero(RightInput))
    {
        FVector Force = RightDir * RightInput * Acceleration * VehicleBody->GetMass();
        VehicleBody->AddForce(Force);
    }

    // Apply yaw, pitch, roll torques
    const FVector YawTorque = FVector(0.f, 0.f, YawInput * TurnSpeed * VehicleBody->GetMass());
    const FVector PitchTorque = FVector(PitchInput * TurnSpeed * VehicleBody->GetMass(), 0.f, 0.f);
    const FVector RollTorque = FVector(0.f, RollInput * TurnSpeed * VehicleBody->GetMass(), 0.f);

    VehicleBody->AddTorqueInRadians(YawTorque);
    VehicleBody->AddTorqueInRadians(PitchTorque);
    VehicleBody->AddTorqueInRadians(RollTorque);
}

//////////////////////////////////////////////////////////////////////////
// Serialization helpers (optional)

void AOrbitVehicleController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AOrbitVehicleController, ForwardInput);
    DOREPLIFETIME(AOrbitVehicleController, RightInput);
    DOREPLIFETIME(AOrbitVehicleController, YawInput);
    DOREPLIFETIME(AOrbitVehicleController, PitchInput);
    DOREPLIFETIME(AOrbitVehicleController, RollInput);
}

#undef LOCTEXT_NAMESPACE
