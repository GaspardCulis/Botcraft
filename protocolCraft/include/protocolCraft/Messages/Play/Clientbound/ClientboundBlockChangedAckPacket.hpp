#if PROTOCOL_VERSION > 758
#pragma once

#include "protocolCraft/BaseMessage.hpp"

namespace ProtocolCraft
{
    class ClientboundBlockChangedAckPacket : public BaseMessage<ClientboundBlockChangedAckPacket>
    {
    public:
#if   PROTOCOL_VERSION == 759 || PROTOCOL_VERSION == 760 ||  \
      PROTOCOL_VERSION == 761
        static constexpr int packet_id = 0x05;
#elif PROTOCOL_VERSION == 762 || PROTOCOL_VERSION == 763
        static constexpr int packet_id = 0x06;
#else
#error "Protocol version not implemented"
#endif

        static constexpr std::string_view packet_name = "Block Changed Ack";

        virtual ~ClientboundBlockChangedAckPacket() override
        {

        }

        void SetSequence(const int sequence_)
        {
            sequence = sequence_;
        }


        int GetSequence() const
        {
            return sequence;
        }

    protected:
        virtual void ReadImpl(ReadIterator &iter, size_t &length) override
        {
            sequence = ReadData<VarInt>(iter, length);
        }

        virtual void WriteImpl(WriteContainer &container) const override
        {
            WriteData<VarInt>(sequence, container);
        }

        virtual Json::Value SerializeImpl() const override
        {
            Json::Value output;

            output["sequence"] = sequence;

            return output;
        }

    private:
        int sequence = 0;
    };
} //ProtocolCraft

#endif
