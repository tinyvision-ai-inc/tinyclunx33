from pinout.core import Group, Image
from pinout.components.layout import Diagram_2Rows
from pinout.components.pinlabel import PinLabelGroup, PinLabel
from pinout.components.annotation import AnnotationLabel
from pinout.components.text import TextBlock
from pinout.components import leaderline as lline
from pinout.components.legend import Legend


# Import data for the diagram
import data

# Create a new diagram
# The Diagram_2Rows class provides 2 panels,
# 'panel_01' and 'panel_02', to insert components into.
diagram = Diagram_2Rows(1512, 1210, 1000, "diagram")

# Add a stylesheet
diagram.add_stylesheet("styles.css", embed=True)

# Create a group to hold the pinout-diagram components.
graphic = diagram.panel_01.add(Group(775, 42))

# Add and embed an image
#hardware = graphic.add(Image("devkit_front.png", x=-50, y=-70, embed=True))
hardware = graphic.add(Image("devkit_front.png", x=-350, embed=True))

# Measure and record key locations with the hardware Image instance
pin_pitch_v = 23
pin_start_y = 190

#hardware.add_coord("right", 307, pin_start_y)
hardware.add_coord("gpio0", 16, 100)
hardware.add_coord("right", 570, pin_start_y)
hardware.add_coord("right_heading", 570, -10)
hardware.add_coord("right_mid", 570, pin_start_y+160)
hardware.add_coord("right_low", 570, pin_start_y+318)
hardware.add_coord("right_jumper_j7", 570, 95)
hardware.add_coord("right_jumper_j10", 570, pin_start_y+487)

hardware.add_coord("left_heading", 30, -10)
hardware.add_coord("left_jumper_j13", 373, pin_start_y+480)
hardware.add_coord("left_jumper_j11", 489, 104)
hardware.add_coord("left_jumper_j12", 302, pin_start_y+567)
hardware.add_coord("left_jumper_j17", 279, 37)
hardware.add_coord("left_jumper_j18", 488, pin_start_y+115)
hardware.add_coord("left_jumper_j19", 488, pin_start_y+224)
hardware.add_coord("left_jumper_j21", 489, pin_start_y+440)
# Other (x,y) pairs can also be stored here
hardware.add_coord("pin_pitch_v", 0, pin_pitch_v)

# graphic.add(
#     PinLabel(
#         content="RESET",
#         x=hardware.coord("right_jumper_j7").x,
#         y=hardware.coord("right_jumper_j7").y,
#         tag="pwr",
#         body={"x": 117, "y": 30},
#         leaderline={"direction": "vh"},
#     )
# )

# Create pinlabels on the right header
graphic.add(
    PinLabelGroup(
        x=hardware.coord("right").x,
        y=hardware.coord("right").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(60, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(1, 1),
        labels=data.right_header,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("right_mid").x,
        y=hardware.coord("right_mid").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(60, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(1, 1),
        labels=data.right_header_mid,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("right_low").x,
        y=hardware.coord("right_low").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(60, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(1, 1),
        labels=data.right_header_low,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("right_heading").x,
        y=hardware.coord("right_heading").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(60, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(1, 1),
        labels=data.right_header_heading,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("right_jumper_j7").x,
        y=hardware.coord("right_jumper_j7").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(60, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(1, 1),
        labels=data.right_jumper_j7,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("right_jumper_j10").x,
        y=hardware.coord("right_jumper_j10").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(60, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(1, 1),
        labels=data.right_jumper_j10,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("left_heading").x,
        y=hardware.coord("left_heading").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(0, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(-1, 1),
        labels=data.left_header_heading,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("left_jumper_j13").x,
        y=hardware.coord("left_jumper_j13").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(347, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(-1, 1),
        labels=data.left_jumper_j13,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("left_jumper_j11").x,
        y=hardware.coord("left_jumper_j11").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(460, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(-1, 1),
        labels=data.left_jumper_j11,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("left_jumper_j12").x,
        y=hardware.coord("left_jumper_j12").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(275, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(-1, 1),
        labels=data.left_jumper_j12,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("left_jumper_j17").x,
        y=hardware.coord("left_jumper_j17").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(250, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(-1, 1),
        labels=data.left_jumper_j17,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("left_jumper_j18").x,
        y=hardware.coord("left_jumper_j18").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(300, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(-1, 1),
        labels=data.left_jumper_j18,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("left_jumper_j19").x,
        y=hardware.coord("left_jumper_j19").y,
        pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        label_start=(300, 0),
        label_pitch=(0, pin_pitch_v),
        scale=(-1, 1),
        labels=data.left_jumper_j19,
    )
)

graphic.add(
    PinLabelGroup(
        x=hardware.coord("left_jumper_j21").x,
        y=hardware.coord("left_jumper_j21").y,
        #pin_pitch=hardware.coord("pin_pitch_v", raw=True),
        pin_pitch=(-22, 0),
        label_start=(145, 200),
        label_pitch=(0, 25),
        scale=(1, 1),
        labels=data.left_jumper_j21,
        leaderline=lline.Curved(direction="vh"),
    )
)

# Create a title and description text-blocks
title_block = diagram.panel_02.add(
    TextBlock(
        data.title,
        x=20,
        y=30,
        line_height=20,
        tag="panel title_block",
    )
)

diagram.panel_02.add(
    TextBlock(
        data.description,
        x=20,
        y=60,
        width=title_block.width,
        height=diagram.panel_02.height - title_block.height,
        line_height=20,
        tag="panel text_block",
    )
)

# Create a legend
legend = diagram.panel_02.add(
    Legend(
        data.legend,
        x=400,
        y=20,
        max_height=132,
    )
)

# Export the diagram via commandline:
# >>> py -m pinout.manager --export pinout_diagram.py diagram.svg
