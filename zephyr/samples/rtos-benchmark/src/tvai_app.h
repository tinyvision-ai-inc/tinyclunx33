/* USB API */

int app_usb_init(void);

/* USB UVC API */

struct app_uvc_data {
	const struct device *uvc_dev;
	const struct device *video_dev;
};

int app_uvc_init(struct app_uvc_data *data);
