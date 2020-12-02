# Generated from settingsui.pro.

#####################################################################
## settingsui Binary:
#####################################################################

qt_add_executable(settingsui
    GUI
    SOURCES
        main.cpp
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::Qml
        Qt::Quick
)

# Resources:
set(settingsuiapp_resource_files
    "AutoScroller.qml"
    "ViewSettings.qml"
    "common/CustomSwitch.qml"
    "common/HandwritingModeButton.qml"
    "main.qml"
    "network/AddressListEntry.qml"
    "network/ComboBoxEntry.qml"
    "network/EditWiredSettings.qml"
    "network/IpAddressTextField.qml"
    "network/NetworkDetails.qml"
    "network/NetworkListView.qml"
    "network/NetworkSettings.qml"
    "network/PassphraseEnter.qml"
    "network/WifiSelectorDelegate.qml"
    "network/WifiSettings.qml"
    "network/WifiSignalMonitor.qml"
    "network/WiredSettings.qml"
    "settingsuiplugin/SettingsDelegate.qml"
    "settingsuiplugin/SettingsHeader.qml"
    "settingsuiplugin/SettingsTitleItem.qml"
    "settingsuiplugin/pluginMain.qml"
)

qt_add_resource(settingsui "settingsuiapp"
    PREFIX
        "/"
    FILES
        ${settingsuiapp_resource_files}
)


#### Keys ignored in scope 1:.:.:settingsui.pro:<TRUE>:
# QML_IMPORT_PATH = <EMPTY>
# TEMPLATE = "app"

## Scopes:
#####################################################################

qt_extend_target(settingsui CONDITION static
    PUBLIC_LIBRARIES
        Qt::Svg
)

#### Keys ignored in scope 2:.:.:settingsui.pro:static:
# QTPLUGIN = "qtvirtualkeyboardplugin"

#### Keys ignored in scope 4:.:.:deployment.pri:android-no-sdk:
# INSTALLS = "target"
# target.path = "/data/user/qt"

#### Keys ignored in scope 6:.:.:deployment.pri:ANDROID:
# INSTALLS = "target"

#### Keys ignored in scope 7:.:.:deployment.pri:TEST_architecture_arch STREQUAL i386:
# target.path = "/libs/x86"

#### Keys ignored in scope 9:.:.:deployment.pri:TEST_architecture_arch STREQUAL arm:
# target.path = "/libs/armeabi-v7a"

#### Keys ignored in scope 10:.:.:deployment.pri:else:
# target.path = "/libs/armeabi"

#### Keys ignored in scope 12:.:.:deployment.pri:UNIX:
# INSTALLS = "target"

#### Keys ignored in scope 14:.:.:deployment.pri:QNX:
# target.path = "/tmp/$${TARGET}/bin"

#### Keys ignored in scope 15:.:.:deployment.pri:else:
# target.path = "$$[QT_INSTALL_BINS]"
