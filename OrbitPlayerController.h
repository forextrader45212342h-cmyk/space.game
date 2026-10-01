#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OrbitPlayerController.generated.h"

UCLASS()
class PROJECTORBIT_API AOrbitPlayerController : public APlayerController
{
    GENERATED_BODY()

public:

    AOrbitPlayerController();

protected:

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:

    void MoveForward(float Value);
    void MoveRight(float Value);
    void MoveUp(float Value);

    void LookHorizontal(float Value);
    void LookVertical(float Value);

public:

    UFUNCTION(BlueprintCallable, Category="Orbit|Controls")
    void EnableOrbitControls();

    UFUNCTION(BlueprintCallable, Category="Orbit|Controls")
    void DisableOrbitControls();

private:

    bool bOrbitControlsEnabled;
};
