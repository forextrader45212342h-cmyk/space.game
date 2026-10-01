#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpacecraftPawn.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UPlanetGravityComponent;

UENUM(BlueprintType)
enum class EFlightMode : uint8
{
    Atmospheric,
    Orbital,
    DeepSpace
};

UCLASS()
class PROJECTORBIT_API ASpacecraftPawn : public APawn
{
    GENERATED_BODY()

public:

    ASpacecraftPawn();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    virtual void SetupPlayerInputComponent(
        UInputComponent* PlayerInputComponent
    ) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* ShipMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UPlanetGravityComponent* GravityComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
    float MaxThrust = 2500000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
    float PitchTorque = 1500000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
    float YawTorque = 1200000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
    float RollTorque = 900000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
    float AtmosphericDrag = 0.25f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
    float SpaceDrag = 0.005f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
    float AtmosphericLimit = 100000.0f;

    UPROPERTY(BlueprintReadOnly)
    float Throttle = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float PitchInput = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float YawInput = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float RollInput = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    EFlightMode FlightMode =
        EFlightMode::Atmospheric;

    UFUNCTION(BlueprintCallable)
    void SetThrottle(float Value);

    UFUNCTION(BlueprintCallable)
    void SetPitch(float Value);

    UFUNCTION(BlueprintCallable)
    void SetYaw(float Value);

    UFUNCTION(BlueprintCallable)
    void SetRoll(float Value);

protected:

    void ApplyFlightForces(
        float DeltaSeconds
    );

    void UpdateFlightMode();

    void ApplyFlightTorque();

    void ApplyDrag();

    void UpdateCamera();

private:

    FVector SmoothedVelocity =
        FVector::ZeroVector;
};
