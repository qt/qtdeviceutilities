#include "qdroidutils.h"
#include <unistd.h>

#ifdef Q_OS_ANDROID_NO_SDK
#include <cutils/android_reboot.h>
#include <hardware/lights.h>
#else
#include <sys/reboot.h>
#endif

/*!
 * Reboots the system. Does not return.
 *
 * \sa powerOffSystem()
 */
void QDroidUtils::rebootSystem()
{
    sync();
#ifdef Q_OS_ANDROID_NO_SDK
    (void)android_reboot(ANDROID_RB_RESTART, 0, 0);
#else
    reboot(RB_AUTOBOOT);
#endif
    qWarning("reboot returned");
}

/*!
 * Shuts down the system. Does not return.
 *
 * \sa rebootSystem()
 */
void QDroidUtils::powerOffSystem()
{
    sync();
#ifdef Q_OS_ANDROID_NO_SDK
    (void)android_reboot(ANDROID_RB_POWEROFF, 0, 0);
#else
    reboot(RB_POWER_OFF);
#endif
    qWarning("powerOff returned");
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
