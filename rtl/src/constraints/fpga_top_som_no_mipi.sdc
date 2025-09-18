create_clock -name {clk_2} -period 41.6666666666667 [get_ports clk_2]
create_clock -name {usb_clk} -period 16.6666666666667 [get_nets usb_clk]
create_clock -name {hf_clk} -period 22.2222222222222 [get_nets hf_clk]
create_clock -name {lf_clk} -period 1000 [get_nets lf_clk]
create_clock -name {pixel_clk} -period 10 [get_nets pixel_clk]
#create_clock -name {tx_clk} -period 5 [get_nets {tx_clk tx_clk_90}]
create_clock -name {wb_clk} -period 12 [get_nets wb_clk]
#set_clock_groups -group [get_clocks byte_clk] -group [get_clocks clk_2] -group [get_clocks clk_60m] -group [get_clocks hf_clk] -group [get_clocks lf_clk] -group [get_clocks pixel_clk] -group [get_clocks sync_clk] -group [get_clocks tx_clk] -group [get_clocks usb_clk] -asynchronous
set_clock_groups -group [get_clocks clk_2] -group [get_clocks usb_clk] -group [get_clocks hf_clk] -group [get_clocks lf_clk] -group [get_clocks wb_clk] -group [get_clocks pixel_clk] -asynchronous
set_clock_uncertainty 0.5 [all_clocks]
