#include "Arduino.h"
#include "AudioPlayer.h"
#include <TMRpcm.h>

TMRpcm tmrpcm;
long alarmStartedAt;
bool isStarted = false;

void AudioPlayer::init(int audioPin)
{
    tmrpcm.speakerPin = audioPin;
    tmrpcm.setVolume(7);
    tmrpcm.loop(true);
    tmrpcm.play("alarm.wav");
    delay(1000);
    tmrpcm.stopPlayback();
}

void AudioPlayer::startAlarm()
{
    if (isStarted)
    {
        return;
    }

    isStarted = true;
    tmrpcm.setVolume(7);
    tmrpcm.loop(true);
    tmrpcm.play("alarm.wav");
}

void AudioPlayer::stopAlarm()
{
    if (!isStarted)
    {
        return;
    }

    isStarted = false;
    tmrpcm.stopPlayback();
}