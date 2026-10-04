// OrbitVehicleController.cpp
// 3‑D vehicle controller with pitch, yaw, roll, gravity, resistance and acceleration
// ------------------------------------------------------------------------------

#include "OrbitVehicleController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AOrbitVehicleController::AOrbitVehicleController()
{
    PrimaryActorTick.bCanEverTick = true;

    // --- Vehicle body -------------------------------------------------------
    VehicleBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VehicleBody"));
    RootComponent = VehicleBody;
    VehicleBody->SetSimulatePhysics(true);
    VehicleBody->SetEnableGravity(false);   // custom gravity

    // --- Camera (optional) ---------------------------------------------------
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 300.f;
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    // --- Default physics parameters -----------------------------------------
    AccelerationForce      = 5000.f;   // Newtons
    SteeringTorqueStrength = 2000.f;   // N·m
    PitchTorqueStrength    = 1500.f;
    YawTorqueStrength      = 1500.f;
    RollTorqueStrength     = 1500.f;
    GravityStrength        = 980.f;    // m/s² (Earth gravity)
    DragCoefficient        = 0.1f;     // simple linear drag
}

void AOrbitVehicleController::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        PC->bShowMouseCursor = true;
        PC->SetInputMode(FInputModeGameOnly());
    }
}

void AOrbitVehicleController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AOrbitVehicleController::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"),   this, &AOrbitVehicleController::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Pitch"),      this, &AOrbitVehicleController::Pitch);
    PlayerInputComponent->BindAxis(TEXT("Yaw"),        this, &AOrbitVehicleController::Yaw);
    PlayerInputComponent->BindAxis(TEXT("Roll"),       this, &AOrbitVehicleController::Roll);
}

void AOrbitVehicleController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ApplyPhysics(DeltaTime);
}

void AOrbitVehicleController::MoveForward(float Value)
{
    ThrottleInput = FMath::Clamp(Value, -1.f, 1.f);
}

void AOrbitVehicleController::MoveRight(float Value)
{
    SteeringInput = FMath::Clamp(Value, -1.f, 1.f);
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

void AOrbitVehicleController::ApplyPhysics(float DeltaTime)
{
    if (!VehicleBody) return;

    const FVector Forward = VehicleBody->GetForwardVector();
    const FVector Right   = VehicleBody->GetRightVector();
    const FVector Up      = VehicleBody->GetUpVector();

    // 1) Acceleration (throttle)
    const FVector AccelForce = Forward * ThrottleInput * AccelerationForce;
    VehicleBody->AddForce(AccelForce);

    // 2) Steering torque (yaw via right‑axis torque)
    const FVector SteeringTorque = Right * SteeringInput * SteeringTorqueStrength;
    VehicleBody->AddTorqueInRadians(SteeringTorque);

    // 3) Pitch / Yaw / Roll torques
    const FVector PitchTorque = Right * PitchInput * PitchTorqueStrength;
    const FVector YawTorque   = Up   * YawInput   * YawTorqueStrength;
    const FVector RollTorque  = Forward * RollInput * RollTorqueStrength;
    VehicleBody->AddTorqueInRadians(PitchTorque + YawTorque + RollTorque);

    // 4) Custom gravity (downward force)
    const FVector GravityForce = -GravityStrength * VehicleBody->GetMass() * FVector::UpVector;
    VehicleBody->AddForce(GravityForce);

    // 5) Linear resistance / drag
    const FVector Velocity = VehicleBody->GetPhysicsLinearVelocity();
    const FVector DragForce = -DragCoefficient * Velocity;
    VehicleBody->AddForce(DragForce);
}
