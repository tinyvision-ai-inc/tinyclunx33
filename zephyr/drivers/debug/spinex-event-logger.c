#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/usb/udc.h>
#include <zephyr/drivers/clock_control.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/usb/bos.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/barrier.h>
LOG_MODULE_REGISTER(tx_logger, LOG_LEVEL_DBG);
#include <zephyr/shell/shell.h>

#include <stdlib.h>
#include <stdio.h>

typedef struct GlobalLogger_info_t {
   uint32_t ctrl;
   uint32_t captured_events;
   uint32_t checksum;
   uint32_t sysclk_lsb;
   uint32_t fifo_occupancy;
   uint32_t num_of_events;
   uint32_t inactive_mask;
   uint32_t signature;
   uint32_t dropped_events;
   const uint32_t* event_counter;
} GlobalLogger_info_t;

static GlobalLogger_info_t GlobalLogger_info_get(volatile uint32_t* base) {
  GlobalLogger_info_t rtn = (GlobalLogger_info_t) {
    .ctrl = base[0],
    .captured_events = base[1],
    .checksum = base[5],
    .sysclk_lsb = base[6],
    .fifo_occupancy = base[7],
    .num_of_events = base[8],
    .inactive_mask = base[9],
    .signature = base[12],
    .dropped_events = base[13]
  };
  rtn.event_counter = &base[56/4];
  return rtn;
}

struct spinex_event_logger_dev_config_t {
    uint32_t base;
    uint32_t signature;
};

struct spinex_event_logger_dev_data_t {
    const struct device *dev;

    uint32_t inactive_mask;
    bool do_log;
    uint32_t dropped, captured;
};

static const struct device *global_dev = 0;
#define GLOBAL_IP_ADDRESS ((uint32_t*)(((struct spinex_event_logger_dev_config_t*)global_dev->config)->base))

static int cmd_info(const struct shell *sh, size_t argc, char **argv) {
  GlobalLogger_info_t info = GlobalLogger_info_get(GLOBAL_IP_ADDRESS);

  shell_print(sh, "Info for %p:", (void*)GLOBAL_IP_ADDRESS);
  shell_print(sh, "  ctrl            = %d", info.ctrl);
  shell_print(sh, "  dropped_events  = %d", info.dropped_events);
  shell_print(sh, "  captured_events = %d", info.captured_events);
  shell_print(sh, "  checksum        = 0x%x", info.checksum);
  shell_print(sh, "  sysclk_lsb      = 0x%x", info.sysclk_lsb);
  shell_print(sh, "  fifo_occupancy  = %d", info.fifo_occupancy);
  shell_print(sh, "  inactive_mask   = 0x%x", info.inactive_mask);
  shell_print(sh, "  signature       = 0x%x", info.signature);

  for(int i = 0;i < info.num_of_events;i++) {
    shell_print(sh, "%s [%3d]: %11d", ((info.inactive_mask & (1 << i)) == 0) ? " " : "*", i, info.event_counter[i]);
  }


  return 0;
}

static int cmd_clear(const struct shell *sh, size_t argc, char **argv) {
  GLOBAL_IP_ADDRESS[7] = 0;
  GLOBAL_IP_ADDRESS[13] = 0;
  k_sleep(K_MSEC(10));
  GLOBAL_IP_ADDRESS[7] = 0;
  GLOBAL_IP_ADDRESS[13] = 0;

  return cmd_info(sh, argc, argv);
}
static int cmd_trigger(const struct shell *sh, size_t argc, char **argv) {
    GLOBAL_IP_ADDRESS[8] = 0xffffffff;
    return 0;
}

static int cmd_mask(const struct shell *sh, size_t argc, char **argv) {
  GlobalLogger_info_t info = GlobalLogger_info_get(GLOBAL_IP_ADDRESS);
  if(argc < 2) {
    info.inactive_mask |= 0xffffffff;
  } else {

    for(int i = 1;i < argc;i++) {
      info.inactive_mask |= (1 << atoi(argv[i]));
    }
  }
  shell_print(sh, "New mask %x", info.inactive_mask);
  GLOBAL_IP_ADDRESS[9] = info.inactive_mask;

  return cmd_info(sh, argc, argv);
}


static int cmd_unmask(const struct shell *sh, size_t argc, char **argv) {
  GlobalLogger_info_t info = GlobalLogger_info_get(GLOBAL_IP_ADDRESS);
  if(argc < 2) {
      info.inactive_mask = 0;
  } else {
      for(int i = 1;i < argc;i++) {
        info.inactive_mask &= ~(1 << atoi(argv[i]));
      }
  }

  shell_print(sh, "New mask %x", info.inactive_mask);
  GLOBAL_IP_ADDRESS[9] = info.inactive_mask;

  return cmd_info(sh, argc, argv);
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_tx,
			       SHELL_CMD(info, NULL,
					     "Dump the transacation log info\n"
					     "Usage: info",
					     cmd_info),
			       SHELL_CMD(clear, NULL,
					     "Clear the transacation log info\n"
					     "Usage: clear",
					     cmd_clear),
			       SHELL_CMD(trigger, NULL,
					 "Manually trigger events",
					 cmd_trigger),
 			       SHELL_CMD(mask, NULL,
					 "Mask certain events",
					 cmd_mask),
			       SHELL_CMD(unmask, NULL,
					 "Unmask certain events",
					 cmd_unmask),
			       SHELL_SUBCMD_SET_END);


SHELL_CMD_REGISTER(events, &sub_tx, "Inspect registers and such", NULL);

#define DT_DRV_COMPAT spinex_event_logger

static int spinex_event_logger_init(const struct device *dev)
{
	struct spinex_event_logger_dev_data_t * const dev_data = dev->data;
	int ret = 0;

	dev_data->dev = dev;

	LOG_DBG("Event viewer device dev %p dev_data %p cfg %p", dev, dev_data, dev->config);

	global_dev = dev;
	return ret;
}

#define SPINEX_EVENT_LOGGER_DT_DEVICE_DEFINE(n)					\
	struct spinex_event_logger_dev_data_t spinex_event_logger_dev_data_##n = {\
	};\
	struct spinex_event_logger_dev_config_t spinex_event_logger_dev_config_##n = {\
	.base = DT_INST_REG_ADDR_BY_NAME(n, base),\
	};\
									\
	DEVICE_DT_INST_DEFINE(n, spinex_event_logger_init, NULL,			\
		&spinex_event_logger_dev_data_##n, &spinex_event_logger_dev_config_##n,		\
		POST_KERNEL, CONFIG_DEBUG_DRIVER_INIT_PRIORITY,		\
		0);

DT_INST_FOREACH_STATUS_OKAY(SPINEX_EVENT_LOGGER_DT_DEVICE_DEFINE);
