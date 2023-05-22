#pragma once

#include "protocolCraft/BaseMessage.hpp"

#include "protocolCraft/Utilities/PluginLoader.hpp"

namespace ProtocolCraft
{
    class ServerboundCustomPayloadPacket : public BaseMessage<ServerboundCustomPayloadPacket>
    {
    public:
#if PROTOCOL_VERSION == 340 // 1.12.2
        static constexpr int packet_id = 0x09;
#elif PROTOCOL_VERSION == 393 || PROTOCOL_VERSION == 401 || PROTOCOL_VERSION == 404 // 1.13.X
        static constexpr int packet_id = 0x0A;
#elif PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 || PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 || PROTOCOL_VERSION == 498 // 1.14.X
        static constexpr int packet_id = 0x0B;
#elif PROTOCOL_VERSION == 573 || PROTOCOL_VERSION == 575 || PROTOCOL_VERSION == 578 // 1.15.X
        static constexpr int packet_id = 0x0B;
#elif PROTOCOL_VERSION == 735 || PROTOCOL_VERSION == 736  // 1.16 or 1.16.1
        static constexpr int packet_id = 0x0B;
#elif PROTOCOL_VERSION == 751 || PROTOCOL_VERSION == 753 || PROTOCOL_VERSION == 754 // 1.16.2, 1.16.3, 1.16.4, 1.16.5
        static constexpr int packet_id = 0x0B;
#elif PROTOCOL_VERSION == 755 || PROTOCOL_VERSION == 756 // 1.17.X
        static constexpr int packet_id = 0x0A;
#elif PROTOCOL_VERSION == 757 || PROTOCOL_VERSION == 758 // 1.18, 1.18.1 or 1.18.2
        static constexpr int packet_id = 0x0A;
#elif PROTOCOL_VERSION == 759 // 1.19
        static constexpr int packet_id = 0x0C;
#elif PROTOCOL_VERSION == 760 // 1.19.1 or 1.19.2
        static constexpr int packet_id = 0x0D;
#elif PROTOCOL_VERSION == 761 // 1.19.3
        static constexpr int packet_id = 0x0C;
#elif PROTOCOL_VERSION == 762 // 1.19.4
static constexpr int packet_id = 0x0D;
#else
#error "Protocol version not implemented"
#endif
        static constexpr std::string_view packet_name = "Custom Payload";

        virtual ~ServerboundCustomPayloadPacket() override
        {

        }

        void SetIdentifier(const std::string& identifier_)
        {
            identifier = identifier_;
        }

        void SetRawData(const std::vector<unsigned char>& raw_data_)
        {
            raw_data = raw_data_;
        }

        void SetParsedData(const std::shared_ptr<NetworkType>& parsed_data_)
        {
            parsed_data = parsed_data_;
        }


        const std::string& GetIdentifier() const
        {
            return identifier;
        }

        const std::vector<unsigned char>& GetRawData() const
        {
            return raw_data;
        }

        const std::shared_ptr<NetworkType>& GetParsedData() const
        {
            return parsed_data;
        }


    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {
            identifier = ReadData<std::string>(iter, length);
            parsed_data = CreateObjectFromPlugin(identifier.c_str());
            if (parsed_data == nullptr)
            {
                raw_data = ReadByteArray(iter, length, length);
            }
            else
            {
                raw_data.clear();
                parsed_data->Read(iter, length);
            }
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
            WriteData<std::string>(identifier, container);
            if (parsed_data == nullptr)
            {
                WriteByteArray(raw_data, container);
            }
            else
            {
                parsed_data->Write(container);
            }
        }

        virtual Json::Value SerializeImpl() const override
        {
            Json::Value output;

            output["identifier"] = identifier;
            if (parsed_data == nullptr)
            {
                output["data"] = "Vector of " + std::to_string(raw_data.size()) + " unsigned chars";
            }
            else
            {
                output["data"] = parsed_data->Serialize();
            }

            return output;
        }

    private:
        std::string identifier;
        std::vector<unsigned char> raw_data;
        std::shared_ptr<NetworkType> parsed_data;

    };
} //ProtocolCraft
