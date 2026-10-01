#include "OrbitPBR.h"

namespace OrbitPBR
{
    void MaterialSystem::SetMaterial(const Material& value)
    {
        material = value;

        if (material.roughness < 0.04f)
            material.roughness = 0.04f;

        if (material.roughness > 1.0f)
            material.roughness = 1.0f;

        if (material.metallic < 0.0f)
            material.metallic = 0.0f;

        if (material.metallic > 1.0f)
            material.metallic = 1.0f;
    }

    const Material& MaterialSystem::GetMaterial() const
    {
        return material;
    }

    SurfaceOutput MaterialSystem::Evaluate(
        const Color& albedo,
        float metallic,
        float roughness) const
    {
        SurfaceOutput result;

        result.color = albedo;
        result.metallic = metallic;
        result.roughness = roughness;

        if (result.metallic < 0)
            result.metallic = 0;

        if (result.metallic > 1)
            result.metallic = 1;

        if (result.roughness < 0.04f)
            result.roughness = 0.04f;

        if (result.roughness > 1)
            result.roughness = 1;

        return result;
    }
}
