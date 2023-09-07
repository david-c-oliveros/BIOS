#pragma once

#include <iostream>

#include "miniaudio.h"



class Audio
{
    public:
        enum class Type
        {
            AUDIO_FILE,
            WAVEFORM
        };

    public:
        Audio(const char* pPath, Type _eType);
        ~Audio();

        bool Start();
        bool Stop();
        bool SeekToTime(float fTimeInSec);


    public:
        /******************************/
        /*        Audio Engine        */
        /******************************/
        ma_device gDevice;
        ma_decoder gDecoder;
        Type eType;

        uint32_t m_nSampleRate;


    private:
        bool InitMA(const char* pPath);
        static void DataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 nFrameCount);
        static void DataCallbackWaveform(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 nFrameCount);
};
