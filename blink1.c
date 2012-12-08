/**
 * Very inefficient "blink LED" program.
 *
 * Shows that your board works and that the programs you've burned actually run
 * on the board by alternating between the red and green LEDs present on the
 * MSP-EXP430G2 board.
 **/
#include <msp430.h>

/* The MSP-EXP430G2 board has two LEDs, one green and one red. They are
 * attached to the MCU using GPIO lines on PORT 1. Each port has 8 bits. */

/**
 * The green LED is on port 1 pin 6
 **/
#define GREEN_LED (1<<6)

/**
 * The red LED is on port 1 pin 0.
 **/
#define RED_LED (1<<0)


static void disable_watchdog();


int main(void) {
    /**
     * Disable the watchdog so that we don't reboot all the time. This is
     * somewhat complicated so see below, after main() for an explanation.
     **/
    disable_watchdog();
#if defined(__MSP430_HAS_PORT1_R__)
    /**
     * Reset all bits in the P1OUT register, except the one that powers the
     * green LED.
     *
     * This register is used as a buffer to set the GPIO lines on the first
     * port (hence P1). Each value is held in the buffer (the P1OUT register)
     * and only actually used to drive the relevant pins on that port when
     * P1DIR register is configured (per pin).
     **/
    P1OUT = GREEN_LED;
    /**
     * Set the direction of the GPIO line on port 1 that is connected to the
     * green and red LEDs to OUT (writing 0 makes this an 'in' line that we can
     * read from). This will instantly copy the value of the corresponding bit
     * from the P1OUT register to the pins selected by this mask.
     **/
    P1DIR = GREEN_LED | RED_LED;
#endif
    for (;;) {
        /**
         * Do nothing useful for a moment, so that we can see the LED blinking.
         * This is totally inefficient as the MCU is basically in a busy loop
         * while it could have been sleeping. This is going to be improved in
         * subsequent examples.
         **/
        volatile unsigned int i;
        for (i = 0; i < 32000; ++i);
#if defined(__MSP430_HAS_PORT1_R__)
        /**
         * Toggle the value on the pin associated with the green and red LEDs.
         * Since we've started with GREEN_LED enabled this will alternate
         * between both LEDs. This will allow us to see the blinking as the
         * loop above implements a primitive version of sleep()
         **/
        P1OUT ^= GREEN_LED | RED_LED;
#endif
    }
}

/**
 * Disable the watchdog by enabling the HOLD mode.
 **/
static void disable_watchdog() {
    /*
     * Watchdog is a little piece of hardware that resets the MCU, unless
     * periodically told not to. This allows to recover from hangs or crashes
     * that would otherwise require human intervention (probably by pressing
     * the reset button) to resolve.
     *
     * The watchdog is actually configured for us, before main() starts but
     * this is something we'll investigate later.
     *
     * The watchdog is controlled by the 16-bit WDTCTL register. Since it is
     * such a significant piece of hardware, this register is implemented in a
     * way that minimizes the chance of failing (by not rebooting the MCU) in
     * case of a runaway program (stuck or randomly reading/writing to memory)
     *
     * The less significant 8 bits control the watchdog hardware. The more
     * significant 8 bits are used as kind of 'password'. Incorrectly written
     * password triggers PUC (power up clear) that resets the MCU. The value of
     * the password is the constant, WDTPW (watchdog password, 0x5A) and should
     * be bitwise combined with the actual operation that we wish to perform.
     *
     * This is intended to prevent runaway programs that write to various
     * addresses from re-setting or disabling the watchdog and thus rendering
     * the board stuck. Curiously the password always reads as 0x69 so a
     * memcpy() that tries to copy the WDTCTL register would reboot the board.
     *
     * The less significant 8 bits contain a number of flags that control how
     * the watchdog operates. This is too much to describe here so we'll just
     * use the WDTHOLD flag (bit 1) that allows us to simply disable the
     * hardware watchdog altogether.
     *
     * WDTHOLD - hold the watchdog timer, reset (to 0) on PUC. When set (to 1)
     *           disables the ongoing tick of the watchdog timer and saves
     *           some power.
     *
     * The remaining flags are: WDTNMIES WDTNMISEL WDTTMSEL WDTCNTCL WDTSSEL
     * WDTIS1 and WDTIS0. They will be used and explained later throughout the
     * series.
     **/
    WDTCTL = WDTPW | WDTHOLD;
}

