### rtos-benchmark
This sample is https://github.com/zephyrproject-rtos/rtos-benchmark/

Build with `CONF_FILE=src/zephyr/prj.tinyclunx33.conf west build -p -c -b tinyclunx33@rev2/rtl_1_<x> tinyclunx33/zephyr/samples/rtos-benchmark/ -DRTOS=zephyr -DCONFIG_SPEED_OPTIMIZATIONS=y`

#### Results

LiteX (RTL 1.1.2) 80 MHz `-DCONFIG_SPEED_OPTIMIZATIONS=y`

```
** Thread stats [avg, min, max] in nanoseconds **
 Spawn (no context switch)               : 148703, 143444, 403074
 Create (no context switch)              : 119925, 119925, 140962
 Start  (no context switch)              :  71395,  71345, 327296
 Suspend (no context switch)             :  40679,  40679,  43950
 Resume (no context switch)              :  49567,  49567,  59641
 Spawn (context switch)                  : 290962, 290950, 486753
 Start  (context switch)                 : 241839, 229444, 437629
 Suspend (context switch)                : 132333, 132320, 335345
 Resume (context switch)                 : 182901, 182864, 390506
 Terminate (context switch)              : 133148, 133123, 335345
** Mutex Stats [avg, min, max] in nanoseconds **
 Lock (no owner)                         :  12185,  12185,  34814
 Unlock (no waiters)                     :  17530,  17530,  37382
 Recursive lock                          :  12333,  12333,  34296
 Recursive unlock                        :   8407,   8407,  27543
 Unlock with unpend (no context switch)  : 105518,  41419, 112037
 Unlock with unpend (context switch)     : 269061, 269000, 475345
 Pend (no priority inheritance)          : 198543, 198506, 404814
 Pend (priority inheritance)             : 249493, 249444, 451679
** Semaphore stats [avg, min, max] in nanoseconds **
 Take (context switch)                   : 209827, 206049, 413617
 Give (context switch)                   : 207209, 207172, 404975
** Semaphore stats [avg, min, max] in nanoseconds **
 Give (no context switch)                :  19925,  19901, 285876
 Take (no context switch)                :  22493,  22493,  40345
** Yield stats [avg, min, max] in nanoseconds **
 Yield (no context switch)               :  39395,  39395,  48679
 Yield (context switch)                  : 120679, 120259, 1082679
** Allocation stats [avg, min, max] in nanoseconds **
 Malloc                                  :  72888,  72876, 131839
 Free                                    :  78086,  78037, 338814
** Message queue stats [avg, min, max] in nanoseconds **
 Create                                  :    n/a,    n/a,    n/a
 Send (no context switch)                :    n/a,    n/a,    n/a
 Receive (no context switch)             :    n/a,    n/a,    n/a
 Send (context switch)                   :    n/a,    n/a,    n/a
 Receive (context switch)                :    n/a,    n/a,    n/a
** Interrupt Stats [avg, min, max] in nanoseconds **
 Latency                                 :  43370,  36049,  43382
```

SpineX (RTL 1.2.5) 65MHz `-DCONFIG_SPEED_OPTIMIZATIONS=y`

```
** Thread stats [avg, min, max] in nanoseconds **
 Spawn (no context switch)               : 186815, 186784, 520353
 Create (no context switch)              : 157476, 157446, 490938
 Start  (no context switch)              :  75492,  75492,  85415
 Suspend (no context switch)             :  42492,  42492,  49107
 Resume (no context switch)              :  50876,  50876,  50876
 Spawn (context switch)                  : 346015, 345861, 649430
 Start  (context switch)                 : 285292, 285230, 572261
 Suspend (context switch)                : 190600, 190538, 470738
 Resume (context switch)                 : 202953, 202953, 229569
 Terminate (context switch)              : 183661, 183553, 470630
** Mutex Stats [avg, min, max] in nanoseconds **
 Lock (no owner)                         :  11846,  11846,  51692
 Unlock (no waiters)                     :  28553,  28553,  35200
 Recursive lock                          :  12061,  12030, 315507
 Recursive unlock                        :  21123,  21123,  41107
 Unlock with unpend (no context switch)  : 123969, 120630, 133907
 Unlock with unpend (context switch)     : 312692, 309415, 352692
 Pend (no priority inheritance)          : 260492, 260400, 543861
 Pend (priority inheritance)             : 323323, 323215, 607046
** Semaphore stats [avg, min, max] in nanoseconds **
 Take (context switch)                   : 270169, 270107, 546969
 Give (context switch)                   : 229246, 229138, 509476
** Semaphore stats [avg, min, max] in nanoseconds **
 Give (no context switch)                :  14723,  14723,  44676
 Take (no context switch)                :   4907,   4907,  38138
** Yield stats [avg, min, max] in nanoseconds **
 Yield (no context switch)               :  55569,  55569,  62215
 Yield (context switch)                  : 167753, 167323, 1407384
** Allocation stats [avg, min, max] in nanoseconds **
 Malloc                                  : 125692, 125615, 508553
 Free                                    : 123200, 123200, 169753
** Message queue stats [avg, min, max] in nanoseconds **
 Create                                  :    n/a,    n/a,    n/a
 Send (no context switch)                :    n/a,    n/a,    n/a
 Receive (no context switch)             :    n/a,    n/a,    n/a
 Send (context switch)                   :    n/a,    n/a,    n/a
 Receive (context switch)                :    n/a,    n/a,    n/a
** Interrupt Stats [avg, min, max] in nanoseconds **
 Latency                                 :  65400,  52523,  65415
 ```
