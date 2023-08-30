#pragma once

#if PROTOCOL_VERSION > 440
#include "protocolCraft/BaseMessage.hpp"

#if PROTOCOL_VERSION > 760
#include "protocolCraft/Types/Sound/SoundEvent.hpp"
#endif

namespace ProtocolCraft
{
    class ClientboundSoundEntityPacket : public BaseMessage<ClientboundSoundEntityPacket>
    {
    public:
#if   PROTOCOL_VERSION == 477 || PROTOCOL_VERSION == 480 ||  \
      PROTOCOL_VERSION == 485 || PROTOCOL_VERSION == 490 ||  \
      PROTOCOL_VERSION == 498
        static constexpr int packet_id = 0x50;
#elif PROTOCOL_VERSION == 573 || PROTOCOL_VERSION == 575 ||  \
      PROTOCOL_VERSION == 578
        static constexpr int packet_id = 0x51;
#elif PROTOCOL_VERSION == 735 || PROTOCOL_VERSION == 736 ||  \
      PROTOCOL_VERSION == 751 || PROTOCOL_VERSION == 753 ||  \
      PROTOCOL_VERSION == 754
        static constexpr int packet_id = 0x50;
#elif PROTOCOL_VERSION == 755 || PROTOCOL_VERSION == 756
        static constexpr int packet_id = 0x5B;
#elif PROTOCOL_VERSION == 757 || PROTOCOL_VERSION == 758 ||  \
      PROTOCOL_VERSION == 759
        static constexpr int packet_id = 0x5C;
#elif PROTOCOL_VERSION == 760
        static constexpr int packet_id = 0x5F;
#elif PROTOCOL_VERSION == 761
        static constexpr int packet_id = 0x5D;
#elif PROTOCOL_VERSION == 762 || PROTOCOL_VERSION == 763
        static constexpr int packet_id = 0x61;
#else
#error "Protocol version not implemented"
#endif

        static constexpr std::string_view packet_name = "Sound Entity";

        virtual ~ClientboundSoundEntityPacket() override
        {

        }

#if PROTOCOL_VERSION < 761
        void SetSound(const int sound_)
        {
            sound = sound_;
        }
#else
        void SetSound(const SoundEvent& sound_)
        {
            sound = sound_;
        }
#endif

        void SetSource(const int source_)
        {
            source = source_;
        }

        void SetId_(const int id__)
        {
            id_ = id__;
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


#if PROTOCOL_VERSION < 761
        int GetSound() const
        {
            return sound;
        }
#else
        const SoundEvent& GetSound() const
        {
            return sound;
        }
#endif

        int GetSource() const
        {
            return source;
        }

        int GetId_() const
        {
            return id_;
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
#if PROTOCOL_VERSION < 761
            sound = ReadData<VarInt>(iter, length);
#else
            sound = ReadData<SoundEvent>(iter, length);
#endif
            source = ReadData<VarInt>(iter, length);
            id_ = ReadData<VarInt>(iter, length);
            volume = ReadData<float>(iter, length);
            pitch = ReadData<float>(iter, length);
#if PROTOCOL_VERSION > 758
            seed = ReadData<long long int>(iter, length);
#endif
        }

        virtual void WriteImpl(WriteContainer& container) const override
        {
#if PROTOCOL_VERSION < 761
            WriteData<VarInt>(sound, container);
#else
            WriteData<SoundEvent>(sound, container);
#endif
            WriteData<VarInt>(source, container);
            WriteData<VarInt>(id_, container);
            WriteData<float>(volume, container);
            WriteData<float>(pitch, container);
#if PROTOCOL_VERSION > 758
            WriteData<long long int>(seed, container);
#endif
        }

        virtual Json::Value SerializeImpl() const override
        {
            Json::Value output;

            output["sound"] = sound;
            output["source"] = source;
            output["id_"] = id_;
            output["volume"] = volume;
            output["pitch"] = pitch;
#if PROTOCOL_VERSION > 758
            output["seed"] = seed;
#endif

            return output;
        }

    private:
#if PROTOCOL_VERSION < 761
        int sound = 0;
#else
        SoundEvent sound;
#endif
        int source = 0;
        int id_ = 0;
        float volume = 0.0f;
        float pitch = 0.0f;
#if PROTOCOL_VERSION > 758
        long long int seed = 0;
#endif

    };
} //ProtocolCraft
#endif
