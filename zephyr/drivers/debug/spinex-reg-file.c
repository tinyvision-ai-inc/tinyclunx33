/*
 * Debug Port Driver for Zephyr RTOS
 * Reads register configuration from devicetree and provides shell commands
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>

#include <zephyr/shell/shell.h>
#include <zephyr/sys/util.h>
#include <zephyr/devicetree.h>

#define DT_DRV_COMPAT spinex_reg_file

struct reg_file_reg {
    uint32_t addr;
    const char *name;
    const char *doc;
    uint32_t last_value;
};

struct reg_file_config {
    struct reg_file_reg *regs;
    size_t num_regs;
};

struct reg_file_data {
    const struct device *dev;
};



/* Helper macro to count reg entries */
#define REG_FILE_REG_COUNT(inst) \
    (DT_INST_REG_SIZE(inst) / sizeof(uint32_t))

/* Read a 32-bit register */
static inline uint32_t reg_file_read_reg(uint32_t addr)
{
    return sys_read32(addr);
}

/* Update last seen value and return current value */
static uint32_t reg_file_read_and_track(struct reg_file_reg *reg)
{
    uint32_t value = reg_file_read_reg(reg->addr);
    reg->last_value = value;
    return value;
}

static char* pretty_format(int64_t* v) {
   char* units[] = { "Hz", "KHz", "MHz"};
   for(int i = 0;i < sizeof(units)/sizeof(units[0]);i++) {
      if(*v < 9 * 1000) {
      return units[i];
      }
      *v = *v / 1000;
   }
   return "GHz";
}

static int64_t last_dump_time_ms = 0;
/* Shell command: dump all registers */
static int cmd_reg_file_dump(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = device_get_binding(argv[1]);

    if (!dev) {
        shell_error(sh, "Reg file device not found");
        return -ENODEV;
    }

    const struct reg_file_config *cfg = dev->config;

    int64_t current_time = k_uptime_get();
    int64_t delta_time = current_time - last_dump_time_ms;

    shell_print(sh, "\nReg File Register Dump %llu ms %llu (+%lld ms)", current_time, last_dump_time_ms, delta_time);
    shell_print(sh, "========================\n");
    shell_print(sh, "%30s %10s %23s %23s %15s %15s\n", "name", "address", "value", "prior_value", "delta", "hz");

    for (size_t i = 0; i < cfg->num_regs; i++) {
        struct reg_file_reg *reg = &cfg->regs[i];
        uint32_t last_value = reg->last_value;
        uint32_t current = reg_file_read_and_track(reg);
        int32_t delta = current - last_value;

        int64_t hz = delta * 1000ll / delta_time;
        const char* fmt = pretty_format(&hz);
        shell_print(sh, "%30s 0x%08x %10u (0x%08x) %10u (0x%08x) %15d %12lld %4s", reg->name, reg->addr, current, current, last_value, last_value, delta, hz, fmt);
    }

    last_dump_time_ms = current_time;

    return 0;
}

static int cmd_reg_file_clear(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = device_get_binding(argv[1]);

    if (!dev) {
        shell_error(sh, "Reg file device not found");
        return -ENODEV;
    }

    const struct reg_file_config *cfg = dev->config;
    for (size_t i = 0; i < cfg->num_regs; i++) {
        struct reg_file_reg *reg = &cfg->regs[i];
        *(uint32_t*)reg->addr = 0;
    }

    return 0;
}

