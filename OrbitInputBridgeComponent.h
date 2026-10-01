#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "OrbitInputBridgeComponent.generated.h"

class UInputAction;
class UInputMappingContext;
class UEnhancedInputComponent;
class APlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOrbitFloatInputEvent,
    float,
    Value
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
    FOrbitActionInputEvent
);

UCLASS(ClassGroup=(ProjectOrbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitInputBridgeComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:

    UOrbitInputBridgeComponent();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputMappingContext> MappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> ThrottleAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> BrakeAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> SteerAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> BoostAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
    TObjectPtr<UInputAction> ExitVehicleAction;

    UPROPERTY(BlueprintAssignable, Category="Input")
    FOrbitFloatInputEvent OnThrottle;

    UPROPERTY(BlueprintAssignable, Category="Input")
    FOrbitFloatInputEvent OnBrake;

    UPROPERTY(BlueprintAssignable, Category="Input")
    FOrbitFloatInputEvent OnSteer;

    UPROPERTY(BlueprintAssignable, Category="Input")
    FOrbitActionInputEvent OnBoostPressed;

    UPROPERTY(BlueprintAssignable, Category="Input")
    FOrbitActionInputEvent OnExitVehicle;

    UFUNCTION(BlueprintCallable, Category="Input")
    void InstallMappingContext(
        APlayerController* PlayerController,
        int32 Priority = 10
    );

    UFUNCTION(BlueprintCallable, Category="Input")
    void BindEnhancedInput(
        UEnhancedInputComponent* EnhancedInputComponent
    );

private:

    void HandleThrottle(const FInputActionValue& Value);
    void HandleBrake(const FInputActionValue& Value);
    void HandleSteer(const FInputActionValue& Value);
    void HandleBoost(const FInputActionValue& Value);
    void HandleExit(const FInputActionValue& Value);
};
