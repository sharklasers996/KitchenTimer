#include "Arduino.h"
#include "AudioPlayer.h"
#include <TMRpcm.h>

TMRpcm tmrpcm;
long alarmStartedAt;
int volume = 0;
bool isStarted = false;

void AudioPlayer::init(int audioPin)
{
    tmrpcm.speakerPin = audioPin;
    tmrpcm.setVolume(6);
    tmrpcm.loop(true);
    tmrpcm.play("alarm.wav");
    delay(5000);
    tmrpcm.stopPlayback();
}

void AudioPlayer::play()
{
    if (!isStarted)
    {
        return;
    }

    long duration = millis() - alarmStartedAt;
    if (duration > 2000 && volume < 5)
    {
        alarmStartedAt = millis();
        volume++;
        if (volume >= 7)
        {
            return;
        }

        tmrpcm.setVolume(volume);
    }
}

void AudioPlayer::startAlarm()
{
    if (isStarted)
    {
        return;
    }

    isStarted = true;

    volume = 0;
    alarmStartedAt = millis();
    tmrpcm.setVolume(volume);
    tmrpcm.loop(true);
    tmrpcm.play("alarm.wav");
}

void AudioPlayer::stopAlarm()
{
    tmrpcm.stopPlayback();
    isStarted = false;
}