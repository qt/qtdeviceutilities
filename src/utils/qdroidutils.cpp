#include "qdroidutils.h"

#ifdef Q_OS_ANDROID_NO_SDK
#include <unistd.h>
#include <cutils/android_reboot.h>
#include <hardware/lights.h>
#endif

/*!
 * Reboots the system. Does not return.
 *
 * \sa powerOffSystem()
 */
void QDroidUtils::rebootSystem()
{
#ifdef Q_OS_ANDROID_NO_SDK
    sync();
    (void)android_reboot(ANDROID_RB_RESTART, 0, 0);
    qWarning("android_reboot returned");
#else
    qDebug("QDroidUtils::rebootSystem()");
#endif
}

/*!
 * Shuts down the system. Does not return.
 *
 * \sa rebootSystem()
 */
void QDroidUtils::powerOffSystem()
{
#ifdef Q_OS_ANDROID_NO_SDK
    sync();
    (void)android_reboot(ANDROID_RB_POWEROFF, 0, 0);
    qWarning("android_reboot returned");
#else
    qDebug("QDroidUtils::powerOffSystem()");
#endif
}

/*!
 * Sets the display brightness (i.e. the intensity of the backlight)
 * to \a value. A value of 255 requests maximum brightness, while 0 requests
 * minimum (typically, the backlight turned off).
 *
 * Returns true on success.
 */
//### TBD: add the user/sensor setting as parameter!
bool QDroidUtils::setDisplayBrightness(quint8 value)
{
#ifdef Q_OS_ANDROID_NO_SDK
    const struct hw_module_t* module = 0;
    if (hw_get_module(LIGHTS_HARDWARE_MODULE_ID, &module))
        return false;
    if (!module || !module->methods || !module->methods->open)
        return false;

    struct light_device_t* device = 0;
    if (module->methods->open(module, LIGHT_ID_BACKLIGHT, (struct hw_device_t**)&device))
        return false;
    if (!device || !device->set_light || !device->common.close)
        return false;

    struct light_state_t state;
    memset(&state, 0, sizeof(light_state_t));
    state.color = 0xff000000 | (value << 16) | (value << 8) | value;
    if (!device->set_light(device, &state))
        return false;

    device->common.close(&device->common);
#else
    qDebug("QDroidUtils::setDisplayBrightness(%i)", value);
#endif
    return true;
}
