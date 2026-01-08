### Coremark sample
This sample is a modified version of this PR: https://github.com/eembc/coremark/pull/68

The main modification is adding an option to relocate code to RAM with zephyr_code_relocate.

Build with `west build -p -c -b tinyclunx33@rev2/rtl_1_<x> tinyclunx33/zephyr/samples/coremark/zephyr/ <Additional Arguments>`

#### Results
|Configuration|Clock Frequency|Relocation|Additional Arguments|Coremark/s|Coremark/MHz
|---|---|---|---|---|---|
|LiteX (RTL 1.1.2)|80MHz|SPI Flash|None|64|0.8|
|LiteX (RTL 1.1.2)|80MHz|SPI Flash|`-DCONFIG_SPEED_OPTIMIZATIONS=y`|68|0.85|
|LiteX (RTL 1.1.2)|80MHz|RAM|`-DCONFIG_CODE_DATA_RELOCATION=y`|68|0.85|
|LiteX (RTL 1.1.2)|80MHz|RAM|`-DCONFIG_CODE_DATA_RELOCATION=y -DCONFIG_SPEED_OPTIMIZATIONS=y`|73|0.912|
|LiteX (RTL 1.1.2)|80MHz|RAM|`-DCONFIG_CODE_DATA_RELOCATION=y -DCONFIG_SPEED_OPTIMIZATIONS=y -DCONFIG_COMPILER_OPT="\"-fno-common -funroll-loops -finline-functions --param max-inline-insns-auto=256\""`|78|0.975|
|SpineX (RTL 1.2.5)|65MHz|SPI Flash|None|50|0.769|
|SpineX (RTL 1.2.5)|65MHz|SPI Flash|`-DCONFIG_SPEED_OPTIMIZATIONS=y`|55|0.846|
|SpineX (RTL 1.2.5)|65MHz|RAM|`-DCONFIG_CODE_DATA_RELOCATION=y`|52|0.8|
|SpineX (RTL 1.2.5)|65MHz|RAM|`-DCONFIG_CODE_DATA_RELOCATION=y -DCONFIG_SPEED_OPTIMIZATIONS=y`|57|0.877|
|SpineX (RTL 1.2.5)|65MHz|RAM|`-DCONFIG_CODE_DATA_RELOCATION=y -DCONFIG_SPEED_OPTIMIZATIONS=y -DCONFIG_COMPILER_OPT="\"-Ofast\""`|61|0.938|
