#include "OrbitPostProcess.h"

namespace OrbitPost
{
    void Chain::SetSettings(
        const Settings& value)
    {
        settings = value;
        Rebuild();
    }

    const Settings&
    Chain::GetSettings() const
    {
        return settings;
    }

    const std::vector<Pass>&
    Chain::GetPasses() const
    {
        return passes;
    }

    void Chain::Rebuild()
    {
        passes.clear();

        if (settings.ambientOcclusion)
            passes.push_back(
            {"AmbientOcclusion", true});

        if (settings.bloom)
            passes.push_back(
            {"Bloom", true});

        if (settings.toneMapping)
            passes.push_back(
            {"ToneMapping", true});

        if (settings.depthOfField)
            passes.push_back(
            {"DepthOfField", true});

        if (settings.motionBlur)
            passes.push_back(
            {"MotionBlur", true});

        passes.push_back(
            {"FinalComposite", true});
    }
}
