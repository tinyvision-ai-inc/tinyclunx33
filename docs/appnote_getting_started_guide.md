# Getting started guide {#appnote_getting_started_guide}

This app note covers various steps involved in customizing the tinyCLUNX platform to your application as well as FAQ's.

Going through each step is not mandatory but only informative, and reaching tinyVision.ai on the
[chat server](https://discord.com/invite/3qbXujE) or by email is more than welcome.

## Private Repository access

Parts of our codebase are provided by invitation only via a private Github repository. Please request access by sending email to sales@tinyvision.ai. We also provide support via Discord by setting up a private channel for Enginee-Engineer discussions.

Please provide Github and Discord ID's of staff who will be working on the firmware and RTL.

You will receive access to the following repos:
1. Private USB driver repo
2. Private RTL repo with sample implementations

## Customizing your firmware

1. Get familiar with the Zephyr framework and provided RTL repositories.
2. The most common question we receive is whether we can support `xyz` sensor. The answer is a most probable yes. If it has a MIPI interface that is capable of a maximum of 1.2Gbps and up to 4 lanee, the FPGA can interface with it. I2C and/or SPI interfaces can be supported, we provide the I2C sample.
3. You will be writing a Zephyr driver for your application, you can use the sample `IMX219` there as a baseline for your sensor in the [`priv-tvai-usb`](https://github.com/tinyvision-ai-inc/priv-tvai-usb) private repo.
   It helps to have either a linux driver or the datasheets for reference.

## Customizing your RTL

1. Figure out the MIPI data rate per lane, data type (eg. `YUV422`, `RAW10` etc.) as well as whether the MIPI clock is going to use a continuous clock or a discontinuous clock.
2. Open up the Rx DPHY in the Radiant GUI and make changes to the `Number of Rx lanes`, `Rx Line rate` and `D-PHY clock mode`. The deskew calibration detection is recommended to be turned on. The GUI will give you the `Byte Clock Frequency`. Note this down, you will need it for the Byte2Pixel converter and PLL customization. Other settings can be modified ass needed but not necesary.
3. Customize the Byte2Pixel block by entering the `Number of Rx lanes`, `Byte Clock Frequency`. You should now select a suitable pixel clock frequency that exceeds the minimum required.
4. Customize the PLL by entering in the `Pixel clock` and `Byte Clock` frequencies. Make sure the error in the byte clock is as low as possible, ideally zero. Turn on the fractional PLL if required.
5. Build the FPGA and check for timing errors if any. Tweak RTL to add pipelines as well as Synthesis and PAR settings to eliminate timing errors.

**NOTE:** The design must be timing clean!
