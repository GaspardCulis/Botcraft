#pragma once

#include "protocolCraft/BaseMessage.hpp"
#include "protocolCraft/Utilities/Plugins/PluginLoader.hpp"
#include "protocolCraft/Utilities/Plugins/PluginObject.hpp"

namespace ProtocolCraft
{
    class ServerboundCustomPayloadPacket : public BaseMessage<ServerboundCustomPayloadPacket>
    {
    public:
#if   PROTOCOL_VERSION == 340
        static constexpr int packet_id = 0x09;
#elif PROTOCOL_VERSION == 393 || PROTOCOL_VERSION == 401 ||  \
      PROTOCOL_VERSION == 404
        static constexpr int packet_id = 0x0A;
#elif PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 ||  \
      PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 ||  \
      PROTOCOL_VERSION == 498 || PROTOCOL_VERSION == 573 ||  \
      PROTOCOL_VERSION == 575 || PROTOCOL_VERSION == 578 ||  \
      PROTOCOL_VERSION == 735 || PROTOCOL_VERSION == 736 ||  \
      PROTOCOL_VERSION == 751 || PROTOCOL_VERSION == 753 ||  \
      PROTOCOL_VERSION == 754
        static constexpr int packet_id = 0x0B;
#elif PROTOCOL_VERSION == 755 || PROTOCOL_VERSION == 756 ||  \
      PROTOCOL_VERSION == 757 || PROTOCOL_VERSION == 758
        static constexpr int packet_id = 0x0A;
#elif PROTOCOL_VERSION == 759
        static constexpr int packet_id = 0x0C;
#elif PROTOCOL_VERSION == 760
        static constexpr int packet_id = 0x0D;
#elif PROTOCOL_VERSION == 761
        static constexpr int packet_id = 0x0C;
#elif PROTOCOL_VERSION == 762 || PROTOCOL_VERSION == 763
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

        void SetParsedData(const std::shared_ptr<PluginObject>& parsed_data_)
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

        const std::shared_ptr<PluginObject>& GetParsedData() const
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
                unsigned long long int data_size = static_cast<unsigned long long int>(length);
                const unsigned char* data_ptr = &(*iter);
                parsed_data->Read(data_ptr, data_size);
                iter += length - data_size;
                length = static_cast<size_t>(data_size);
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
                unsigned long long int serialized_length = 0;
                const unsigned char* serialized = parsed_data->Write(serialized_length);
                WriteByteArray(serialized, static_cast<size_t>(serialized_length), container);
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
                const std::string json_serialized(parsed_data->Serialize());
                output["data"] = Json::Parse(json_serialized);
            }

            return output;
        }

    private:
        std::string identifier;
        std::vector<unsigned char> raw_data;
        std::shared_ptr<PluginObject> parsed_data;

    };
} //ProtocolCraft
