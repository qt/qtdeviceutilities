#include "settingsuiplugin_plugin.h"

#include <qqml.h>

void SettingsuipluginPlugin::registerTypes(const char *uri)
{
    // @uri QtDeviceUtilities.SettingsUI
    qmlRegisterType(QUrl("qrc:/pluginMain.qml"), uri, 1, 0, "SettingsUI");
}

