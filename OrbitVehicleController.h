// OrbitVehicleController.h
// ---------------
// UE5 C++ header for a futuristic spaceship controller.
// Handles thruster vectors, speed, and physics integration.
//
//  Author: Principal UE5 C++ Engineer
//  Date:   2026-10-04
//  ------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OrbitVehicleController.generated.h"

/**
 *  A spaceship pawn that uses a set of thrusters to control
 *  its velocity in 3D space.  Each thruster is defined by a
 *  direction vector (in local space) and a maximum thrust
 *  magnitude.  The controller blends the active thrusters
 *  to produce a net force that is applied to the physics
 *  body each frame.
 */
UCLASS()
class ORBIT_API AOrbitVehicleController : public APawn
{
    GENERATED_BODY()

public:
    // ------------------------------------------------------------------
    // Construction & Lifecycle
    // ------------------------------------------------------------------
    AOrbitVehicleController();

    /** Called every frame */
    virtual void Tick(float DeltaTime) override;

    /** Setup player input bindings */
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    /** Called when the pawn is spawned or the level starts */
    virtual void BeginPlay() override;

    /** Called when the pawn is destroyed */
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // ------------------------------------------------------------------
    // Thruster Management
    // ------------------------------------------------------------------
    /** Add a new thruster to the vehicle */
    UFUNCTION(BlueprintCallable, Category = "Thrusters")
    void AddThruster(const FVector& LocalDirection, float MaxThrust);

    /** Remove all thrusters */
    UFUNCTION(BlueprintCallable, Category = "Thrusters")
    void ClearThrusters();

    /** Get the number of active thrusters */
    UFUNCTION(BlueprintPure, Category = "Thrusters")
    int32 GetThrusterCount() const { return Thrusters.Num(); }

    /** Get the local direction of a thruster by index */
    UFUNCTION(BlueprintPure, Category = "Thrusters")
    FVector GetThrusterDirection(int32 Index) const;

    /** Get the maximum thrust of a thruster by index */
    UFUNCTION(BlueprintPure, Category = "Thrusters")
    float GetThrusterMaxThrust(int32 Index) const;

    // ------------------------------------------------------------------
    // Speed Control
    // ------------------------------------------------------------------
    /** Current speed magnitude (m/s) */
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float CurrentSpeed = 0.f;

    /** Maximum allowed speed (m/s) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MaxSpeed = 3000.f;

    /** Acceleration rate (m/s²) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Acceleration = 500.f;

    /** Deceleration rate (m/s²) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Deceleration = 300.f;

    /** Current throttle value (0.0 – 1.0) */
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float Throttle = 0.f;

    /** Current yaw input (-1.0 – 1.0) */
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float YawInput = 0.f;

    /** Current pitch input (-1.0 – 1.0) */
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float PitchInput = 0.f;

    /** Current roll input (-1.0 – 1.0) */
    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float RollInput = 0.f;

    // ------------------------------------------------------------------
    // Components
    // ------------------------------------------------------------------
    /** Root component for the pawn */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* Root;

    /** Visual representation of the spaceship */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    /** Camera for the player */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* Camera;

private:
    /** Internal struct representing a thruster */
    struct FThruster
    {
        FVector LocalDirection;   // Normalized local-space direction
        float MaxThrust;          // Maximum force (N) this thruster can produce
    };

    /** All thrusters attached to this vehicle */
    TArray<FThruster> Thrusters;

    /** Helper to apply physics forces each tick */
    void ApplyThrusterForces(float DeltaTime);

    /** Helper to update speed based on throttle */
    void UpdateSpeed(float DeltaTime);

    /** Helper to rotate the vehicle based on input */
    void ApplyRotation(float DeltaTime);

    /** Cached physics handle for the mesh */
    UPrimitiveComponent* PhysicsComponent = nullptr;
};
