/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define DT_DRV_COMPAT zephyr_cdc_mute

#include <zephyr/init.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/ring_buffer.h>
#include <zephyr/sys/byteorder.h>

#include <zephyr/usb/usbd.h>
#include <zephyr/usb/usb_ch9.h>
#include <zephyr/usb/class/usb_cdc.h>

#include <zephyr/drivers/usb/udc.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(cdc_mute, CONFIG_USBD_CDC_MUTE_LOG_LEVEL);

#define CDC_ACM_DEFAULT_INT_EP_MPS	16
#define CDC_ACM_INTERVAL_DEFAULT	10000UL
#define CDC_ACM_FS_INT_EP_INTERVAL	USB_FS_INT_EP_INTERVAL(10000U)
#define CDC_ACM_HS_INT_EP_INTERVAL	USB_HS_INT_EP_INTERVAL(10000U)
#define CDC_ACM_SS_INT_EP_INTERVAL	USB_SS_INT_EP_INTERVAL(10000U)

#define CDC_ACM_BULK_EP_MPS		1024
#define CDC_ACM_INT_EP_MPS		64
#define CDC_ACM_INT_INTERVAL		0x0A

struct cdc_acm_desc {
	struct usb_association_descriptor iad;

	struct usb_if_descriptor if0;
	struct cdc_header_descriptor if0_header;
	struct cdc_cm_descriptor if0_cm;
	struct cdc_acm_descriptor if0_acm;
	struct cdc_union_descriptor if0_union;
	struct usb_ep_descriptor if0_fs_int_ep;
	struct usb_ep_descriptor if0_hs_int_ep;
	struct usb_ep_descriptor if0_ss_int_ep;
	struct usb_ss_endpoint_companion_descriptor if0_ss_int_comp;

	struct usb_if_descriptor if1;
	struct usb_ep_descriptor if1_fs_in_ep;
	struct usb_ep_descriptor if1_fs_out_ep;
	struct usb_ep_descriptor if1_hs_in_ep;
	struct usb_ep_descriptor if1_hs_out_ep;
	struct usb_ep_descriptor if1_ss_in_ep;
	struct usb_ss_endpoint_companion_descriptor if1_ss_in_comp;
	struct usb_ep_descriptor if1_ss_out_ep;
	struct usb_ss_endpoint_companion_descriptor if1_ss_out_comp;

	struct usb_desc_header nil_desc;
};

struct cdc_mute_data {
	/* Pointer to the associated USBD class node */
	struct usbd_class_data *c_data;
	/* Pointer to the class interface descriptors */
	struct cdc_acm_desc *const desc;
	const struct usb_desc_header **const fs_desc;
	const struct usb_desc_header **const hs_desc;
	const struct usb_desc_header **const ss_desc;
};

static int usbd_cdc_mute_cth(struct usbd_class_data *const c_data,
			    const struct usb_setup_packet *const setup,
			    struct net_buf *const buf)
{
	errno  = -ENOTSUP;
	return 0;
}

static int usbd_cdc_mute_ctd(struct usbd_class_data *const c_data,
			    const struct usb_setup_packet *const setup,
			    const struct net_buf *const buf)
{
	errno  = -ENOTSUP;
	return 0;
}

static int usbd_cdc_mute_preinit(const struct device *dev)
{
	return 0;
}

static int usbd_cdc_mute_init(struct usbd_class_data *const c_data)
{
	const struct device *dev = usbd_class_get_private(c_data);
	struct cdc_mute_data *data = dev->data;
	struct cdc_acm_desc *desc = data->desc;

	desc->iad.bFirstInterface = desc->if0.bInterfaceNumber;
	desc->if0_union.bControlInterface = desc->if0.bInterfaceNumber;
	desc->if0_union.bSubordinateInterface0 = desc->if1.bInterfaceNumber;
	return 0;
}

static int usbd_cdc_mute_request(struct usbd_class_data *const c_data,
				struct net_buf *buf, int err)
{
	return -ENOTSUP;
}

