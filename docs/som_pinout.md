# SoM Pinout {#som_pinout}

## Pinout of the two connectors

The SoM communicates with the outside through twow Hirose DF40C 60-pins
connectors, each with two rows of 30-pins.

| Pin  | Signal       | Pin  | Signal        | Pin  | Signal       | Pin  | Signal          |
|------|--------------|------|---------------|------|--------------|------|-----------------|
| Left connector (J1)                     |||| Right connector (J2)                      ||||
| Left row           || Right row           || Left row           || Right row             ||
| 01   | **GND**      | 02   | **GND**       | 01   | **GND**      | 02   | **GND**         |
| 03.U | SS_RX_P      | 04   | X             | 03.1 | TCK          | 04   | FLASH_SCK       |
| 05.U | SS_RX_N      | 06   | X             | 05.1 | JTAG_EN      | 06   | FLASH_SIO0      |
| 07   | **GND**      | 08   | **GND**       | 07.1 | TDO          | 08   | FLASH_SIO1      |
| 09.U | SS_TX_P      | 10   | X             | 09.1 | TDI          | 10   | FLASH_SIO2      |
| 11.U | SS_TX_N      | 12   | X             | 11.1 | TMS          | 12   | FLASH_SIO3      |
| 13   | **GND**      | 14   | **GND**       | 13.1 | DONE         | 14   | FLASH_SSN       |
| 15.U | USB2_P       | 16   | REFCLK_P      | 15.1 | GPIO_A1_INTN | 16.0 | GPIO1V8_A3      |
| 17.U | USB2_N       | 18   | REFCLK_N      | 17.1 | GPIO_B1      | 18.0 | PROGN           |
| 19   | **GND**      | 20   | **GND**       | 19.1 | GPIO_F1      | 20   | X               |
| 21.3 | DIFF0_P      | 22.3 | DIFF1_PCLK_P  | 21.1 | GPIO_G1      | 22   | X               |
| 23.3 | DIFF0_N      | 24.3 | DIFF1_PCLK_N  | 23.0 | SDA          | 24   | X               |
| 25   | **GND**      | 26   | **GND**       | 25.0 | SCL          | 26   | X               |
| 27.3 | DIFF2_P      | 28.3 | DIFF3_PCLK_P  | 27   | **GND**      | 28   | X               |
| 29.3 | DIFF2_N      | 30.3 | DIFF3_PCLK_N  | 29   | X            | 30   | X               |
| 31   | **GND**      | 32   | **GND**       | 31   | X            | 32   | X               |
| 33.3 | DIFF4_PCLK_P | 34.3 | DIFF5_PCLK_P  | 33   | X            | 34   | X               |
| 35.3 | DIFF4_PCLK_N | 36.3 | DIFF5_PCLK_N  | 35   | X            | 36   | X               |
| 37   | **GND**      | 38   | **GND**       | 37   | X            | 38   | X               |
| 39   | X            | 40   | X             | 39   | X            | 40.0 | CLK2            |
| 41   | X            | 42   | X             | 41   | X            | 42.0 | GPIO1V8_H8_PCLK |
| 43   | **GND**      | 44   | **GND**       | 43   | X            | 44   | **GND**         |
| 45   | X            | 46   | X             | 45   | X            | 46.P | VBUS            |
| 47   | X            | 48   | X             | 47   | **GND**      | 48   | EN              |
| 49   | **GND**      | 50   | **GND**       | 49.0 | AON_OUT      | 50   | **GND**         |
| 51   | X            | 52   | X             | 51.0 | AON_INT      | 52.P | P5V             |
| 53   | X            | 54   | X             | 53   | P1V_AON      | 54.P | P5V             |
| 55   | **GND**      | 56   | **GND**       | 55.0 | P1V8D        | 56.P | P3V3D           |
| 57   | X            | 58   | X             | 57.P | PVDD_1       | 58.P | PVDD_2          |
| 59   | X            | 60   | X             | 59.P | PVDD_3       | 60.P | PVDD_4          |

