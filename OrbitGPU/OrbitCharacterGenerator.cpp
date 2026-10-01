#include "OrbitCharacterGenerator.h"

#include <cmath>
#include <array>

namespace Orbit
{

CharacterGenerator::CharacterGenerator()
{
}

std::uint64_t CharacterGenerator::Hash(std::uint64_t x)
{
    x += 0x9e3779b97f4a7c15ULL;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

float CharacterGenerator::Random01(
    std::uint64_t seed,
    std::uint64_t salt)
{
    const std::uint64_t h = Hash(seed ^ Hash(salt));

    return static_cast<float>(
        (h & 0xFFFFFFULL) / 16777215.0
    );
}

CharacterColor CharacterGenerator::SkinColor(int tone)
{
    static const std::array<CharacterColor, 8> colors =
    {{
        {0.96f, 0.78f, 0.64f},
        {0.88f, 0.65f, 0.48f},
        {0.72f, 0.48f, 0.32f},
        {0.55f, 0.34f, 0.22f},
        {0.38f, 0.22f, 0.14f},
        {0.26f, 0.14f, 0.08f},
        {0.68f, 0.42f, 0.28f},
        {0.82f, 0.57f, 0.39f}
    }};

    return colors[static_cast<std::size_t>(tone) % colors.size()];
}

CharacterColor CharacterGenerator::HairColor(int color)
{
    static const std::array<CharacterColor, 8> colors =
    {{
        {0.03f, 0.02f, 0.015f},
        {0.10f, 0.055f, 0.025f},
        {0.22f, 0.10f, 0.04f},
        {0.38f, 0.20f, 0.08f},
        {0.55f, 0.34f, 0.16f},
        {0.72f, 0.62f, 0.40f},
        {0.20f, 0.20f, 0.22f},
        {0.78f, 0.78f, 0.82f}
    }};

    return colors[static_cast<std::size_t>(color) % colors.size()];
}

CharacterColor CharacterGenerator::EyeColor(int color)
{
    static const std::array<CharacterColor, 6> colors =
    {{
        {0.12f, 0.07f, 0.03f},
        {0.08f, 0.25f, 0.55f},
        {0.12f, 0.48f, 0.22f},
        {0.38f, 0.22f, 0.08f},
        {0.45f, 0.48f, 0.50f},
        {0.04f, 0.12f, 0.18f}
    }};

    return colors[static_cast<std::size_t>(color) % colors.size()];
}

std::string CharacterGenerator::FirstName(std::uint64_t seed)
{
    static const std::array<const char*, 20> names =
    {{
        "Adam", "Omar", "Ayaan", "Rayyan", "Zain",
        "Ali", "Hamza", "Sara", "Maya", "Lina",
        "Noor", "Amir", "Daniel", "Elena", "Mia",
        "Leo", "Alex", "Layla", "Hana", "Ibrahim"
    }};

    return names[
        static_cast<std::size_t>(
            Hash(seed) % names.size()
        )
    ];
}

std::string CharacterGenerator::LastName(std::uint64_t seed)
{
    static const std::array<const char*, 20> names =
    {{
        "Khan", "Malik", "Ahmed", "Hassan", "Farooq",
        "Raza", "Shah", "Rahman", "Stone", "Walker",
        "Morgan", "Reed", "Carter", "Hayes", "Wilson",
        "Turner", "Ali", "Hussain", "Qureshi", "Nadeem"
    }};

    return names[
        static_cast<std::size_t>(
            Hash(seed + 17) % names.size()
        )
    ];
}

std::string CharacterGenerator::Profession(std::uint64_t seed)
{
    static const std::array<const char*, 12> jobs =
    {{
        "Explorer",
        "Engineer",
        "Scientist",
        "Pilot",
        "Trader",
        "Medic",
        "Security Officer",
        "Farmer",
        "Mechanic",
        "Miner",
        "Mission Commander",
        "Technician"
    }};

    return jobs[
        static_cast<std::size_t>(
            Hash(seed + 31) % jobs.size()
        )
    ];
}

std::string CharacterGenerator::Faction(std::uint64_t seed)
{
    static const std::array<const char*, 8> factions =
    {{
        "Earth Federation",
        "Mars Colony",
        "Independent",
        "Deep Space Union",
        "Orbital Authority",
        "Mining Guild",
        "Scientific Alliance",
        "Frontier Settlers"
    }};

    return factions[
        static_cast<std::size_t>(
            Hash(seed + 71) % factions.size()
        )
    ];
}

void CharacterGenerator::BuildCharacterMesh(
    OrbitCharacter& character,
    std::uint64_t seed)
{
    /*
        Procedural humanoid base mesh.

        This creates actual CPU geometry:
        head + torso + arms + legs.

        Later this vertex/index data is uploaded
        into Vulkan vertex/index buffers.
    */

    character.vertices.clear();
    character.indices.clear();

    const float height =
        1.65f +
        static_cast<float>(character.appearance.heightClass) * 0.06f;

    const float bodyWidth =
        0.38f +
        static_cast<float>(character.appearance.bodyType) * 0.045f;

    auto addBox =
        [&](float cx, float cy, float cz,
            float sx, float sy, float sz)
    {
        const std::uint32_t base =
            static_cast<std::uint32_t>(character.vertices.size());

        const float x0 = cx - sx;
        const float x1 = cx + sx;
        const float y0 = cy - sy;
        const float y1 = cy + sy;
        const float z0 = cz - sz;
        const float z1 = cz + sz;

        const float positions[8][3] =
        {
            {x0,y0,z0},
            {x1,y0,z0},
            {x1,y1,z0},
            {x0,y1,z0},
            {x0,y0,z1},
            {x1,y0,z1},
            {x1,y1,z1},
            {x0,y1,z1}
        };

        for (int i = 0; i < 8; ++i)
        {
            CharacterMeshVertex v{};
            v.px = positions[i][0];
            v.py = positions[i][1];
            v.pz = positions[i][2];

            const float len =
                std::sqrt(
                    v.px * v.px +
                    v.py * v.py +
                    v.pz * v.pz
                );

            if (len > 0.0001f)
            {
                v.nx = v.px / len;
                v.ny = v.py / len;
                v.nz = v.pz / len;
            }
            else
            {
                v.nx = 0.0f;
                v.ny = 1.0f;
                v.nz = 0.0f;
            }

            v.u = (i & 1) ? 1.0f : 0.0f;
            v.v = (i & 2) ? 1.0f : 0.0f;

            character.vertices.push_back(v);
        }

        const std::uint32_t boxIndices[] =
        {
            0,1,2, 2,3,0,
            4,6,5, 6,4,7,
            0,4,5, 5,1,0,
            3,2,6, 6,7,3,
            0,3,7, 7,4,0,
            1,5,6, 6,2,1
        };

        for (std::uint32_t i : boxIndices)
            character.indices.push_back(base + i);
    };

    const float torsoY = height * 0.56f;
    const float legY = height * 0.25f;

    addBox(
        0.0f,
        torsoY,
        0.0f,
        bodyWidth,
        height * 0.20f,
        bodyWidth * 0.55f
    );

    addBox(
        -bodyWidth * 0.62f,
        torsoY,
        0.0f,
        bodyWidth * 0.22f,
        height * 0.18f,
        bodyWidth * 0.22f
    );

    addBox(
        bodyWidth * 0.62f,
        torsoY,
        0.0f,
        bodyWidth * 0.22f,
        height * 0.18f,
        bodyWidth * 0.22f
    );

    addBox(
        -bodyWidth * 0.45f,
        legY,
        0.0f,
        bodyWidth * 0.25f,
        height * 0.24f,
        bodyWidth * 0.25f
    );

    addBox(
        bodyWidth * 0.45f,
        legY,
        0.0f,
        bodyWidth * 0.25f,
        height * 0.24f,
        bodyWidth * 0.25f
    );

    addBox(
        0.0f,
        height * 0.82f,
        0.0f,
        bodyWidth * 0.38f,
        bodyWidth * 0.38f,
        bodyWidth * 0.38f
    );

    const float variation =
        0.95f +
        Random01(seed, 900) * 0.1f;

    character.transform.scale = variation;
}

OrbitCharacter CharacterGenerator::Generate(
    std::uint64_t seed) const
{
    OrbitCharacter character;

    character.identity.id = seed;

    character.identity.firstName =
        FirstName(seed);

    character.identity.lastName =
        LastName(seed);

    character.identity.profession =
        Profession(seed);

    character.identity.faction =
        Faction(seed);

    character.identity.homePlanet =
        (Hash(seed + 100) % 2 == 0)
        ? "Earth"
        : "Mars";

    character.identity.personality =
        Random01(seed, 1);

    character.identity.bravery =
        Random01(seed, 2);

    character.identity.intelligence =
        Random01(seed, 3);

    character.identity.friendliness =
        Random01(seed, 4);

    character.appearance.skinTone =
        static_cast<int>(Hash(seed + 10) % 8);

    character.appearance.hairStyle =
        static_cast<int>(Hash(seed + 11) % 10);

    character.appearance.hairColor =
        static_cast<int>(Hash(seed + 12) % 8);

    character.appearance.eyeColor =
        static_cast<int>(Hash(seed + 13) % 6);

    character.appearance.faceShape =
        static_cast<int>(Hash(seed + 14) % 8);

    character.appearance.bodyType =
        static_cast<int>(Hash(seed + 15) % 5);

    character.appearance.heightClass =
        static_cast<int>(Hash(seed + 16) % 7);

    character.appearance.ageClass =
        static_cast<int>(Hash(seed + 18) % 5);

    character.appearance.skin =
        SkinColor(character.appearance.skinTone);

    character.appearance.hair =
        HairColor(character.appearance.hairColor);

    character.appearance.eyes =
        EyeColor(character.appearance.eyeColor);

    character.appearance.clothingPrimary =
    {
        Random01(seed, 21),
        Random01(seed, 22),
        Random01(seed, 23)
    };

    character.appearance.clothingSecondary =
    {
        Random01(seed, 24),
        Random01(seed, 25),
        Random01(seed, 26)
    };

    BuildCharacterMesh(character, seed);

    return character;
}

}
