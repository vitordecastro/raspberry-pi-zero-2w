typedef unsigned int   uint32_t;

#define PERIPHERAL_BASE 0x3F000000
#define GPIO_BASE (PERIPHERAL_BASE + 0x200000)

#define GPFSEL0  (*(volatile unsigned int *)(GPIO_BASE + 0x00))
#define GPSET0   (*(volatile unsigned int *)(GPIO_BASE + 0x1C))
#define GPCLR0   (*(volatile unsigned int *)(GPIO_BASE + 0x28))

void gpio4_high(void)
{
    // GPFSEL0    = 0b11111111111111111111111111111111
    //            & 
    // ~(7 << 12) = 0b11111111111111111000111111111111
    //              ----------------------------------
    //              0b11111111111111111000111111111111
    //            |
    //  (1 << 12) = 0b00000000000000000001000000000000
    //              ----------------------------------
    //              0b11111111111111111001111111111111
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

#define IRQ_BASE          (PERIPHERAL_BASE + 0xB000)
#define TIMER_BASE        (PERIPHERAL_BASE + 0x3000)

#define IRQ_PENDING_1     (*(volatile uint32_t*)(IRQ_BASE + 0x204))
#define IRQ_ENABLE_1      (*(volatile uint32_t*)(IRQ_BASE + 0x210))

#define SYS_TIMER_CLO     (*(volatile uint32_t*)(TIMER_BASE + 0x04))
#define SYS_TIMER_C1      (*(volatile uint32_t*)(TIMER_BASE + 0x10))
#define SYS_TIMER_CS      (*(volatile uint32_t*)(TIMER_BASE + 0x00))

#define TIMER_IRQ_1       (1 << 1)

volatile uint32_t tick = 0;

void irq_c_handler(void) {
    if (IRQ_PENDING_1 & TIMER_IRQ_1) {
        tick++;                    // Incrementa contador
        // blink_gpio4(); it doesnt work here inside
        SYS_TIMER_CS = TIMER_IRQ_1; // Limpa interrupção
    }
}

void main(void) {
    // blink_gpio4();
    // Configura timer 1 para disparar daqui 1_000_000 ticks (~1 segundo se clock 1 MHz)
    SYS_TIMER_C1 = SYS_TIMER_CLO + 1000000;
    
    // Habilita IRQ do Timer 1
    IRQ_ENABLE_1 |= TIMER_IRQ_1;

    // Habilita interrupções globalmente
    asm volatile("msr daifclr, #2");

    while (1) {
        // Loop principal pode usar tick
        if (tick) {
            blink_gpio4();
            blink_gpio4();
            tick = 0;
            // Aqui você poderia piscar LED ou enviar algo via UART
        }
    }
}