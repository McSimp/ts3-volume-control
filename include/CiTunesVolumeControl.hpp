#ifndef CITUNESVOLUMECONTROL_HPP
#define CITUNESVOUMECONTROL_HPP

#include "iTunesCOMInterface.h"

class CiTunesVolumeControl
{
public:
    CiTunesVolumeControl();
    ~CiTunesVolumeControl();
    void InitiTunesInterface();
    void AddSpeaker();
    void RemoveSpeaker();
    bool IsSomeoneTalking();
    void OnTalkStatusChangeEvent(int status);

private:
    IiTunes* m_piTunes;
    int m_currentSpeakers;
    long m_previousVolume;
    bool m_volumeModified;
};

#endif