#include "OrbitLODSystem.h"
#include "OrbitMath.h"

namespace Orbit
{
    void OrbitLODSystem::SetDistances(
        double lod0,
        double lod1,
        double lod2,
        double lod3)
    {
        Distances[0] = lod0;
        Distances[1] = lod1;
        Distances[2] = lod2;
        Distances[3] = lod3;
    }

    LODResult OrbitLODSystem::Calculate(
        const Vector3& objectPosition,
        const Vector3& cameraPosition,
        double objectRadius) const
    {
        LODResult result;

        result.Distance =
            OrbitMath::Distance(
                objectPosition,
                cameraPosition);

        result.ScreenScale =
            objectRadius /
            (result.Distance + 1.0);

        if (result.Distance <= Distances[0])
            result.Level = 0;
        else if (result.Distance <= Distances[1])
            result.Level = 1;
        else if (result.Distance <= Distances[2])
            result.Level = 2;
        else
            result.Level = 3;

        return result;
    }
}
