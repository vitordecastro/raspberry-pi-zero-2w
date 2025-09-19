typedef unsigned int   uint32_t;

#define PERI_BASE   0x3F000000UL
#define PM_BASE     (PERI_BASE + 0x100000)

#define PM_RSTC     ((volatile uint32_t*)(PM_BASE + 0x1C))
#define PM_WDOG     ((volatile uint32_t*)(PM_BASE + 0x24))

#define PM_PASSWORD 0x5A000000
#define PM_RSTC_WRCFG_FULL_RESET 0x00000020

// ~1 MHz clock /16 = 62_500 ticks ≈ 1 s
#define SECS_TO_TICKS(s)  ((s) * 62500)

#define PERIPHERAL_BASE 0x3F000000
#define GPIO_BASE (PERIPHERAL_BASE + 0x200000)

#define GPFSEL0  (*(volatile unsigned int *)(GPIO_BASE + 0x00))
#define GPSET0   (*(volatile unsigned int *)(GPIO_BASE + 0x1C))
#define GPCLR0   (*(volatile unsigned int *)(GPIO_BASE + 0x28))

void gpio4_high(void)
{
    GPFSEL0 = (GPFSEL0 & ~(7 << 12)) | (1 << 12); // 0b11111111111111111001111111111111 -> GPIO 4 as output (001)
    GPSET0 = 1 << 4;   // write a 1 to bit 4
}

void gpio4_low(void)
{
    // GPIO4 LOW
    GPCLR0 = 1 << 4;   // write a 1 to bit 4 to drive low
}

void blink_gpio4() {
    // Configure GPIO4 as output
    GPFSEL0 = (GPFSEL0 & ~(7 << 12)) | (1 << 12);

    // GPIO4 HIGH
    GPSET0 = 1 << 4;

    // Wait a bit (loop)
    for(volatile int i=0;i<1000000;i++);

    gpio4_low();

    // Wait a bit (loop)
    for(volatile int i=0;i<1000000;i++);
}

// Start the watchdog with a timeout in "ticks" (~1/16 s each).
void watchdog_start(unsigned int ticks)
{
    *PM_WDOG = PM_PASSWORD | ticks;  // set timeout
    *PM_RSTC = PM_PASSWORD | PM_RSTC_WRCFG_FULL_RESET; // enable full reset
}

// Feed (kick) the watchdog to prevent reset
void watchdog_kick(unsigned int ticks)
{
    *PM_WDOG = PM_PASSWORD | ticks;
}

// Disable the watchdog completely
void watchdog_stop(void)
{
    *PM_RSTC = PM_PASSWORD; // clear reset configuration
}

void main(void)
{
    // Configura watchdog para 10 s
    watchdog_start(4096*20);
    blink_gpio4();

    int ticks = 4095;
    while (1) {
        // Your main loop code
        for (volatile uint32_t i = 0; i < 1000000; i++);

        // Feed every ~1 second so it never expires
        if (ticks > 4090)
            watchdog_kick(ticks*20);
        ticks--;
    }
}
