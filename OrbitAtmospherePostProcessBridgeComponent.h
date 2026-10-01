#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitAtmospherePostProcessBridgeComponent.generated.h"

class UPostProcessComponent;
class UMaterialParameterCollection;
class UMaterialParameterCollectionInstance;

UCLASS(ClassGroup=(ProjectOrbit), meta=(BlueprintSpawnableComponent))
class PROJECTORBIT_API UOrbitAtmospherePostProcessBridgeComponent
    : public UActorComponent
{
    GENERATED_BODY()

public:

    UOrbitAtmospherePostProcessBridgeComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Atmosphere")
    TObjectPtr<UPostProcessComponent> TargetPostProcess;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Atmosphere")
    TObjectPtr<UMaterialParameterCollection> ParameterCollection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Atmosphere")
    FName AtmosphereAlphaParameter =
        TEXT("AtmosphereAlpha");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Atmosphere")
    float FadeStartAltitudeKm = 80.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Atmosphere")
    float FadeEndAltitudeKm = 140.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Atmosphere")
    float SmoothingSpeed = 4.0f;

    UPROPERTY(BlueprintReadOnly, Category="Atmosphere")
    float AtmosphereAlpha = 1.0f;

    UFUNCTION(BlueprintCallable, Category="Atmosphere")
    void UpdateAtmosphereAlpha(
        float AltitudeKm,
        float DeltaTime
    );

private:

    TObjectPtr<UMaterialParameterCollectionInstance>
        CollectionInstance;
};
