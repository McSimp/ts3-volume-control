#include <Windows.h>
#include <exception>
#include <algorithm>
#include "CiTunesVolumeControl.hpp"
#include "public_definitions.h"

CiTunesVolumeControl::CiTunesVolumeControl()
{
    HRESULT result = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    if (result != S_OK && result != S_FALSE) {
        throw std::exception("Failed to initialize COM library");
    }

    m_currentSpeakers = 0;
    m_piTunes = nullptr;
}

CiTunesVolumeControl::~CiTunesVolumeControl()
{
    if (m_piTunes) {
        m_piTunes->Release();
    }

    CoUninitialize();
}

void CiTunesVolumeControl::InitiTunesInterface()
{
    HRESULT result = CoCreateInstance(CLSID_iTunesApp, NULL, CLSCTX_LOCAL_SERVER, IID_IiTunes, (void**)&m_piTunes);
    if (result != S_OK || !m_piTunes) {
        throw std::exception("Failed to initialize iTunes interface");
    }
}

void CiTunesVolumeControl::AddSpeaker()
{
    m_currentSpeakers++;
}

void CiTunesVolumeControl::RemoveSpeaker()
{
    m_currentSpeakers = std::max(m_currentSpeakers - 1, 0);
}

bool CiTunesVolumeControl::IsSomeoneTalking()
{
    return m_currentSpeakers > 0;
}

void CiTunesVolumeControl::OnTalkStatusChangeEvent(int status)
{
    ITPlayerState state;
    if (FAILED(m_piTunes->get_PlayerState(&state))) {
        return;
    }

    if (status == STATUS_TALKING) {
        AddSpeaker();
    } else if (status == STATUS_NOT_TALKING) {
        RemoveSpeaker();
    }

    // If iTunes is playing and someone starts talking when no one else is,
    // take note of the volume level we currently have set, then reduce the volume.
    if (state == ITPlayerStatePlaying && status == STATUS_TALKING && m_currentSpeakers == 1) {
        // Record the volume iTunes is at before we turn it down.
        m_piTunes->get_SoundVolume(&m_previousVolume);

        // Set the new volume
        m_piTunes->put_SoundVolume(25);
        m_volumeModified = true;
    }

    // If everyone has stopped talking and we have previously modified the volume,
    // restore it to what it was before.
    if (!IsSomeoneTalking() && m_volumeModified) {
        m_piTunes->put_SoundVolume(m_previousVolume);
        m_volumeModified = false;
    }
}