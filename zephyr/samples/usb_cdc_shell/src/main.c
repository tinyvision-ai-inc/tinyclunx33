#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app_main, LOG_LEVEL_INF);

int app_usb_init(void);

int main(void)
{
	int ret;

	ret = app_usb_init();
	if (ret != 0) {
		LOG_ERR("Failed to initialize USB");
		return ret;
	}

	LOG_INF("Done, exiting main() and letting the USB stack run");

	return 0;
}
