#pragma once

#if PROTOCOL_VERSION > 754
#include "protocolCraft/BaseMessage.hpp"
#include "protocolCraft/Types/Chat/Chat.hpp"

namespace ProtocolCraft
{
    class ClientboundSetTitleTextPacket : public BaseMessage<ClientboundSetTitleTextPacket>
    {
    public:
#if   PROTOCOL_VERSION == 755 || PROTOCOL_VERSION == 756
        static constexpr int packet_id = 0x59;
#elif PROTOCOL_VERSION == 757 || PROTOCOL_VERSION == 758 ||  \
      PROTOCOL_VERSION == 759
        static constexpr int packet_id = 0x5A;
#elif PROTOCOL_VERSION == 760
        static constexpr int packet_id = 0x5D;
#elif PROTOCOL_VERSION == 761
        static constexpr int packet_id = 0x5B;
#elif PROTOCOL_VERSION == 762 || PROTOCOL_VERSION == 763
        static constexpr int packet_id = 0x5F;
#else
#error "Protocol version not implemented"
#endif

        static constexpr std::string_view packet_name = "Set Title Text";

        virtual ~ClientboundSetTitleTextPacket() override
        {

        }


        void SetText(const Chat& text_)
        {
            text = text_;
        }


        const Chat& GetText() const
        {
            return text;
        }


    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {
            text = ReadData<Chat>(iter, length);
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
            WriteData<Chat>(text, container);
        }

        virtual Json::Value SerializeImpl() const override
        {
            Json::Value output;

            output["text"] = text;

            return output;
        }

    private:
        Chat text;

    };
} //ProtocolCraft
#endif
