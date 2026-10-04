// OrbitVehicleController.cpp
// 3‑D pitch / yaw / roll physics controller with gravity compensation, drag and acceleration

#include "OrbitVehicleController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// AOrbitVehicleController

AOrbitVehicleController::AOrbitVehicleController()
{
    PrimaryActorTick.bCanEverTick = true;

    // Root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Vehicle body
    VehicleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleMesh"));
    VehicleMesh->SetupAttachment(RootComponent);
    VehicleMesh->SetSimulatePhysics(true);
    VehicleMesh->SetEnableGravity(false); // we handle gravity manually
    VehicleMesh->SetMassOverrideInKg(NAME_None, VehicleMass);

    // Spring arm for camera
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(VehicleMesh);
    SpringArm->TargetArmLength = 300.f;
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 3.f;

    // Camera
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    // Input flags
    bIsAccelerating = false;
    bIsBraking = false;
    bIsTurningLeft = false;
    bIsTurningRight = false;
    bIsPitchUp = false;
    bIsPitchDown = false;
    bIsRollLeft = false;
    bIsRollRight = false;
}

void AOrbitVehicleController::BeginPlay()
{
    Super::BeginPlay();

    // Ensure physics is enabled
    VehicleMesh->SetSimulatePhysics(true);
}

void AOrbitVehicleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 1. Gravity compensation
    FVector GravityForce = FVector(0.f, 0.f, -VehicleMass * GravityScale);
    VehicleMesh->AddForce(GravityForce, NAME_None, true);

    // 2. Drag (linear)
    FVector Velocity = VehicleMesh->GetPhysicsLinearVelocity();
    FVector DragForce = -DragCoefficient * Velocity;
    VehicleMesh->AddForce(DragForce, NAME_None, true);

    // 3. Acceleration / braking
    FVector Forward = VehicleMesh->GetForwardVector();
    if (bIsAccelerating)
    {
        FVector AccelForce = Forward * Acceleration * VehicleMass;
        VehicleMesh->AddForce(AccelForce, NAME_None, true);
    }
    if (bIsBraking)
    {
        FVector BrakeForce = -Forward * Acceleration * VehicleMass;
        VehicleMesh->AddForce(BrakeForce, NAME_None, true);
    }

    // 4. Turning (yaw)
    if (bIsTurningLeft)
    {
        FVector Torque = FVector(0.f, 0.f, -TurnTorque);
        VehicleMesh->AddTorqueInRadians(Torque, NAME_None, true);
    }
    if (bIsTurningRight)
    {
        FVector Torque = FVector(0.f, 0.f, TurnTorque);
        VehicleMesh->AddTorqueInRadians(Torque, NAME_None, true);
    }

    // 5. Pitch
    if (bIsPitchUp)
    {
        FVector Torque = VehicleMesh->GetRightVector() * -PitchTorque;
        VehicleMesh->AddTorqueInRadians(Torque, NAME_None, true);
    }
    if (bIsPitchDown)
    {
        FVector Torque = VehicleMesh->GetRightVector() * PitchTorque;
        VehicleMesh->AddTorqueInRadians(Torque, NAME_None, true);
    }

    // 6. Roll
    if (bIsRollLeft)
    {
        FVector Torque = VehicleMesh->GetForwardVector() * -RollTorque;
        VehicleMesh->AddTorqueInRadians(Torque, NAME_None, true);
    }
    if (bIsRollRight)
    {
        FVector Torque = VehicleMesh->GetForwardVector() * RollTorque;
        VehicleMesh->AddTorqueInRadians(Torque, NAME_None, true);
    }

    // 7. Clamp speed
    float CurrentSpeed = Velocity.Size();
    if (CurrentSpeed > MaxSpeed)
    {
        FVector NewVelocity = Velocity.GetSafeNormal() * MaxSpeed;
        VehicleMesh->SetPhysicsLinearVelocity(NewVelocity, true);
    }
}

//////////////////////////////////////////////////////////////////////////
// Input binding

void AOrbitVehicleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Movement
    PlayerInputComponent->BindAction("Accelerate", IE_Pressed, this, &AOrbitVehicleController::StartAccelerate);
    PlayerInputComponent->BindAction("Accelerate", IE_Released, this, &AOrbitVehicleController::StopAccelerate);

    PlayerInputComponent->BindAction("Brake", IE_Pressed, this, &AOrbitVehicleController::StartBrake);
    PlayerInputComponent->BindAction("Brake", IE_Released, this, &AOrbitVehicleController::StopBrake);

    // Turning
    PlayerInputComponent->BindAction("TurnLeft", IE_Pressed, this, &AOrbitVehicleController::StartTurnLeft);
    PlayerInputComponent->BindAction("TurnLeft", IE_Released, this, &AOrbitVehicleController::StopTurnLeft);

    PlayerInputComponent->BindAction("TurnRight", IE_Pressed, this, &AOrbitVehicleController::StartTurnRight);
    PlayerInputComponent->BindAction("TurnRight", IE_Released, this, &AOrbitVehicleController::StopTurnRight);

    // Pitch
    PlayerInputComponent->BindAction("PitchUp", IE_Pressed, this, &AOrbitVehicleController::StartPitchUp);
    PlayerInputComponent->BindAction("PitchUp", IE_Released, this, &AOrbitVehicleController::StopPitchUp);

    PlayerInputComponent->BindAction("PitchDown", IE_Pressed, this, &AOrbitVehicleController::StartPitchDown);
    PlayerInputComponent->BindAction("PitchDown", IE_Released, this, &AOrbitVehicleController::StopPitchDown);

    // Roll
    PlayerInputComponent->BindAction("RollLeft", IE_Pressed, this, &AOrbitVehicleController::StartRollLeft);
    PlayerInputComponent->BindAction("RollLeft", IE_Released, this, &AOrbitVehicleController::StopRollLeft);

    PlayerInputComponent->BindAction("RollRight", IE_Pressed, this, &AOrbitVehicleController::StartRollRight);
    PlayerInputComponent->BindAction("RollRight", IE_Released, this, &AOrbitVehicleController::StopRollRight);
}

//////////////////////////////////////////////////////////////////////////
// Input handlers

void AOrbitVehicleController::StartAccelerate() { bIsAccelerating = true; }
void AOrbitVehicleController::StopAccelerate()  { bIsAccelerating = false; }

void AOrbitVehicleController::StartBrake() { bIsBraking = true; }
void AOrbitVehicleController::StopBrake()  { bIsBraking = false; }

void AOrbitVehicleController::StartTurnLeft()  { bIsTurningLeft = true; }
void AOrbitVehicleController::StopTurnLeft()   { bIsTurningLeft = false; }

void AOrbitVehicleController::StartTurnRight() { bIsTurningRight = true; }
void AOrbitVehicleController::StopTurnRight()  { bIsTurningRight = false; }

void AOrbitVehicleController::StartPitchUp()   { bIsPitchUp = true; }
void AOrbitVehicleController::StopPitchUp()    { bIsPitchUp = false; }

void AOrbitVehicleController::StartPitchDown() { bIsPitchDown = true; }
void AOrbitVehicleController::StopPitchDown()  { bIsPitchDown = false; }

void AOrbitVehicleController::StartRollLeft()  { bIsRollLeft = true; }
void AOrbitVehicleController::StopRollLeft()   { bIsRollLeft = false; }

void AOrbitVehicleController::StartRollRight() { bIsRollRight = true; }
void AOrbitVehicleController::StopRollRight()  { bIsRollRight = false; }
