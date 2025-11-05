#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/net_buf.h>
#include <zephyr/drivers/usb/udc.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/usb/class/usb_cdc_raw.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG);

/* Static pool of buffer with no data in it */
NET_BUF_POOL_DEFINE(app_buf_pool, 2, 0, sizeof(struct udc_buf_info), NULL);

/* And then define the actual buffer that is going to be used, with an attribute
 * that makes the linker script place it on a DMA-accessible memory
 */
static __nocache uint8_t input_buf[1024];

static const struct device *const cdc0_dev = DEVICE_DT_GET(DT_NODELABEL(cdc0));

int app_usb_init(void);

static int my_read_callback(const struct device *dev, struct net_buf *buf, int err)
{
	if (err) {
		LOG_ERR("USB read error %d", err);
		return err;
	}

	LOG_DBG("%s: buf=%p data=%p size=%u len=%u", __func__, buf, buf->data, buf->size, buf->len);
	LOG_HEXDUMP_DBG(buf->data, buf->len, "USB read complete");

	/* Convert to uppercase to proove we received and modifed it in the way */
	for (size_t i = 0; i < buf->len; i++) {
		char *s = (char *)buf->data + i;

		if (*s >= 'a' && *s <= 'z') {
			*s += 'A' - 'a';
		}
	}

	/* Once we read something, echo the same buffer back, so it is not
	 * freed now yet... */
	return cdc_raw_write(dev, buf, true);
}

static int my_write_callback(const struct device *dev, struct net_buf *buf, int err)
{
	LOG_DBG("%s: buf=%p data=%p size=%u len=%u", __func__, buf, buf->data, buf->size, buf->len);

	if (err) {
		LOG_ERR("USB write error %d", err);
		return err;
	}

	/* Now that the chain is complete, we can free that buffer, but we can
	 * also re-use it for the next read */
	return cdc_raw_read(dev, buf);
}

int main(void)
{
	struct net_buf *buf;
	int ret = 0;

	/* Glue with the CDC RAW so that we can react to read and write completion. */
	cdc_raw_set_read_callback(cdc0_dev, &my_read_callback);
	cdc_raw_set_write_callback(cdc0_dev, &my_write_callback);

	/* Allocate a buffer with existing data */
	buf = net_buf_alloc_with_data(&app_buf_pool, input_buf, sizeof(input_buf),
		Z_TIMEOUT_NO_WAIT);
	if (buf == NULL) {
		LOG_ERR("failed creating a pre-allocated buffer");
		return -ENOMEM;
	}

	LOG_DBG("%s: buf=%p data=%p size=%u len=%u", __func__, buf, buf->data, buf->size, buf->len);

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

	/* Enqueue a first read request, the rest happens from the callbacks */
	LOG_DBG("%s: buf=%p data=%p size=%u len=%u", __func__, buf, buf->data, buf->size, buf->len);
	ret = cdc_raw_read(cdc0_dev, buf);
	if (ret != 0) {
		LOG_ERR("reading from CDC inteface");
		return ret;
	}

	k_sleep(K_FOREVER);
	return 0;
}
