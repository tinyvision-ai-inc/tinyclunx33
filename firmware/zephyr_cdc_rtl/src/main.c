#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/net_buf.h>
#include <zephyr/drivers/usb/udc.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/usb/class/usb_cdc_raw.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG);

static const struct device *const cdc0_dev = DEVICE_DT_GET(DT_NODELABEL(cdc0));

int app_usb_init(void);

int main(void)
{
	int ret = 0;

	/* Start the USB stack */
	ret = app_usb_init();
	if (ret != 0) {
		LOG_ERR("Failed to initialize USB");
		return ret;
	}

	/* Wait that the user reacts */
	while (!cdc_raw_is_ready(cdc0_dev)) {
		k_sleep(K_MSEC(10));
	}

	LOG_DBG("CDC raw is ready");

	k_sleep(K_FOREVER);
	return 0;
}
