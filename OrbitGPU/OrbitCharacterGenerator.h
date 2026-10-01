#pragma once

#include "OrbitCharacterTypes.h"

#include <cstdint>
#include <string>

namespace Orbit
{

class CharacterGenerator
{
public:
    CharacterGenerator();

    OrbitCharacter Generate(std::uint64_t seed) const;

private:
    static std::uint64_t Hash(std::uint64_t value);
    static float Random01(std::uint64_t seed, std::uint64_t salt);

    static CharacterColor SkinColor(int tone);
    static CharacterColor HairColor(int color);
    static CharacterColor EyeColor(int color);

    static std::string FirstName(std::uint64_t seed);
    static std::string LastName(std::uint64_t seed);
    static std::string Profession(std::uint64_t seed);
    static std::string Faction(std::uint64_t seed);

    static void BuildCharacterMesh(
        OrbitCharacter& character,
        std::uint64_t seed
    );
};

}
