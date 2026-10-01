#include "OrbitSaveSystem.h"

#include <fstream>

namespace Orbit
{
    bool OrbitSaveSystem::Save(
        const std::string& path,
        const SaveState& state) const
    {
        std::ofstream file(
            path,
            std::ios::binary);

        if (!file)
            return false;

        file.write(
            reinterpret_cast<const char*>(
                &state),
            sizeof(SaveState));

        return file.good();
    }

    bool OrbitSaveSystem::Load(
        const std::string& path,
        SaveState& state) const
    {
        std::ifstream file(
            path,
            std::ios::binary);

        if (!file)
            return false;

        file.read(
            reinterpret_cast<char*>(
                &state),
            sizeof(SaveState));

        return file.good();
    }
}
