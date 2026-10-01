#include "OrbitNetworkState.h"

namespace Orbit
{
    void OrbitNetworkState::Apply(
        const NetworkEntityState& state)
    {
        State = state;
    }

    const NetworkEntityState&
    OrbitNetworkState::GetState() const
    {
        return State;
    }
}
