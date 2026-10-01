#include "MarsRoverPawn.h"

AMarsRoverPawn::AMarsRoverPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    RoverBody =
        CreateDefaultSubobject<
            UStaticMeshComponent>(
                TEXT("RoverBody"));

    RootComponent =
        RoverBody;

    RoverBody->SetSimulatePhysics(
        false);
}

void AMarsRoverPawn::BeginPlay()
{
    Super::BeginPlay();
}

void AMarsRoverPawn::SetDriveInput(
    float Throttle,
    float Steering)
{
    ThrottleInput =
        FMath::Clamp(
            Throttle,
            -1.0f,
            1.0f);

    SteeringInput =
        FMath::Clamp(
            Steering,
            -1.0f,
            1.0f);
}

void AMarsRoverPawn::Tick(
    float DeltaSeconds)
{
    Super::Tick(
        DeltaSeconds);

    const FVector Forward =
        GetActorForwardVector();

    const FVector DeltaMove =
        Forward *
        ThrottleInput *
        DriveAcceleration *
        DeltaSeconds;

    const FVector NewLocation =
        GetActorLocation() +
        DeltaMove;

    SetActorLocation(
        NewLocation,
        true);

    const float SpeedScale =
        FMath::Clamp(
            FMath::Abs(
                ThrottleInput),
            0.0f,
            1.0f);

    AddActorLocalRotation(
        FRotator(
            0.0f,
            SteeringInput *
            SteeringRate *
            SpeedScale *
            DeltaSeconds,
            0.0f));
}
