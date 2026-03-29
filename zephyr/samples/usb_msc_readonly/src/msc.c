/*
 * Copyright (c) 2016 Intel Corporation.
 * Copyright (c) 2019-2020 Nordic Semiconductor ASA
 * Copyright (c) tinyVision.ai Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include <zephyr/fs/fs.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/usb/class/usbd_msc.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/toolchain.h>

#include <ff.h>

LOG_MODULE_REGISTER(app_msc, LOG_LEVEL_INF);

#if !defined(CONFIG_DISK_DRIVER_FLASH)
#error No supported disk driver enabled
#endif

#define STORAGE_PARTITION_ID FIXED_PARTITION_ID(storage_partition)

static struct fs_mount_t fs_mnt;

USBD_DEFINE_MSC_LUN(nand, "NAND", "Zephyr", "FlashDisk", "0.00");

static int setup_flash(struct fs_mount_t *mnt) {
  int rc = 0;
  unsigned int id;
  const struct flash_area *pfa;

  mnt->storage_dev = (void *)STORAGE_PARTITION_ID;
  id = STORAGE_PARTITION_ID;

  rc = flash_area_open(id, &pfa);
  if (rc < 0) {
    flash_area_close(pfa);
  }

  return rc;
}

static int mount_app_fs(struct fs_mount_t *mnt) {
  int rc;

  static FATFS fat_fs;

  mnt->type = FS_FATFS;
  mnt->fs_data = &fat_fs;
  mnt->mnt_point = "/NAND:";

  rc = fs_mount(mnt);

  return rc;
}

int app_msc_init(void) {
  struct fs_mount_t *mp = &fs_mnt;
  int rc;

  rc = setup_flash(mp);
  if (rc < 0) {
    LOG_ERR("Failed to setup flash area");
    return rc;
  }

  if (!IS_ENABLED(CONFIG_FAT_FILESYSTEM_ELM)) {
    LOG_INF("No file system selected");
    return rc;
  }

  rc = mount_app_fs(mp);
  if (rc < 0) {
    LOG_ERR("Failed to mount filesystem");
    return rc;
  }

  return 0;
}
