#include "jdprofile.h"

#include "addons/example.h"

// Edit the string below to match your company name, the device name, and hardware revision.
// Do not change the generated 0x3.... value, as that would break the firmware update process.
FIRMWARE_IDENTIFIER(0x30249c44, "Project Janus XBox Gateway");

void app_init_services() {
    // see jacdac-c/services/jd_services.h for the services that can be
    // enabled here; you can enable zero or more services
    button_init(PA_5, 0, NO_PIN);

    example_custom_function(); // call our custom function
}
