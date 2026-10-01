#include "OrbitAtmospherePostProcessBridgeComponent.h"

#include "Components/PostProcessComponent.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/World.h"

UOrbitAtmospherePostProcessBridgeComponent::
UOrbitAtmospherePostProcessBridgeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UOrbitAtmospherePostProcessBridgeComponent::
UpdateAtmosphereAlpha(
    float AltitudeKm,
    float DeltaTime)
{
    const float Range =
        FMath::Max(
            FadeEndAltitudeKm -
            FadeStartAltitudeKm,
            0.01f
        );

    const float TargetAlpha =
        1.0f -
        FMath::Clamp(
            (AltitudeKm -
             FadeStartAltitudeKm) /
            Range,
            0.0f,
            1.0f
        );

    if (DeltaTime > 0.0f)
    {
        AtmosphereAlpha =
            FMath::FInterpTo(
                AtmosphereAlpha,
                TargetAlpha,
                DeltaTime,
                SmoothingSpeed
            );
    }
    else
    {
        AtmosphereAlpha =
            TargetAlpha;
    }

    if (TargetPostProcess)
    {
        TargetPostProcess->BlendWeight =
            AtmosphereAlpha;
    }

    if (ParameterCollection && GetWorld())
    {
        if (!CollectionInstance)
        {
            CollectionInstance =
                GetWorld()
                ->GetParameterCollectionInstance(
                    ParameterCollection
                );
        }

        if (CollectionInstance)
        {
            CollectionInstance
                ->SetScalarParameterValue(
                    AtmosphereAlphaParameter,
                    AtmosphereAlpha
                );
        }
    }
}
