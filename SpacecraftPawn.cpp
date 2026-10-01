#include "Flight/SpacecraftPawn.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Physics/PlanetGravityComponent.h"

ASpacecraftPawn::ASpacecraftPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    ShipMesh =
        CreateDefaultSubobject<
            UStaticMeshComponent
        >(TEXT("ShipMesh"));

    RootComponent = ShipMesh;

    ShipMesh->SetSimulatePhysics(true);
    ShipMesh->SetEnableGravity(false);

    ShipMesh->SetLinearDamping(0.02f);
    ShipMesh->SetAngularDamping(0.4f);

    CameraBoom =
        CreateDefaultSubobject<
            USpringArmComponent
        >(TEXT("CameraBoom"));

    CameraBoom->SetupAttachment(
        RootComponent
    );

    CameraBoom->TargetArmLength = 900.0f;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 7.0f;

    Camera =
        CreateDefaultSubobject<
            UCameraComponent
        >(TEXT("Camera"));

    Camera->SetupAttachment(
        CameraBoom
    );

    GravityComponent =
        CreateDefaultSubobject<
            UPlanetGravityComponent
        >(TEXT("GravityComponent"));
}

void ASpacecraftPawn::BeginPlay()
{
    Super::BeginPlay();

    ShipMesh->WakeAllRigidBodies();
}

void ASpacecraftPawn::Tick(
    float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    UpdateFlightMode();

    ApplyFlightForces(
        DeltaSeconds
    );

    ApplyFlightTorque();

    ApplyDrag();

    UpdateCamera();
}

void ASpacecraftPawn::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(
        PlayerInputComponent
    );

    PlayerInputComponent->BindAxis(
        TEXT("Throttle"),
        this,
        &ASpacecraftPawn::SetThrottle
    );

    PlayerInputComponent->BindAxis(
        TEXT("Pitch"),
        this,
        &ASpacecraftPawn::SetPitch
    );

    PlayerInputComponent->BindAxis(
        TEXT("Yaw"),
        this,
        &ASpacecraftPawn::SetYaw
    );

    PlayerInputComponent->BindAxis(
        TEXT("Roll"),
        this,
        &ASpacecraftPawn::SetRoll
    );
}

void ASpacecraftPawn::SetThrottle(
    float Value)
{
    Throttle =
        FMath::Clamp(
            Value,
            0.0f,
            1.0f
        );
}

void ASpacecraftPawn::SetPitch(
    float Value)
{
    PitchInput =
        FMath::Clamp(
            Value,
            -1.0f,
            1.0f
        );
}

void ASpacecraftPawn::SetYaw(
    float Value)
{
    YawInput =
        FMath::Clamp(
            Value,
            -1.0f,
            1.0f
        );
}

void ASpacecraftPawn::SetRoll(
    float Value)
{
    RollInput =
        FMath::Clamp(
            Value,
            -1.0f,
            1.0f
        );
}

void ASpacecraftPawn::ApplyFlightForces(
    float DeltaSeconds)
{
    if (!ShipMesh)
    {
        return;
    }

    const FVector Forward =
        GetActorForwardVector();

    const FVector ThrustForce =
        Forward *
        MaxThrust *
        Throttle;

    ShipMesh->AddForce(
        ThrustForce,
        NAME_None,
        true
    );
}

void ASpacecraftPawn::ApplyFlightTorque()
{
    if (!ShipMesh)
    {
        return;
    }

    const FVector LocalTorque(
        PitchInput * PitchTorque,
        YawInput   * YawTorque,
        RollInput  * RollTorque
    );

    ShipMesh->AddTorqueInRadians(
        LocalTorque,
        NAME_None,
        true
    );
}

void ASpacecraftPawn::ApplyDrag()
{
    if (!ShipMesh)
    {
        return;
    }

    const FVector Velocity =
        ShipMesh->GetPhysicsLinearVelocity();

    const double Speed =
        Velocity.Size();

    if (Speed < 1.0)
    {
        return;
    }

    const double Altitude =
        GravityComponent
        ? GravityComponent->GetAltitude()
        : 0.0;

    const float DragCoefficient =
        Altitude <
        AtmosphericLimit
        ? AtmosphericDrag
        : SpaceDrag;

    const FVector Drag =
        -Velocity.GetSafeNormal() *
        Velocity.SizeSquared() *
        DragCoefficient;

    ShipMesh->AddForce(
        Drag,
        NAME_None,
        true
    );
}

void ASpacecraftPawn::UpdateFlightMode()
{
    const double Altitude =
        GravityComponent
        ? GravityComponent->GetAltitude()
        : 0.0;

    if (Altitude <
        AtmosphericLimit)
    {
        FlightMode =
            EFlightMode::Atmospheric;
    }
    else if (
        Altitude < 1000000.0)
    {
        FlightMode =
            EFlightMode::Orbital;
    }
    else
    {
        FlightMode =
            EFlightMode::DeepSpace;
    }
}

void ASpacecraftPawn::UpdateCamera()
{
    if (!ShipMesh || !Camera)
    {
        return;
    }

    const FVector Velocity =
        ShipMesh->GetPhysicsLinearVelocity();

    const double Speed =
        Velocity.Size();

    const float TargetFOV =
        FMath::Clamp(
            85.0f +
            static_cast<float>(
                Speed * 0.00002
            ),
            85.0f,
            110.0f
        );

    Camera->SetFieldOfView(
        FMath::FInterpTo(
            Camera->FieldOfView,
            TargetFOV,
            GetWorld()->GetDeltaSeconds(),
            3.0f
        )
    );
}
