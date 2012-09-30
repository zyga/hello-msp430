Hello MSP430
============

Version 0.1 :-)

This repository contains a (growing) number of simple programs for the TI
MSP430 micro controller. All of those programs are developed against the TI
MSP430 LaunchPad board (http://www.ti.com/ww/en/launchpad/msp430\_head.html)
and built with free, open source software toolchain.

Getting started
===============

You will need the following things to get started:

 * MSP430 LaunchPad board ($4.30, no typos here, free shipping!)

   Note: Currently you don't need to do anything else to your board (no need to
   solder the crystal yet) but make sure you don't loose or misplace anything
   that was in the box (the other MCU and the crystal)

 * Ubuntu 12.04 or later (I was using 12.10 beta) or other free
   operating system with the appropriate packages
 * GNU Make (ubuntu package: make)
 * GCC MSP430 toolchain (ubuntu package: gcc-msp430)
 * MSP430 debugger and flash utility (ubuntu package: mspdebug)

Setting up the software
=======================

To quickly install everything you will need run:

    $ sudo apt-get install make gcc-msp430 mspdebug

Get the source if you have not done so already:

    $ git clone git://github.com/zyga/hello-msp430.git 
    $ cd hello-msp430

Build everything:

    $ make -j

If you got this far then the software side worked. Now let's use the hardware!

Setting up hardware
===================

Get your MSP430 board ready, plug the mini-usb cable into the board, don't plug
it to your computer just yet. The USB port is connected to the 'emulation side'
of the board where a second MSP430 (M430F1612) and USB-to-Serial (TUSB4310)
allow us to talk to the serial port and re-program the main (replaceable) micro
controller.

The mspdebug package ships with /lib/udev/rules.d/92-mspdebug.rules that sets
the ownership of the relevant usb and tty devices to 'plugdev'. To  so make
sure you are a part of that group. Run 'groups' to see which groups you're a
member of. To quickly add yourself to the relevant group run this command:

    $ sudo usermod -a -G plugdev $(id -un)

Sadly you will need to log out to see gain the new group membership. Once you
log back in you should be ready to plug in your device

Once that is done have a look at syslog by running:

    $ tail -F /var/log/syslog

And plug the board to your computer. You should see a few messages that look
similar to those:


    Sep 29 22:10:27 fx kernel: [ 7567.976630] usb 2-4.1.5: new full-speed USB device number 8 using ehci_hcd
    Sep 29 22:10:27 fx kernel: [ 7568.091953] usb 2-4.1.5: New USB device found, idVendor=0451, idProduct=f432
    Sep 29 22:10:27 fx kernel: [ 7568.091957] usb 2-4.1.5: New USB device strings: Mfr=1, Product=2, SerialNumber=3
    Sep 29 22:10:27 fx kernel: [ 7568.091959] usb 2-4.1.5: Product: Texas Instruments MSP-FET430UIF
    Sep 29 22:10:27 fx kernel: [ 7568.091960] usb 2-4.1.5: Manufacturer: Texas Instruments
    Sep 29 22:10:27 fx kernel: [ 7568.091962] usb 2-4.1.5: SerialNumber: 83FF469286D92759
    Sep 29 22:10:27 fx kernel: [ 7568.097725] cdc_acm 2-4.1.5:1.0: This device cannot do calls on its own. It is not a modem.
    Sep 29 22:10:27 fx kernel: [ 7568.097728] cdc_acm 2-4.1.5:1.0: No union descriptor, testing for castrated device
    Sep 29 22:10:27 fx kernel: [ 7568.097740] cdc_acm 2-4.1.5:1.0: ttyACM0: USB ACM device
    Sep 29 22:10:27 fx mtp-probe: checking bus 2, device 8: "/sys/devices/pci0000:00/0000:00:1d.7/usb2/2-4/2-4.1/2-4.1.5"
    Sep 29 22:10:37 fx kernel: [ 7578.141186] hid-generic 0003:0451:F432.0007: usb_submit_urb(ctrl) failed: -1
    Sep 29 22:10:37 fx kernel: [ 7578.141252] hid-generic 0003:0451:F432.0007: timeout initializing reports
    Sep 29 22:10:37 fx kernel: [ 7578.141598] hid-generic 0003:0451:F432.0007: hiddev0,hidraw1: USB HID v1.01 Device [Texas Instruments Texas Instruments MSP-FET430UIF] on usb-0000:00:1d.7-4.1.5/input1
    Sep 29 22:10:37 fx mtp-probe: bus: 2, device: 8 was not an MTP device

You may get additional lines (a few seconds later) from modem-manager. This is
the annoying bit. It's harmless (unless you need to talk to your board via
serial before anything else does) and in practice it just blocks the serial
port for a few seconds. Later on we'll add a new udev rule to discourage
modem-manager to poke at our board. 

Flashing the first program
==========================

Ok, now you should be ready to re-flash your board. Run the following command now:

    $ mspdebug rf2500
    
If everything else is okay and all the permissions are set correctly you will
likely see something like this: 

    MSPDebug version 0.19 - debugging tool for MSP430 MCUs
    Copyright (C) 2009-2012 Daniel Beer <dlbeer@gmail.com>
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    Trying to open interface 1 on 008
    rf2500: warning: can't detach kernel driver: No data available
    Initializing FET...
    FET protocol version is 30394216
    Configured for Spy-Bi-Wire
    Sending reset...
    Set Vcc: 3000 mV
    Device ID: 0x2553
    Device: MSP430G2553
    Code memory starts at 0xc000
    Number of breakpoints: 2

    Available commands:
        =         delbreak  gdb       load      opt       reset     simio     
        alias     dis       help      locka     prog      run       step      
        break     erase     hexout    md        read      set       sym       
        cgraph    exit      isearch   mw        regs      setbreak  

    Available options:
        color           gdb_loop        iradix          
        fet_block_size  gdbc_xfer_size  quiet           

    Type "help <topic>" for more information.
    Press Ctrl+D to quit.

    (mspdebug) 

To write the 'empty.elf' program now run the following command in mspdebug shell.
You obviously need to build empty.elf if you have not done so before:

    (mspdebug) prog empty.elf

The output, on success, looks like this:

    Erasing...
    Programming...
    Writing   80 bytes to c000 [section: .text]...
    Writing   32 bytes to ffe0 [section: .vectors]...
    Done, 112 bytes written

Then run the 'exit' command detach the debugger and let the program start:

    (mspdebug) exit

If something fails just disconnect the board, quit mspdebug and try again.

That's it, your board is now happily doing nothing, in a busy, non-low-power
mode!

Flashing the second program
===========================

Since manually flashing with mspdebug is kind of tedious you can quickly flash
a single elf file by adding the special 'flash' target when invoking make:

    $ make flash empty.elf

This will properly build (if needed) and flash the program.