static void *usbd_cdc_mute_get_desc(struct usbd_class_data *const c_data,
				   const enum usbd_speed speed)
{
	const struct device *dev = usbd_class_get_private(c_data);
	struct cdc_mute_data *data = dev->data;

	switch (speed) {
	case USBD_SPEED_SS:
		return data->ss_desc;
	case USBD_SPEED_HS:
		return data->hs_desc;
	default:
		return data->fs_desc;
	}
}

struct usbd_class_api usbd_cdc_mute_api = {
	.request = usbd_cdc_mute_request,
	.control_to_host = usbd_cdc_mute_cth,
	.control_to_dev = usbd_cdc_mute_ctd,
	.init = usbd_cdc_mute_init,
	.get_desc = usbd_cdc_mute_get_desc,
};

#define CDC_ACM_DEFINE_DESCRIPTOR(n)						\
static struct cdc_acm_desc cdc_acm_desc_##n = {					\
	.iad = {								\
		.bLength = sizeof(struct usb_association_descriptor),		\
		.bDescriptorType = USB_DESC_INTERFACE_ASSOC,			\
		.bFirstInterface = 0,						\
		.bInterfaceCount = 0x02,					\
		.bFunctionClass = USB_BCC_CDC_CONTROL,				\
		.bFunctionSubClass = ACM_SUBCLASS,				\
		.bFunctionProtocol = 0,						\
		.iFunction = 0,							\
	},									\
										\
	.if0 = {								\
		.bLength = sizeof(struct usb_if_descriptor),			\
		.bDescriptorType = USB_DESC_INTERFACE,				\
		.bInterfaceNumber = 0,						\
		.bAlternateSetting = 0,						\
		.bNumEndpoints = 1,						\
		.bInterfaceClass = USB_BCC_CDC_CONTROL,				\
		.bInterfaceSubClass = ACM_SUBCLASS,				\
		.bInterfaceProtocol = 0,					\
		.iInterface = 0,						\
	},									\
										\
	.if0_header = {								\
		.bFunctionLength = sizeof(struct cdc_header_descriptor),	\
		.bDescriptorType = USB_DESC_CS_INTERFACE,			\
		.bDescriptorSubtype = HEADER_FUNC_DESC,				\
		.bcdCDC = sys_cpu_to_le16(USB_SRN_1_1),				\
	},									\
										\
	.if0_cm = {								\
		.bFunctionLength = sizeof(struct cdc_cm_descriptor),		\
		.bDescriptorType = USB_DESC_CS_INTERFACE,			\
		.bDescriptorSubtype = CALL_MANAGEMENT_FUNC_DESC,		\
		.bmCapabilities = 0,						\
		.bDataInterface = 1,						\
	},									\
										\
	.if0_acm = {								\
		.bFunctionLength = sizeof(struct cdc_acm_descriptor),		\
		.bDescriptorType = USB_DESC_CS_INTERFACE,			\
		.bDescriptorSubtype = ACM_FUNC_DESC,				\
		/* See CDC PSTN Subclass Chapter 5.3.2 */			\
		.bmCapabilities = BIT(1),					\
	},									\
										\
	.if0_union = {								\
		.bFunctionLength = sizeof(struct cdc_union_descriptor),		\
		.bDescriptorType = USB_DESC_CS_INTERFACE,			\
		.bDescriptorSubtype = UNION_FUNC_DESC,				\
		.bControlInterface = 0,						\
		.bSubordinateInterface0 = 1,					\
	},									\
										\
	.if0_fs_int_ep = {							\
		.bLength = sizeof(struct usb_ep_descriptor),			\
		.bDescriptorType = USB_DESC_ENDPOINT,				\
		.bEndpointAddress = 0x81,					\
		.bmAttributes = USB_EP_TYPE_INTERRUPT,				\
		.wMaxPacketSize = sys_cpu_to_le16(CDC_ACM_DEFAULT_INT_EP_MPS),	\
		.bInterval = CDC_ACM_FS_INT_EP_INTERVAL,			\
	},									\
										\
	.if0_hs_int_ep = {							\
		.bLength = sizeof(struct usb_ep_descriptor),			\
		.bDescriptorType = USB_DESC_ENDPOINT,				\
		.bEndpointAddress = 0x81,					\
		.bmAttributes = USB_EP_TYPE_INTERRUPT,				\
		.wMaxPacketSize = sys_cpu_to_le16(CDC_ACM_DEFAULT_INT_EP_MPS),	\
		.bInterval = CDC_ACM_HS_INT_EP_INTERVAL,			\
	},									\
										\
	.if0_ss_int_ep = {							\
		.bLength = sizeof(struct usb_ep_descriptor),			\
		.bDescriptorType = USB_DESC_ENDPOINT,				\
		.bEndpointAddress = 0x81,					\
		.bmAttributes = USB_EP_TYPE_INTERRUPT,				\
		.wMaxPacketSize = sys_cpu_to_le16(CDC_ACM_DEFAULT_INT_EP_MPS),	\
		.bInterval = CDC_ACM_SS_INT_EP_INTERVAL,			\
	},									\
										\
	.if0_ss_int_comp = {							\
		.bLength = sizeof(struct usb_ss_endpoint_companion_descriptor),	\
		.bDescriptorType = USB_DESC_ENDPOINT_COMPANION,			\
		.bMaxBurst = 0,							\
		.bmAttributes = 0,						\
		.wBytesPerInterval = 0,						\
	},									\
										\
	.if1 = {								\
		.bLength = sizeof(struct usb_if_descriptor),			\
		.bDescriptorType = USB_DESC_INTERFACE,				\
		.bInterfaceNumber = 1,						\
		.bAlternateSetting = 0,						\
		.bNumEndpoints = 2,						\
		.bInterfaceClass = USB_BCC_CDC_DATA,				\
		.bInterfaceSubClass = 0,					\
		.bInterfaceProtocol = 0,					\
		.iInterface = 0,						\
	},									\
										\
	.if1_fs_in_ep = {							\
		.bLength = sizeof(struct usb_ep_descriptor),			\
		.bDescriptorType = USB_DESC_ENDPOINT,				\
		.bEndpointAddress = 0x82,					\
		.bmAttributes = USB_EP_TYPE_BULK,				\
		.wMaxPacketSize = sys_cpu_to_le16(64U),				\
		.bInterval = 0,							\
	},									\
										\
	.if1_fs_out_ep = {							\
		.bLength = sizeof(struct usb_ep_descriptor),			\
		.bDescriptorType = USB_DESC_ENDPOINT,				\
		.bEndpointAddress = 0x01,					\
		.bmAttributes = USB_EP_TYPE_BULK,				\
		.wMaxPacketSize = sys_cpu_to_le16(64U),				\
		.bInterval = 0,							\
	},									\
										\
	.if1_hs_in_ep = {							\
		.bLength = sizeof(struct usb_ep_descriptor),			\
		.bDescriptorType = USB_DESC_ENDPOINT,				\
		.bEndpointAddress = 0x82,					\
		.bmAttributes = USB_EP_TYPE_BULK,				\
		.wMaxPacketSize = sys_cpu_to_le16(512U),			\
		.bInterval = 0,							\
	},									\
										\
	.if1_hs_out_ep = {							\
		.bLength = sizeof(struct usb_ep_descriptor),			\
		.bDescriptorType = USB_DESC_ENDPOINT,				\
		.bEndpointAddress = 0x01,					\
		.bmAttributes = USB_EP_TYPE_BULK,				\
		.wMaxPacketSize = sys_cpu_to_le16(512U),			\
		.bInterval = 0,							\
	},									\
										\
	.if1_ss_in_ep = {							\
		.bLength = sizeof(struct usb_ep_descriptor),			\
		.bDescriptorType = USB_DESC_ENDPOINT,				\
		.bEndpointAddress = 0x82,					\
		.bmAttributes = USB_EP_TYPE_BULK,				\
		.wMaxPacketSize = sys_cpu_to_le16(1024U),			\
		.bInterval = 0,							\
	},									\
										\
	.if1_ss_in_comp = {							\
		.bLength = sizeof(struct usb_ss_endpoint_companion_descriptor),	\
		.bDescriptorType = USB_DESC_ENDPOINT_COMPANION,			\
		.bMaxBurst = 15,						\
		.bmAttributes = 0,						\
		.wBytesPerInterval = 0,						\
	},									\
										\
	.if1_ss_out_ep = {							\
		.bLength = sizeof(struct usb_ep_descriptor),			\
		.bDescriptorType = USB_DESC_ENDPOINT,				\
		.bEndpointAddress = 0x01,					\
		.bmAttributes = USB_EP_TYPE_BULK,				\
		.wMaxPacketSize = sys_cpu_to_le16(1024U),			\
		.bInterval = 0,							\
	},									\
										\
	.if1_ss_out_comp = {							\
		.bLength = sizeof(struct usb_ss_endpoint_companion_descriptor),	\
		.bDescriptorType = USB_DESC_ENDPOINT_COMPANION,			\
		.bMaxBurst = 15,						\
		.bmAttributes = 0,						\
		.wBytesPerInterval = 0,						\
	},									\
										\
	.nil_desc = {								\
		.bLength = 0,							\
		.bDescriptorType = 0,						\
	},									\
};										\
										\
