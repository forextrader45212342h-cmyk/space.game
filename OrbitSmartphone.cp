#include "OrbitSmartphone.h"

namespace Orbit
{
    void OrbitSmartphone::PowerOn()
    {
        PoweredOn = true;
    }

    void OrbitSmartphone::PowerOff()
    {
        EndCall();
        PoweredOn = false;
    }

    bool OrbitSmartphone::IsPoweredOn() const
    {
        return PoweredOn;
    }

    void OrbitSmartphone::AddContact(
        const PhoneContact& contact)
    {
        Contacts.push_back(contact);
    }

    bool OrbitSmartphone::Call(
        const std::string& number)
    {
        if (!PoweredOn)
            return false;

        if (number.empty())
            return false;

        CurrentCall = {};
        CurrentCall.Id = NextCallId++;
        CurrentCall.Number = number;
        CurrentCall.State =
            CallState::Dialing;

        return true;
    }

    void OrbitSmartphone::AcceptCall()
    {
        if (CurrentCall.State ==
                CallState::Ringing ||
            CurrentCall.State ==
                CallState::Dialing)
        {
            CurrentCall.State =
                CallState::Connected;
        }
    }

    void OrbitSmartphone::EndCall()
    {
        if (CurrentCall.State !=
            CallState::Idle)
        {
            CurrentCall.State =
                CallState::Ended;
        }
    }

    void OrbitSmartphone::Update(
        double deltaSeconds)
    {
        if (!PoweredOn)
            return;

        if (CurrentCall.State ==
            CallState::Dialing)
        {
            if (CurrentCall.DurationSeconds >
                1.0)
            {
                CurrentCall.State =
                    CallState::Connected;
            }
        }

        if (CurrentCall.State ==
            CallState::Connected)
        {
            CurrentCall.DurationSeconds +=
                deltaSeconds;
        }
        else if (CurrentCall.State ==
                 CallState::Dialing)
        {
            CurrentCall.DurationSeconds +=
                deltaSeconds;
        }
    }

    const PhoneCall&
    OrbitSmartphone::GetCurrentCall() const
    {
        return CurrentCall;
    }

    const std::vector<PhoneContact>&
    OrbitSmartphone::GetContacts() const
    {
        return Contacts;
    }
}
