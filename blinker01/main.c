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

void main()
{
    while(1) {
        blink_gpio4();
    }
}