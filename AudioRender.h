#ifndef H_AUDIO_RENDER_H
#define H_AUDIO_RENDER_H

#include <SDL.h>
#include <fstream>
#include <iostream>

class CAudioRender
{
public:
    CAudioRender();
    ~CAudioRender();

    typedef enum AudioFeedMode
    {
        kInnFileMode,  //根据路径自动读取模式，测试播放pcm使用
        kExtFeedMode,  //由用户频繁输入音频数据,实际播放器使用
    }AudioFeedMode;

    int SetAudioParams(int channelNum, int sampleRate, int bytePerSample);

    int Start();
    int Start(const char* path);

    int Pause();

    int Stop();
private:
    int InitPlayerParams();

public:
    static void sdl_audio_callback(void *opaque, Uint8 *stream, int len);
private:
    SDL_AudioDeviceID m_audio_devID;

    std::string m_inputFilePath = "";
    std::ifstream m_inputFile;
    AudioFeedMode m_inputMode = kInnFileMode;

    int m_channelNum = 0;
    int m_sampleRate = 0;
    int m_bytePerSample = 0;
};

#endif
