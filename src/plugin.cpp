#include <exception>
#include "plugin.hpp"
#include "CiTunesVolumeControl.hpp"

TS3Functions g_ts3Functions;
CiTunesVolumeControl* g_volumeControl = nullptr;

const char* ts3plugin_name()
{
    return "iTunes Volume Control";
}

const char* ts3plugin_version()
{
    return "1.0";
}

int ts3plugin_apiVersion()
{
    return PLUGIN_API_VERSION;
}

const char* ts3plugin_author()
{
    return "McSimp";
}

const char* ts3plugin_description()
{
    return "This plugin will reduce your iTunes volume when someone starts talking in your channel.";
}

void ts3plugin_setFunctionPointers(const struct TS3Functions funcs)
{
    g_ts3Functions = funcs;
}

int ts3plugin_init()
{
    try {
        g_volumeControl = new CiTunesVolumeControl();
        return 0;
    } catch (std::exception& ex) {
        return 1;
    }
}

void ts3plugin_shutdown()
{
    if (g_volumeControl) {
        delete g_volumeControl;
    }
}

void ts3plugin_onTalkStatusChangeEvent(uint64 serverConnectionHandlerID, int status, int isReceivedWhisper, anyID clientID)
{
    if (g_volumeControl) {
        g_volumeControl->OnTalkStatusChangeEvent(status);
    }
}
