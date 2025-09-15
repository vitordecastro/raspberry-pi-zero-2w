typedef unsigned int   uint32_t;

#define PERIPHERAL_BASE  0x3F000000
#define GPIO_BASE        (PERIPHERAL_BASE + 0x200000)
#define TIMER_BASE       (PERIPHERAL_BASE + 0x3000)

// GPIO registers
#define GPFSEL0 (*(volatile uint32_t *)(GPIO_BASE + 0x00))
#define GPSET0  (*(volatile uint32_t *)(GPIO_BASE + 0x1C))
#define GPCLR0  (*(volatile uint32_t *)(GPIO_BASE + 0x28))

// System Timer registers
#define SYST_CS  (*(volatile uint32_t *)(TIMER_BASE + 0x00))
#define SYST_CLO (*(volatile uint32_t *)(TIMER_BASE + 0x04))
#define SYST_CHI (*(volatile uint32_t *)(TIMER_BASE + 0x08))

// Delay function using System Timer
void delay_ms(uint32_t ms)
{
    uint32_t start = SYST_CLO;
    uint32_t wait_ticks = ms * 1000; // timer counts in microseconds
    while ((SYST_CLO - start) < wait_ticks) {
        // busy wait
    }
}

void main() {
    // Configure GPIO4 as output
    GPFSEL0 = (GPFSEL0 & ~(7 << 12)) | (1 << 12);

    while(1) {
        GPSET0 = 1 << 4;  // LED on
        delay_ms(500);    // 500 ms
        GPCLR0 = 1 << 4;  // LED off
        delay_ms(500);    // 500 ms
    }
}