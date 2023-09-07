#define MINIAUDIO_IMPLEMENTATION

#include "audio.h"



Audio::Audio(const char* pPath, Type _eType)
    : eType(_eType)
{
    InitMA(pPath);
}


Audio::~Audio()
{
    ma_device_uninit(&gDevice);
    ma_decoder_uninit(&gDecoder);
}



bool Audio::Start()
{
    if (ma_device_start(&gDevice) != MA_SUCCESS)
    {
        std::cout << "ERROR::MINAUDIO: Failed to start playback device" << std::endl;
        return false;
    }

    const char* e[2] = { "audio file", "waveform" };
    std::cout << "Playing " << e[(size_t)eType] << std::endl;

    return true;
}



bool Audio::Stop()
{
    if (ma_device_stop(&gDevice) != MA_SUCCESS)
    {
        std::cout << "ERROR::MINAUDIO: Failed to stop playback device" << std::endl;
        return false;
    }

    const char* e[2] = { "audio file", "waveform" };
    std::cout << "Stopping " << e[(size_t)eType] << std::endl;

    return true;
}



bool Audio::InitMA(const char* pPath)
{
    switch(eType)
    {
        case Type::AUDIO_FILE:
        {
            ma_result result;
            ma_device_config gDeviceConfig;

            result = ma_decoder_init_file(pPath, NULL, &gDecoder);
            if (result != MA_SUCCESS)
            {
                std::cout << "ERROR::MINAUDIO: Could not load audio file" << std::endl;
                return false;
            }

            gDeviceConfig = ma_device_config_init(ma_device_type_playback);
            gDeviceConfig.playback.format   = gDecoder.outputFormat;
            gDeviceConfig.playback.channels = gDecoder.outputChannels;
            gDeviceConfig.sampleRate        = gDecoder.outputSampleRate;
            gDeviceConfig.dataCallback      = DataCallback;
            gDeviceConfig.pUserData         = &gDecoder;

            m_nSampleRate = gDeviceConfig.sampleRate;

            if (ma_device_init(NULL, &gDeviceConfig, &gDevice) != MA_SUCCESS)
            {
                std::cout << "ERROR::MINAUDIO: Failed to open playback device" << std::endl;
                ma_decoder_uninit(&gDecoder);
                return false;
            }

            break;
        }

        case Type::WAVEFORM:
        {
            ma_waveform gSineWave;
            ma_device_config gDeviceConfig;
            ma_waveform_config gSineWaveConfig;

            gDeviceConfig = ma_device_config_init(ma_device_type_playback);
            gDeviceConfig.sampleRate        = 48000;
            gDeviceConfig.dataCallback      = DataCallbackWaveform;
            gDeviceConfig.pUserData         = &gSineWave;

            m_nSampleRate = gDeviceConfig.sampleRate;

            if (ma_device_init(NULL, &gDeviceConfig, &gDevice) != MA_SUCCESS)
            {
                std::cout << "ERROR::MINAUDIO: Failed to open playback device" << std::endl;
                return false;
            }

            gSineWaveConfig = ma_waveform_config_init(gDevice.playback.format, gDevice.playback.channels, gDevice.sampleRate, ma_waveform_type_sine, 0.2, 220);
            ma_waveform_init(&gSineWaveConfig, &gSineWave);

            break;
        }
    }
    return true;
}



bool Audio::SeekToTime(float fTimeInSec)
{
    uint32_t iTargetFrame = fTimeInSec * m_nSampleRate;
    ma_result result = ma_decoder_seek_to_pcm_frame(&gDecoder, iTargetFrame);
    if (result != MA_SUCCESS)
    {
        std::cout << "ERROR::MINIAUDIO: Could not seek to frame" << std::endl;
        return false;
    }

    return true;
}



void Audio::DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 nFrameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;

    if (pDecoder == NULL)
        return;

    ma_decoder_read_pcm_frames(pDecoder, pOutput, nFrameCount, NULL);
}



void Audio::DataCallbackWaveform(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 nFrameCount)
{
    ma_waveform* pSineWave;

//    MA_ASSERT(pDevice->playback.channels == 1);

    pSineWave = (ma_waveform*)pDevice->pUserData;
    MA_ASSERT(pSineWave != NULL);

    ma_waveform_read_pcm_frames(pSineWave, pOutput, nFrameCount, NULL);
}
