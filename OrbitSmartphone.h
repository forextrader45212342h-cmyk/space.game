#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Orbit
{
    struct PhoneContact
    {
        std::uint64_t Id = 0;
        std::string Name;
        std::string Number;
    };

    enum class CallState
    {
        Idle,
        Dialing,
        Ringing,
        Connected,
        Ended,
        Failed
    };

    struct PhoneCall
    {
        std::uint64_t Id = 0;

        std::string Number;
        std::string ContactName;

        CallState State =
            CallState::Idle;

        double DurationSeconds = 0.0;
    };

    class OrbitSmartphone
    {
    public:
        void PowerOn();
        void PowerOff();

        bool IsPoweredOn() const;

        void AddContact(
            const PhoneContact& contact);

        bool Call(
            const std::string& number);

        void AcceptCall();

        void EndCall();

        void Update(
            double deltaSeconds);

        const PhoneCall& GetCurrentCall() const;

        const std::vector<PhoneContact>&
            GetContacts() const;

    private:
        bool PoweredOn = true;

        std::vector<PhoneContact> Contacts;

        PhoneCall CurrentCall;
        std::uint64_t NextCallId = 1;
    };
}