const static struct usb_desc_header *cdc_acm_fs_desc_##n[] = {			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.iad,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_header,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_cm,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_acm,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_union,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_fs_int_ep,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if1,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if1_fs_in_ep,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if1_fs_out_ep,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.nil_desc,			\
};										\
										\
const static struct usb_desc_header *cdc_acm_hs_desc_##n[] = {			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.iad,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_header,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_cm,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_acm,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_union,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_hs_int_ep,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if1,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if1_hs_in_ep,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if1_hs_out_ep,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.nil_desc,			\
};										\
										\
const static struct usb_desc_header *cdc_acm_ss_desc_##n[] = {			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.iad,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_header,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_cm,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_acm,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_union,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_ss_int_ep,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if0_ss_int_comp,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if1,			\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if1_ss_in_ep,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if1_ss_in_comp,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if1_ss_out_ep,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.if1_ss_out_comp,		\
	(struct usb_desc_header *) &cdc_acm_desc_##n.nil_desc,			\
}

#define USBD_CDC_MUTE_DT_DEVICE_DEFINE(n)					\
	BUILD_ASSERT(DT_INST_ON_BUS(n, usb),					\
		     "node " DT_NODE_PATH(DT_DRV_INST(n))			\
		     " is not assigned to a USB device controller");		\
										\
	CDC_ACM_DEFINE_DESCRIPTOR(n);						\
										\
	USBD_DEFINE_CLASS(cdc_mute_##n,						\
			  &usbd_cdc_mute_api,					\
			  (void *)DEVICE_DT_GET(DT_DRV_INST(n)), NULL);		\
										\
	static struct cdc_mute_data data_##n = {					\
		.c_data = &cdc_mute_##n,						\
		.desc = &cdc_acm_desc_##n,					\
		.fs_desc = cdc_acm_fs_desc_##n,					\
		.hs_desc = cdc_acm_hs_desc_##n,					\
		.ss_desc = cdc_acm_ss_desc_##n,					\
	};									\
										\
	DEVICE_DT_INST_DEFINE(n, usbd_cdc_mute_preinit, NULL,			\
		&data_##n, NULL,						\
		POST_KERNEL, CONFIG_SERIAL_INIT_PRIORITY,			\
		NULL);

DT_INST_FOREACH_STATUS_OKAY(USBD_CDC_MUTE_DT_DEVICE_DEFINE);
