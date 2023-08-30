#if PROTOCOL_VERSION < 761
#pragma once

#include "protocolCraft/BaseMessage.hpp"

namespace ProtocolCraft
{
    class ClientboundCustomSoundPacket : public BaseMessage<ClientboundCustomSoundPacket>
    {
    public:
#if   PROTOCOL_VERSION == 340
        static constexpr int packet_id = 0x19;
#elif PROTOCOL_VERSION == 393 || PROTOCOL_VERSION == 401 ||  \
      PROTOCOL_VERSION == 404
        static constexpr int packet_id = 0x1A;
#elif PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 ||  \
      PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 ||  \
      PROTOCOL_VERSION == 498
        static constexpr int packet_id = 0x19;
#elif PROTOCOL_VERSION == 573 || PROTOCOL_VERSION == 575 ||  \
      PROTOCOL_VERSION == 578
        static constexpr int packet_id = 0x1A;
#elif PROTOCOL_VERSION == 735 || PROTOCOL_VERSION == 736
        static constexpr int packet_id = 0x19;
#elif PROTOCOL_VERSION == 751 || PROTOCOL_VERSION == 753 ||  \
      PROTOCOL_VERSION == 754
        static constexpr int packet_id = 0x18;
#elif PROTOCOL_VERSION == 755 || PROTOCOL_VERSION == 756 ||  \
      PROTOCOL_VERSION == 757 || PROTOCOL_VERSION == 758
        static constexpr int packet_id = 0x19;
#elif PROTOCOL_VERSION == 759
        static constexpr int packet_id = 0x16;
#elif PROTOCOL_VERSION == 760
        static constexpr int packet_id = 0x17;
#else
#error "Protocol version not implemented"
#endif

        static constexpr std::string_view packet_name = "Custom Sound";

        virtual ~ClientboundCustomSoundPacket() override
        {

        }

        void SetName_(const std::string& name__)
        {
            name_ = name__;
        }

        void SetSource(const int source_)
        {
            source = source_;
        }

        void SetX(const int x_)
        {
            x = x_;
        }

        void SetY(const int y_)
        {
            y = y_;
        }

        void SetZ(const int z_)
        {
            z = z_;
        }

        void SetVolume(const float volume_)
        {
            volume = volume_;
        }

        void SetPitch(const float pitch_)
        {
            pitch = pitch_;
        }

#if PROTOCOL_VERSION > 758
        void SetSeed(const long long int seed_)
        {
            seed = seed_;
        }
#endif


        const std::string& GetName_() const
        {
            return name_;
        }

        int GetSource() const
        {
            return source;
        }

        int GetX() const
        {
            return x;
        }

        int GetY() const
        {
            return y;
        }

        int GetZ() const
        {
            return z;
        }

        float GetVolume() const
        {
            return volume;
        }

        float GetPitch() const
        {
            return pitch;
        }

#if PROTOCOL_VERSION > 758
        long long int GetSeed() const
        {
            return seed;
        }
#endif


    protected:
        virtual void ReadImpl(ReadIterator& iter, size_t& length) override
        {
            name_ = ReadData<std::string>(iter, length);
            source = ReadData<VarInt>(iter, length);
            x = ReadData<int>(iter, length);
            y = ReadData<int>(iter, length);
            z = ReadData<int>(iter, length);
            volume = ReadData<float>(iter, length);
            pitch = ReadData<float>(iter, length);
#if PROTOCOL_VERSION > 758
            seed = ReadData<long long int>(iter, length);
#endif
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
            WriteData<std::string>(name_, container);
            WriteData<VarInt>(source, container);
            WriteData<int>(x, container);
            WriteData<int>(y, container);
            WriteData<int>(z, container);
            WriteData<float>(volume, container);
            WriteData<float>(pitch, container);
#if PROTOCOL_VERSION > 758
            WriteData<long long int>(seed, container);
#endif
        }

        virtual Json::Value SerializeImpl() const override
        {
            Json::Value output;

            output["name_"] = name_;
            output["source"] = source;
            output["x"] = x;
            output["y"] = y;
            output["z"] = z;
            output["volume"] = volume;
            output["pitch"] = pitch;
#if PROTOCOL_VERSION > 758
            output["seed"] = seed;
#endif

            return output;
        }

    private:
        std::string name_;
        int source = 0;
        int x = 0;
        int y = 0;
        int z = 0;
        float volume = 0.0f;
        float pitch = 0.0f;
#if PROTOCOL_VERSION > 758
        long long int seed = 0;
#endif

    };
} //ProtocolCraft
#endif