/* Shell command: read specific register by name */
static int cmd_reg_file_read(const struct shell *sh, size_t argc, char **argv)
{
    if (argc < 2) {
        shell_error(sh, "Usage: reg_file read <reg_name>");
        return -EINVAL;
    }

    const struct device *dev = device_get_binding(argv[1]);

    if (!dev) {
        shell_error(sh, "Reg file device not found");
        return -ENODEV;
    }

    const struct reg_file_config *cfg = dev->config;
    const char *reg_name = argv[1];

    for (size_t i = 0; i < cfg->num_regs; i++) {
        struct reg_file_reg *reg = &cfg->regs[i];

        if (strcmp(reg->name, reg_name) == 0) {
            uint32_t current = reg_file_read_and_track(reg);

            shell_print(sh, "\nRegister: %s", reg->name);
            shell_print(sh, "  Doc:     %s", reg->doc);
            shell_print(sh, "  Address: 0x%08x", reg->addr);
            shell_print(sh, "  Value:   0x%08x (%u)", current, current);
            shell_print(sh, "  Last:    0x%08x (%u)", reg->last_value, reg->last_value);

            return 0;
        }
    }

    shell_error(sh, "Register '%s' not found", reg_name);
    return -EINVAL;
}

uint32_t api = 0;

static bool device_isready(const struct device *dev)
{
	return device_is_ready(dev) && dev->api == &api;
}

static void complete_device(size_t idx, struct shell_static_entry *entry)
{
	const struct device *dev = shell_device_filter(idx, device_isready);

	entry->syntax = (dev != NULL) ? dev->name : NULL;
	entry->handler = NULL;
	entry->help = NULL;
	entry->subcmd = NULL;
}

SHELL_DYNAMIC_CMD_CREATE(reg_file_device, complete_device);

SHELL_STATIC_SUBCMD_SET_CREATE(reg_file_cmds,
    SHELL_CMD_ARG(dump, &reg_file_device, "Dump all reg file registers", cmd_reg_file_dump, 2, 0),
    SHELL_CMD_ARG(clear, &reg_file_device, "Clear all reg file registers", cmd_reg_file_dump, 2, 0),
    SHELL_CMD(read, NULL, "Read specific register by name", cmd_reg_file_read),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(reg_file, &reg_file_cmds, "Register file commands", NULL);

/* Driver initialization */
static int reg_file_init(const struct device *dev)
{
    struct reg_file_data *data = dev->data;
    data->dev = dev;

    printk("Reg File driver initialized\n");
    return 0;
}

#define STRINGIFY2(x) #x
#define STRINGIFY1(x) STRINGIFY2(x)
#define STRINGIFY(x) STRINGIFY1(x)
/* Initialize register array from devicetree */
#define REG_FILE_REG_INIT(idx, inst)					\
  {									\
    .addr = DT_INST_REG_ADDR_BY_IDX(inst, idx),				\
      .name = STRINGIFY(DT_INST_STRING_TOKEN_BY_IDX(inst, reg_names, idx)), \
      .doc = STRINGIFY(DT_INST_STRING_TOKEN_BY_IDX(inst, reg_docs, idx)), \
      .last_value = 0,							\
  },

#define REG_FILE_REGS2(cnt, inst)					\
  static struct reg_file_reg reg_file_regs_##inst[] = {		\
    LISTIFY(cnt, REG_FILE_REG_INIT, (), inst)				\
  };									\
									
  #define REG_FILE_REGS1(cnt, inst) REG_FILE_REGS2(cnt, inst)
  #define REG_FILE_REGS(cnt, inst) REG_FILE_REGS1(cnt, inst)
/* Define driver instance */
#define REG_FILE_DEVICE_INIT(inst)                                    \
  REG_FILE_REGS(DT_NUM_REGS(DT_DRV_INST(inst)), inst)			\
									\
       static const struct reg_file_config reg_file_cfg_##inst = {	\
    .regs = reg_file_regs_##inst,					\
    .num_regs = ARRAY_SIZE(reg_file_regs_##inst),			\
  };									\
									\
  static struct reg_file_data reg_file_data_##inst;			\
    DEVICE_DT_INST_DEFINE(inst,                                         \
                         reg_file_init,                               \
                         NULL,                                          \
                         &reg_file_data_##inst,                       \
                         &reg_file_cfg_##inst,                        \
                         POST_KERNEL,                                   \
                         CONFIG_DEBUG_DRIVER_INIT_PRIORITY,             \
                         &api);

DT_INST_FOREACH_STATUS_OKAY(REG_FILE_DEVICE_INIT)
