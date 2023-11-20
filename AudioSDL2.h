#ifndef AUDIOSDL2_H
#define AUDIOSDL2_H

#include <QDebug>
extern "C" {        // 用C规则编译指定的代码
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>
#include <SDL.h>
#include <SDL_audio.h>
}
#include <fstream>
#include <iostream>

class AudioSDL2
{
public:
    AudioSDL2();
    int url(const QString& url = QString());
    int play();
    int stop();
private:
    SDL_AudioDeviceID audio_devID;
};

#endif // AUDIOSDL2_H
