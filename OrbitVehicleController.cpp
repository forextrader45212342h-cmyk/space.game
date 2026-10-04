// OrbitVehicleController.cpp
// 3‑D pitch / yaw / roll vehicle controller with physics, gravity, resistance and acceleration
// UE5 C++ implementation

#include "OrbitVehicleController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// Constructor

AOrbitVehicleController::AOrbitVehicleController()
{
    // Enable ticking every frame
    PrimaryActorTick.bCanEverTick = true;

    // Root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Vehicle body
    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
    BodyMesh->SetupAttachment(RootComponent);
    BodyMesh->SetSimulatePhysics(true);
    BodyMesh->SetEnableGravity(true);
    BodyMesh->SetLinearDamping(0.0f);   // We'll apply custom drag
    BodyMesh->SetAngularDamping(0.0f);  // We'll apply custom torque damping

    // Parameters
    MaxEngineForce = 5000.0f;          // Newtons
    MaxBrakeForce = 8000.0f;           // Newtons
    MaxSteerTorque = 2000.0f;          // N·m
    DragCoefficient = 0.1f;           // Linear drag
    AngularDragCoefficient = 0.05f;   // Angular drag
    bUseCustomGravity = true;
    CustomGravity = FVector(0.0f, 0.0f, -980.0f); // cm/s²
}

//////////////////////////////////////////////////////////////////////////
// BeginPlay

void AOrbitVehicleController::BeginPlay()
{
    Super::BeginPlay();

    // Ensure physics is enabled
    if (!BodyMesh->IsSimulatingPhysics())
    {
        BodyMesh->SetSimulatePhysics(true);
    }

    // Disable default gravity if using custom gravity
    if (bUseCustomGravity)
    {
        BodyMesh->SetEnableGravity(false);
    }
}

//////////////////////////////////////////////////////////////////////////
// Tick

void AOrbitVehicleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Apply custom gravity if enabled
    if (bUseCustomGravity)
    {
        BodyMesh->AddForce(CustomGravity * BodyMesh->GetMass());
    }

    // Apply linear drag (resistance)
    FVector Velocity = BodyMesh->GetPhysicsLinearVelocity();
    FVector DragForce = -DragCoefficient * Velocity;
    BodyMesh->AddForce(DragForce);

    // Apply angular drag (resistance)
    FVector AngularVelocity = BodyMesh->GetPhysicsAngularVelocityInRadians();
    FVector AngularDragTorque = -AngularDragCoefficient * AngularVelocity;
    BodyMesh->AddTorqueInRadians(AngularDragTorque);

    // Clamp velocity to avoid runaway speeds
    const float MaxSpeed = 2000.0f; // cm/s
    if (Velocity.Size() > MaxSpeed)
    {
        BodyMesh->SetPhysicsLinearVelocity(Velocity.GetClampedToMaxSize(MaxSpeed));
    }

    // Optional: Visual debugging
    // DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Velocity, FColor::Green, false, -1, 0, 2.0f);
}

//////////////////////////////////////////////////////////////////////////
// Input binding

void AOrbitVehicleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Movement
    PlayerInputComponent->BindAxis("MoveForward", this, &AOrbitVehicleController::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AOrbitVehicleController::MoveRight);

    // Rotation
    PlayerInputComponent->BindAxis("Turn", this, &AOrbitVehicleController::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &AOrbitVehicleController::LookUp);
    PlayerInputComponent->BindAxis("Roll", this, &AOrbitVehicleController::Roll);

    // Braking
    PlayerInputComponent->BindAction("Brake", IE_Pressed, this, &AOrbitVehicleController::BrakePressed);
    PlayerInputComponent->BindAction("Brake", IE_Released, this, &AOrbitVehicleController::BrakeReleased);
}

//////////////////////////////////////////////////////////////////////////
// Movement

void AOrbitVehicleController::MoveForward(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    // Forward vector in world space
    FVector Force = GetActorForwardVector() * Value * MaxEngineForce;
    BodyMesh->AddForce(Force);
}

void AOrbitVehicleController::MoveRight(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    // Right vector in world space
    FVector Force = GetActorRightVector() * Value * MaxEngineForce;
    BodyMesh->AddForce(Force);
}

//////////////////////////////////////////////////////////////////////////
// Rotation

void AOrbitVehicleController::Turn(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    // Yaw torque around world Z
    FVector Torque = FVector(0.0f, 0.0f, Value * MaxSteerTorque);
    BodyMesh->AddTorqueInRadians(Torque);
}

void AOrbitVehicleController::LookUp(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    // Pitch torque around world Y
    FVector Torque = FVector(0.0f, Value * MaxSteerTorque, 0.0f);
    BodyMesh->AddTorqueInRadians(Torque);
}

void AOrbitVehicleController::Roll(float Value)
{
    if (FMath::IsNearlyZero(Value)) return;

    // Roll torque around world X
    FVector Torque = FVector(Value * MaxSteerTorque, 0.0f, 0.0f);
    BodyMesh->AddTorqueInRadians(Torque);
}

//////////////////////////////////////////////////////////////////////////
// Braking

void AOrbitVehicleController::BrakePressed()
{
    bBraking = true;
}

void AOrbitVehicleController::BrakeReleased()
{
    bBraking = false;
}

void AOrbitVehicleController::ApplyBrake(float DeltaTime)
{
    if (!bBraking) return;

    FVector Velocity = BodyMesh->GetPhysicsLinearVelocity();
    FVector BrakeForce = -FMath::Clamp(Velocity.Size(), 0.0f, MaxBrakeForce) * Velocity.GetSafeNormal();
    BodyMesh->AddForce(BrakeForce);
}

//////////////////////////////////////////////////////////////////////////
// Helper: Called every tick to apply brake if needed

void AOrbitVehicleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Apply custom gravity
    if (bUseCustomGravity)
    {
        BodyMesh->AddForce(CustomGravity * BodyMesh->GetMass());
    }

    // Apply linear drag
    FVector Velocity = BodyMesh->GetPhysicsLinearVelocity();
    FVector DragForce = -DragCoefficient * Velocity;
    BodyMesh->AddForce(DragForce);

    // Apply angular drag
    FVector AngularVelocity = BodyMesh->GetPhysicsAngularVelocityInRadians();
    FVector AngularDragTorque = -AngularDragCoefficient * AngularVelocity;
    BodyMesh->AddTorqueInRadians(AngularDragTorque);

    // Apply brake
    ApplyBrake(DeltaTime);

    // Clamp speed
    const float MaxSpeed = 2000.0f; // cm/s
    if (Velocity.Size() > MaxSpeed)
    {
        BodyMesh->SetPhysicsLinearVelocity(Velocity.GetClampedToMaxSize(MaxSpeed));
    }
}
