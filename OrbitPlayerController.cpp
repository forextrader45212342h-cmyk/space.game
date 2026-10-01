#include "OrbitPlayerController.h"

AOrbitPlayerController::AOrbitPlayerController()
{
    bOrbitControlsEnabled = true;

    bShowMouseCursor = false;
}

void AOrbitPlayerController::BeginPlay()
{
    Super::BeginPlay();

    EnableOrbitControls();
}

void AOrbitPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!InputComponent)
    {
        return;
    }

    InputComponent->BindAxis(
        TEXT("MoveForward"),
        this,
        &AOrbitPlayerController::MoveForward
    );

    InputComponent->BindAxis(
        TEXT("MoveRight"),
        this,
        &AOrbitPlayerController::MoveRight
    );

    InputComponent->BindAxis(
        TEXT("MoveUp"),
        this,
        &AOrbitPlayerController::MoveUp
    );

    InputComponent->BindAxis(
        TEXT("LookHorizontal"),
        this,
        &AOrbitPlayerController::LookHorizontal
    );

    InputComponent->BindAxis(
        TEXT("LookVertical"),
        this,
        &AOrbitPlayerController::LookVertical
    );
}

void AOrbitPlayerController::MoveForward(float Value)
{
    if (!bOrbitControlsEnabled || !GetPawn())
    {
        return;
    }

    GetPawn()->AddMovementInput(
        GetPawn()->GetActorForwardVector(),
        Value
    );
}

void AOrbitPlayerController::MoveRight(float Value)
{
    if (!bOrbitControlsEnabled || !GetPawn())
    {
        return;
    }

    GetPawn()->AddMovementInput(
        GetPawn()->GetActorRightVector(),
        Value
    );
}

void AOrbitPlayerController::MoveUp(float Value)
{
    if (!bOrbitControlsEnabled || !GetPawn())
    {
        return;
    }

    GetPawn()->AddMovementInput(
        GetPawn()->GetActorUpVector(),
        Value
    );
}

void AOrbitPlayerController::LookHorizontal(float Value)
{
    if (!bOrbitControlsEnabled)
    {
        return;
    }

    AddYawInput(Value);
}

void AOrbitPlayerController::LookVertical(float Value)
{
    if (!bOrbitControlsEnabled)
    {
        return;
    }

    AddPitchInput(Value);
}

void AOrbitPlayerController::EnableOrbitControls()
{
    bOrbitControlsEnabled = true;
}

void AOrbitPlayerController::DisableOrbitControls()
{
    bOrbitControlsEnabled = false;
}
