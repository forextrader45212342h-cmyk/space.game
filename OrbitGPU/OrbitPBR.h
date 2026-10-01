#pragma once

#include <cstdint>

namespace OrbitPBR
{
    struct Vec3
    {
        float x = 0;
        float y = 0;
        float z = 0;
    };

    struct Color
    {
        float r = 1;
        float g = 1;
        float b = 1;
        float a = 1;
    };

    struct Material
    {
        Color baseColor{1,1,1,1};

        float metallic = 0.0f;
        float roughness = 0.5f;
        float specular = 0.5f;

        float normalStrength = 1.0f;
        float emissiveStrength = 0.0f;

        uint64_t baseColorTexture = 0;
        uint64_t normalTexture = 0;
        uint64_t metallicRoughnessTexture = 0;
        uint64_t emissiveTexture = 0;
    };

    struct SurfaceOutput
    {
        Color color;
        float roughness = 0;
        float metallic = 0;
    };

    class MaterialSystem
    {
    private:
        Material material{};

    public:
        void SetMaterial(const Material& value);
        const Material& GetMaterial() const;

        SurfaceOutput Evaluate(
            const Color& albedo,
            float metallic,
            float roughness) const;
    };
}