The meaning of the labels in the "Pin" column is:

- `.P` - Voltage rail providing power
- `.U` - VBUS I/O voltage level
- `.0` - Bank 0 I/O voltage level at 1.8 V
- `.1` - Bank 1 I/O voltage level selectable between 1.2 V and 3.3 V
- `.2` - Bank 2 I/O voltage level selectable between 1.2 V and 1.8 V
- `.3` - Bank 3 I/O voltage level selectable between 1.2 V and 1.8 V

TODO: Document the connectivity variant

## Pinout of the FPGA

| Bank   | Pin | Signal          | Names                 |
|--------|-----|-----------------|-----------------------|
|        |     |                 |                       |
| Bank 0 | C3  | P1V8D           | VCCIO0                |
| Bank 0 | A4  | PROGN           | PT30A, PROGRAMN       |
| Bank 0 | B4  | FLASH_SCK       | PT32A, MCLK           |
| Bank 0 | B3  | FLASH_SSN       | PT32B, MCSN           |
| Bank 0 | D4  | FLASH_SIO0      | PT34A, MOSI, MD0, SD4 |
| Bank 0 | D3  | FLASH_SIO1      | PT34B, MOSI, MD1, SD5 |
| Bank 0 | E4  | FLASH_SIO2      | PT36A, MD2, SD6       |
| Bank 0 | E3  | FLASH_SIO3      | PT36B, MD3, SD7       |
| Bank 0 | C1  | PVDD_1          | VCCIO1                |
| Bank 0 | A1  | GPIO_A1_INITN   | PT38B, INITN          |
| Bank 0 | B1  | GPIO_B1         | PT40B, MCSNO, MSDO    |
| Bank 0 | E2  | SDA             | PT44A, SD3, SDA       |
| Bank 0 | E1  | SCL             | PT44B, SD2, SCL       |
| Bank 0 | F1  | GPIO_F1         | PT46B, PMU_WAKEUP     |
| Bank 0 | G2  | DONE            | PT48A, DONE           |
| Bank 0 | G1  | GPIO_G1         | PT48B                 |
| Bank 0 | A2  | JTAG_EN         | JTAG_EN               |
| Bank 0 | F2  | TCK             | PT46A, TCK, PCLKT1_3, SCLK, PMU_EXT_CLK |
| Bank 0 | B2  | TMS             | PT40A, TMS, PCLKT1_0, SCSN |
| Bank 0 | D2  | TDI             | PT42A, TDI, PCLKT1_1, SSI, SD0, OSC_HI |
| Bank 0 | D1  | TDO             | PT42B, TDO, PCLKT1_2, SSO, SD1 |
|        |     |                 |                       |
| Bank 2 | K4  | P1V8D           | VCCIO2                |
| Bank 2 | N5  | SRAM_DQ0        | PB32A, VREF2_1        |
| Bank 2 | N4  | SRAM_DQ1        | PB32B                 |
| Bank 2 | M5  | SRAM_DQ2        | PB34A, PCLKT2_0       |
| Bank 2 | M4  | SRAM_DQ3        | PB34B, PCLKC2_0       |
| Bank 2 | L4  | SRAM_DQ4        | PB36A                 |
| Bank 2 | L3  | SRAM_DQ5        | PB36B                 |
| Bank 2 | N3  | SRAM_DQ6        | PB38A, PCLKT2_1       |
| Bank 2 | N2  | SRAM_DQ7        | PB38B, PCLKC2_1       |
| Bank 2 | M3  | SRAM_DQ8_CEn    | PB40A                 |
| Bank 2 | M2  | SRAM_DQ9        | PB40B                 |
| Bank 2 | K3  | SRAM_DQ10       | PB42A, PCLKT2_2       |
| Bank 2 | K2  | SRAM_DQ11_RSTn  | PB42B, PCLKC2_2       |
| Bank 2 | J2  | SRAM_DQ12       | PB44A                 |
| Bank 2 | H2  | SRAM_DQ13_VCC   | PB44B                 |
| Bank 2 | J1  | SRAM_DQ14       | PB46A, PCLKT2_3       |
| Bank 2 | H1  | SRAM_DQ15       | PB46B, PCLKC2_3, VREF2_2 |
|        |     |                 |                       |
| Bank 3 | K6  | PVDD_3          | VCCIO3                |
| Bank 3 | N7  | DIFF0_P         | PB8A, VREF3_1         |
| Bank 3 | N6  | DIFF0_N         | PB8B                  |
| Bank 3 | M7  | DIFF1_PCLK_P    | PB10A, PCLKT3_0       |
| Bank 3 | M6  | DIFF1_PCLK_N    | PB10B, PCLKC3_0       |
| Bank 3 | L7  | DIFF2_P         | PB12A                 |
| Bank 3 | L6  | DIFF2_N         | PB12B                 |
| Bank 3 | F7  | DIFF3_PCLK_P    | PB14A, PCLKT3_1       |
| Bank 3 | E6  | DIFF3_PCLK_N    | PB14B, PCLKC3_1       |
| Bank 3 | G7  | DIFF4_PCLK_P    | PB16A, PCLKT3_2       |
| Bank 3 | G6  | DIFF4_PCLK_N    | PB16B, PCLKC3_2       |
| Bank 3 | J6  | DIFF5_PCLK_P    | PB18A, PCLKT3_3, VREF3_2 |
| Bank 3 | H6  | DIFF5_PCLK_N    | PB18B, PCLKC3_3       |
|        |     |                 |                       |
| Bank 4 | E5  | VBUS            | VBUS                  |
| Bank 4 | D7  | USB2_P          | DP                    |
| Bank 4 | E7  | USB2_N          | DM                    |
| Bank 4 | F8  | REFIN_CLK_P     | REFIN_CLK_EXT_P       |
| Bank 4 | E8  | REFIN_CLK_N     | REFIN_CLK_EXT_M       |
| Bank 4 | A6  | SS_TX_N         | TX_M                  |
| Bank 4 | A7  | SS_TX_P         | TX_P                  |
| Bank 4 | B8  | SS_RX_N         | RX_M                  |
| Bank 4 | A8  | SS_RX_P         | RX_P                  |
| Bank 4 | A5  | P3VDD           | AVDD33                |
| Bank 4 | B7  | P1VD            | AVDD                  |
| Bank 4 | C7  | P1VD            | AVDD_TX               |
| Bank 4 | B6  | P1V8D           | AVDD18                |
| Bank 4 | C5  | P1V8D           | AVDD18_TX             |
| Bank 4 | D6  | P1V8D           | AVDD18_COM            |
| Bank 4 | D8  | GND             | AVSS                  |
| Bank 4 | B5  | GND             | AVSS_TX               |
| Bank 4 | C6  | GND             | AVSS                  |
| Bank 4 | D5  | GND             | AVSS_COM              |
| Bank 4 | C8  | GND             | REXT23                |
| Bank 4 | J8  | P1V8D           | VCCIO4                |
| Bank 4 | N8  | SRAM_DQS0_RWDS  | PB4A, LLC_GPLL0T_IN, VREF4_1 |
| Bank 4 | M8  | SRAM_DQS1_GND   | PB4B, LLC_GPLL0C_IN   |
| Bank 4 | H8  | GPIO1V8_H8_PCLK | PB6A, PCLKT4_1        |
| Bank 4 | G8  | SRAM_CLK        | PB6B, PCLKC4_1, VREF4_2 |
| Bank 4 | N1  | AON_INT         | AON_INT               |
| Bank 4 | M1  | AON_OUT         | AON_OUT               |
| Bank 4 | K1  | P1V8_AON        | VCCAUX_AON            |
| Bank 4 | L1  | GND             | VSS_AON               |

## Parts featured

- Hirose [DF40C-60](https://www.hirose.com/en/product/document?clcode=&productname=&series=DF40&documenttype=Guideline&lang=en&documentid=D80_en)
  60 pin connectors (x2)

![](images/tinyclunx33_som_connectors_schematic.png)
