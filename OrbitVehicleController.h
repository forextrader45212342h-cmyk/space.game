// OrbitVehicleController.h
// ---------------
// UE5 C++ header for a futuristic spaceship controller.
// Handles thruster vectors, speed, and basic physics integration.
// -----------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OrbitVehicleController.generated.h"

/**
 *  OrbitVehicleController
 *
 *  A lightweight spaceship controller that exposes thruster vectors
 *  (forward, right, up) and a speed multiplier.  The controller
 *  applies forces to the owning pawn's root component each tick.
 *
 *  The class is intentionally header‑only for quick prototyping.
 *  In a production build you would move the implementation to a .cpp
 *  file and expose only the interface.
 */
UCLASS()
class ORBIT_API AOrbitVehicleController : public APawn
{
    GENERATED_BODY()

public:
    // -----------------------------------------------------------------
    // Construction / Lifecycle
    // -----------------------------------------------------------------
    AOrbitVehicleController();

    /** Called every frame.  Applies thruster forces. */
    virtual void Tick(float DeltaTime) override;

    /** Called when the game starts or when spawned. */
    virtual void BeginPlay() override;

    /** Called when the pawn is destroyed. */
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // -----------------------------------------------------------------
    // Thruster API
    // -----------------------------------------------------------------
    /** Sets the forward thruster vector (local space). */
    UFUNCTION(BlueprintCallable, Category = "Thrusters")
    void SetForwardThruster(const FVector& InVector);

    /** Sets the right thruster vector (local space). */
    UFUNCTION(BlueprintCallable, Category = "Thrusters")
    void SetRightThruster(const FVector& InVector);

    /** Sets the up thruster vector (local space). */
    UFUNCTION(BlueprintCallable, Category = "Thrusters")
    void SetUpThruster(const FVector& InVector);

    /** Sets the global speed multiplier (0 = idle, 1 = full thrust). */
    UFUNCTION(BlueprintCallable, Category = "Thrusters")
    void SetSpeedMultiplier(float InMultiplier);

    /** Returns the current speed multiplier. */
    UFUNCTION(BlueprintPure, Category = "Thrusters")
    float GetSpeedMultiplier() const { return SpeedMultiplier; }

    // -----------------------------------------------------------------
    // Input helpers (optional)
    // -----------------------------------------------------------------
    /** Bind input actions to thruster controls. */
    void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    /** Root component that receives physics forces. */
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* RootMesh;

    /** Forward thruster vector (local space). */
    UPROPERTY(EditAnywhere, Category = "Thrusters")
    FVector ForwardThruster = FVector::ForwardVector;

    /** Right thruster vector (local space). */
    UPROPERTY(EditAnywhere, Category = "Thrusters")
    FVector RightThruster = FVector::RightVector;

    /** Up thruster vector (local space). */
    UPROPERTY(EditAnywhere, Category = "Thrusters")
    FVector UpThruster = FVector::UpVector;

    /** Global speed multiplier (0–1). */
    UPROPERTY(EditAnywhere, Category = "Thrusters")
    float SpeedMultiplier = 1.0f;

    /** Cached world transform for efficient force application. */
    FTransform CachedTransform;

    /** Helper to apply a single thruster force. */
    void ApplyThruster(const FVector& LocalVector, float DeltaTime);
};
