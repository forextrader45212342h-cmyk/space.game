#include "OrbitInputBridgeComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"

UOrbitInputBridgeComponent::UOrbitInputBridgeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UOrbitInputBridgeComponent::InstallMappingContext(
    APlayerController* PlayerController,
    int32 Priority)
{
    if (!PlayerController || !MappingContext)
        return;

    if (ULocalPlayer* LP =
        PlayerController->GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            Subsystem->AddMappingContext(
                MappingContext,
                Priority
            );
        }
    }
}

void UOrbitInputBridgeComponent::BindEnhancedInput(
    UEnhancedInputComponent* EnhancedInputComponent)
{
    if (!EnhancedInputComponent)
        return;

    if (ThrottleAction)
    {
        EnhancedInputComponent->BindAction(
            ThrottleAction,
            ETriggerEvent::Triggered,
            this,
            &UOrbitInputBridgeComponent::HandleThrottle
        );
    }

    if (BrakeAction)
    {
        EnhancedInputComponent->BindAction(
            BrakeAction,
            ETriggerEvent::Triggered,
            this,
            &UOrbitInputBridgeComponent::HandleBrake
        );
    }

    if (SteerAction)
    {
        EnhancedInputComponent->BindAction(
            SteerAction,
            ETriggerEvent::Triggered,
            this,
            &UOrbitInputBridgeComponent::HandleSteer
        );
    }

    if (BoostAction)
    {
        EnhancedInputComponent->BindAction(
            BoostAction,
            ETriggerEvent::Started,
            this,
            &UOrbitInputBridgeComponent::HandleBoost
        );
    }

    if (ExitVehicleAction)
    {
        EnhancedInputComponent->BindAction(
            ExitVehicleAction,
            ETriggerEvent::Started,
            this,
            &UOrbitInputBridgeComponent::HandleExit
        );
    }
}

void UOrbitInputBridgeComponent::HandleThrottle(
    const FInputActionValue& Value)
{
    OnThrottle.Broadcast(
        FMath::Clamp(
            Value.Get<float>(),
            -1.0f,
            1.0f
        )
    );
}

void UOrbitInputBridgeComponent::HandleBrake(
    const FInputActionValue& Value)
{
    OnBrake.Broadcast(
        FMath::Clamp(
            Value.Get<float>(),
            0.0f,
            1.0f
        )
    );
}

void UOrbitInputBridgeComponent::HandleSteer(
    const FInputActionValue& Value)
{
    OnSteer.Broadcast(
        FMath::Clamp(
            Value.Get<float>(),
            -1.0f,
            1.0f
        )
    );
}

void UOrbitInputBridgeComponent::HandleBoost(
    const FInputActionValue& Value)
{
    OnBoostPressed.Broadcast();
}

void UOrbitInputBridgeComponent::HandleExit(
    const FInputActionValue& Value)
{
    OnExitVehicle.Broadcast();
}
