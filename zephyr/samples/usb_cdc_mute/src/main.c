#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG);

int app_usb_init(void);

int main(void)
{
	int ret;

	ret = app_usb_init();
	if (ret != 0) {
		LOG_ERR("Failed to initialize USB");
	}

	return 0;
}
