#pragma once

#if PROTOCOL_VERSION > 385
#include "protocolCraft/BaseMessage.hpp"

namespace ProtocolCraft
{
    class ServerboundSetBeaconPacket : public BaseMessage<ServerboundSetBeaconPacket>
    {
    public:
        virtual const int GetId() const override
        {
#if PROTOCOL_VERSION == 393 || PROTOCOL_VERSION == 401 || PROTOCOL_VERSION == 404 // 1.13.X
            return 0x20;
#elif PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 || PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 || PROTOCOL_VERSION == 498 // 1.14.X
            return 0x22;
#elif PROTOCOL_VERSION == 573 || PROTOCOL_VERSION == 575 || PROTOCOL_VERSION == 578 // 1.15.X
            return 0x22;
#elif PROTOCOL_VERSION == 735 || PROTOCOL_VERSION == 736  // 1.16.0 or 1.16.1
            return 0x23;
#elif PROTOCOL_VERSION == 751 || PROTOCOL_VERSION == 753 || PROTOCOL_VERSION == 754 // 1.16.2, 1.16.3, 1.16.4, 1.16.5
            return 0x24;
#elif PROTOCOL_VERSION == 755 || PROTOCOL_VERSION == 756 // 1.17.X
            return 0x24;
#elif PROTOCOL_VERSION == 757 || PROTOCOL_VERSION == 758 // 1.18, 1.18.1 or 1.18.2
            return 0x24;
#else
#error "Protocol version not implemented"
#endif
        }

        virtual const std::string GetName() const override
        {
            return "Set Beacon";
        }

        virtual ~ServerboundSetBeaconPacket() override
        {

        }

        void SetPrimary(const int primary_)
        {
            primary = primary_;
        }

        void SetSecondary(const int secondary_)
        {
            secondary = secondary_;
        }


        const int GetPrimary() const
        {
            return primary;
        }

        const int GetSecondary() const
        {
            return secondary;
        }


    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {
            primary = ReadData<VarInt>(iter, length);
            secondary = ReadData<VarInt>(iter, length);
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
            WriteData<VarInt>(primary, container);
            WriteData<VarInt>(secondary, container);
        }

        virtual const nlohmann::json SerializeImpl() const override
        {
            nlohmann::json output;

            output["primary"] = primary;
            output["secondary"] = secondary;

            return output;
        }

    private:
        int primary;
        int secondary;

    };
} //ProtocolCraft
#endif
