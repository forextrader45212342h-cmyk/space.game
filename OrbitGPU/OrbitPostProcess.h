#pragma once

#include <vector>
#include <string>

namespace OrbitPost
{
    struct Settings
    {
        bool bloom = true;
        bool toneMapping = true;
        bool ambientOcclusion = true;
        bool motionBlur = false;
        bool depthOfField = false;

        float exposure = 1.0f;
        float bloomThreshold = 1.0f;
        float bloomIntensity = 0.08f;
    };

    struct Pass
    {
        std::string name;
        bool enabled = true;
    };

    class Chain
    {
    private:
        Settings settings{};
        std::vector<Pass> passes;

        void Rebuild();

    public:
        void SetSettings(const Settings& value);

        const Settings& GetSettings() const;

        const std::vector<Pass>& GetPasses() const;
    };
}
