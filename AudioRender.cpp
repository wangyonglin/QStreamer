#include "AudioRender.h"
#include <string>

CAudioRender::CAudioRender()
{
    int flags = SDL_INIT_AUDIO;
    if (SDL_Init(flags)) {
        return;
    }
}

CAudioRender::~CAudioRender()
{
    Stop();
}

int CAudioRender::SetAudioParams(int channelNum, int sampleRate, int bytePerSample)
{
    if (m_channelNum != channelNum || sampleRate != m_sampleRate || m_bytePerSample != bytePerSample)
    {
        m_channelNum = channelNum;
        m_sampleRate = sampleRate;
        m_bytePerSample = bytePerSample;
        InitPlayerParams();
    }
    return 0;
}

int CAudioRender::Start()
{
    m_inputMode = kExtFeedMode;
    SDL_PauseAudioDevice(m_audio_devID, 0);
    std::string errstr = SDL_GetError();
    return errstr.empty() ? 0 : -1;
}

int CAudioRender::InitPlayerParams()
{
    SDL_ClearQueuedAudio(m_audio_devID);
    SDL_CloseAudioDevice(m_audio_devID);

    SDL_AudioSpec wanted_spec, spec;
    wanted_spec.channels = m_channelNum;
    wanted_spec.freq = m_sampleRate;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.silence = 0;
    wanted_spec.samples = 1024;
    wanted_spec.callback = sdl_audio_callback;
    wanted_spec.userdata = this;
    m_audio_devID = SDL_OpenAudioDevice(NULL, 0, &wanted_spec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE);
    std::string errstr = SDL_GetError();
    if (!errstr.empty())
    {
        SDL_PauseAudioDevice(m_audio_devID, 1);   //默认暂停播放状态
        errstr = SDL_GetError();
        return errstr.empty() ? 0 : 1;
    }
    else {
        return -1;
    }
}

int CAudioRender::Start(const char* path)
{
    if (path != m_inputFilePath)
    {
        if (m_inputFile.is_open())
        {
            m_inputFile.close();
        }
    }

    m_inputFilePath = path;
    m_inputMode = kInnFileMode;
    if (!m_inputFile.is_open())
    {
        m_inputFile.open(m_inputFilePath.c_str(), std::ios::binary);

        if (m_inputFile.is_open())
        {
            std::cout << "file open succ" << std::endl;
        }
        else
        {
            std::cout << "file open fail" << std::endl;
        }
    }
    SDL_PauseAudioDevice(m_audio_devID, 0);
    std::string errstr = SDL_GetError();
    return errstr.empty() ? 0 : -1;
}

int CAudioRender::Pause()
{
    SDL_PauseAudioDevice(m_audio_devID, 1);
    std::string errstr = SDL_GetError();
    return errstr.empty() ? 0 : -1;
    SDL_ClearQueuedAudio(m_audio_devID);
}

int CAudioRender::Stop()
{
    SDL_CloseAudio();
    std::string errstr = SDL_GetError();
    return errstr.empty() ? 0 : -1;
}

void CAudioRender::sdl_audio_callback(void *opaque, Uint8 *stream, int len)
{
    CAudioRender* audioRender = (CAudioRender*)opaque;

    if (audioRender->m_inputMode == kInnFileMode)
    {
        if (audioRender->m_inputFile.is_open())
        {
            audioRender->m_inputFile.read((char*)stream, len);
            int rCount = audioRender->m_inputFile.gcount();
            if (rCount < len)
            {
                //reach file end
                audioRender->m_inputFile.close();
                //if loop play,move to begin pos
                 audioRender->m_inputFile.seekg(0, std::ios::beg); //把文件的读指针从当前位置向后移1234个字节
            }
        }
    }
    else
    {

    }
}
