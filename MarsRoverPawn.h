#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MarsRoverPawn.generated.h"

UCLASS()
class PROJECTORBIT_API AMarsRoverPawn
    : public APawn
{
    GENERATED_BODY()

public:
    AMarsRoverPawn();

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent>
        RoverBody;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Rover")
    float DriveAcceleration =
        900.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Rover")
    float MaxSpeed =
        1200.0f;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category="Rover")
    float SteeringRate =
        45.0f;

    UFUNCTION(BlueprintCallable, Category="Rover")
    void SetDriveInput(
        float Throttle,
        float Steering);

protected:
    virtual void Tick(
        float DeltaSeconds) override;

    virtual void BeginPlay() override;

private:
    float ThrottleInput = 0.0f;

    float SteeringInput = 0.0f;
};
