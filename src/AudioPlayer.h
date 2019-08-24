#include "Arduino.h"
#include <TMRpcm.h>

class AudioPlayer
{
public:
    void init(int audioPin);
    void startAlarm();
    void stopAlarm();
};