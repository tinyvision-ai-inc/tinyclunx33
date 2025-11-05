legend = [
    ("NC : Not Connected", "nc"),
    ("GPIO: ",  "gpio"),
    ("SDA: Serial Data",    "gpio"),
    ("SCL: Serial Clock",  "gpio"),
    ("Ground pin",          "gnd"),
    ("Power pin",           "pwr"),
]

def double(label, type, x):
    return (label, type, {"body": {"width": 166, "x": x}})

def row_2_2(name1, type1, name2, type2):
    return [
        (name1, type1, {"body": {"width": 166, "x": 6}}),
        (name2, type2, {"body": {"width": 166, "x": 20}}),
    ]


def row_1_1_1_1(name1, type1, name2, type2, name3, type3, name4, type4):
    return [
        (name1, type1),
        (name2, type2),
        (name3, type3, {"body": {"x": 20}}),
        (name4, type4),
    ]

def row_1_1(name1, type1, name2, type2):
    return [
        (name1, type1, {"body": {"width": 166, "x": 57}}),
        (name2, type2, {"body": {"width": 166, "x": 60}}),
    ]

def row_1(name1, type1):
    return [
        (name1, type1, {"body": {"width": 166, "x": 57}})
    ]


def row_2_1_1(name1, type1, name2, type2, name3, type3):
    return [
        (name1, type1, {"body": {"width": 166, "x": 6}}),
        (name2, type2, {"body": {"x": 20}}),
        (name3, type3),
    ]

def row_1_1_2(name1, type1, name2, type2, name3, type3, name4, type4):
    return [
        (name1, type1, {"body": {"x": 6}}),
        (name2, type2, {"body": {"x": 6}}),
        (name3, type3, {"body": {"width": 166, "x": 20}}),
    ]

# The pins will appear swapped left/right

right_header_heading = [
    row_1_1(
        "INNER ROW","l", "OUTTER ROW","l"
    ),
]

left_header_heading = [
    row_1_1(
        "INNER ROW","l", "OUTTER ROW","l"
    ),
]

right_header = [
    row_1_1(
        "NC","nc", "GPIO_G1","gpio"
    ),
    row_1_1(
        "NC","nc", "NC","nc"
    ),
    row_1_1(
        "SCL33","gpio", "SCL","gpio"
    ),
    row_1_1(
        "SDA33","gpio", "SDA","gpio"
    ),
    row_1_1(
        "GND","gnd", "GND","gnd"
    ),
    row_1_1(
        "3V3","pwr", "VDD","pwr"
    ),
]

right_header_mid = [
    row_1_1(
        "DONE","gpio", "NC","nc"
    ),
    row_1_1(
        "NC","nc", "NC","nc"
    ),
    row_1_1(
        "GPIO_B1","gpio", "A_OUT","gpio"
    ),
    row_1_1(
        "GPIO_F1","gpio", "A_INT","gpio"
    ),
    row_1_1(
        "GND","gnd", "GND","gnd"
    ),
    row_1_1(
        "VDD","pwr", "1V8","pwr"
    ),
]

right_header_low = [
    row_1_1(
        "FLASH_SSN","gpio", "GPIO1V8_H8_PCLK","gpio"
    ),
    row_1_1(
        "FLASH_SIO0","gpio", "PROGN","gpio"
    ),
    row_1_1(
        "FLASH_SIO1","gpio", "FLASH_SIO2","gpio"
    ),
    row_1_1(
        "FLASH_SCK","gpio", "FLASH_SIO3","gpio"
    ),
    row_1_1(
        "GND","gnd", "GND","gnd"
    ),
    row_1_1(
        "1V8","pwr", "1V8","pwr"
    ),
]

right_jumper_j7 = [
    row_1_1(
        "1V2","pwr", "3V3_SoM","pwr"
    ),
    row_1_1(
        "VDD_2","pwr", "VDD_1","pwr"
    ),
    row_1_1(
        "1V8","pwr", "1V8","pwr"
    ),
]

right_jumper_j10 = [
    row_1_1(
        "GPIO_A1_INITN","gpio", "GND","gnd"
    ),
    row_1_1(
        "JTAG_EN","gpio", "VDD_1","pwr"
    ),
    row_1_1(
        "FTDI_ENB","gpio", "GND","gnd"
    ),
]

left_jumper_j13 = [
    row_1_1(
        "1V8","pwr", "NC","nc"
    ),
    row_1_1(
        "VDD_3","pwr", "CLK_2","gpio"
    ),
    row_1_1(
        "1V2","pwr", "GPIO1V8_H8","gpio"
    ),
]

left_jumper_j11 = [
    row_1(
        "5V","pwr"
    ),
    row_1(
        "EN","gpio"
    ),
    row_1(
        "AON_OUT_5V","pwr"
    ),
]

left_jumper_j12 = [
    row_1(
        "USB_I2C_EN","gpio"
    ),
]


title = "<tspan class='h1'>tinyCLUNX33 </tspan>"

description = """
    Devkit is a carrier for the tinyCLUNX33 SoM that 
    eases product development and debugging
    """
