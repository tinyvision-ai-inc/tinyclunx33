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
        "GPIO_4","gpio", "GPIO_7","gpio"
    ),
    row_1_1(
        "GPIO_3","gpio", "GPIO_6","gpio"
    ),
    row_1_1(
        "GPIO_2","gpio", "GPIO_5","gpio"
    ),
    row_1_1(
        "GPIO_1","gpio", "SCL","gpio"
    ),
    row_1_1(
        "GPIO_0","gpio", "SDA","gpio"
    ),
    row_1_1(
        "GPIO_IRQ","gpio", "GND","gnd"
    ),
]

right_header_mid = [
    row_1_1(
        "DONE","gpio", "A_OUT_5V","gpio"
    ),
    row_1_1(
        "GPIO_G1","gpio", "EN","gpio"
    ),
    row_1_1(
        "GPIO_B1","gpio", "AON_OUT","gpio"
    ),
    row_1_1(
        "GPIO_F1","gpio", "AON_INT","gpio"
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
        "FLASH_SSN","gpio", "GPIO_A3","gpio"
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
        "1V2","pwr", "3V3","pwr"
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
        "CLK_60M","gpio", "CLK_0","gpio"
    ),
    row_1_1(
        "GPIO_H8","gpio", "GPIO_H8","gpio"
    ),
    row_1_1(
        "CLK_2","gpio", "CLK_1","gpio"
    ),
]

left_jumper_j13 = [
    row_1_1(
        "1V8","pwr", "1V8","pwr"
    ),
    row_1_1(
        "VDD_3","pwr", "VDD_4","pwr"
    ),
    row_1_1(
        "1V2","pwr", "1V2","pwr"
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

left_jumper_j17 = [
    row_1(
        "FTDI Power Measurement","gpio"
    ),
]

left_jumper_j18 = [
    # row_1_1(
    #     "1V8","pwr", "NC","nc"
    # ),
    # row_1_1(
    #     "VDD_3","pwr", "CLK_2","gpio"
    # ),
    # row_1_1(
    #     "1V2","pwr", "GPIO1V8_H8","gpio"
    # ),
]

left_jumper_j19 = [
    # row_1_1(
    #     "1V8","pwr", "NC","nc"
    # ),
    # row_1_1(
    #     "VDD_3","pwr", "CLK_2","gpio"
    # ),
    # row_1_1(
    #     "1V2","pwr", "GPIO1V8_H8","gpio"
    # ),
]

left_jumper_j21 = [
    [
        ("VDD_1", "gpio",{"body": {"width": 166, "x": 57}})
    ],
    [
        ("JTAG_EN", "gpio", {"body": {"width": 166, "x": 57}})
    ],
    [
        ("FT_JTAG_EN", "gpio", {"body": {"width": 166, "x": 57}})
    ],
]

title = "<tspan class='h1'>tinyCLUNX33 </tspan>"

description = """
    Devkit is a carrier for the tinyCLUNX33 SoM that 
    eases product development and debugging
    """
