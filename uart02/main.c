#define PERIPHERAL_BASE  0x3F000000
#define GPIO_BASE        (PERIPHERAL_BASE + 0x200000)
#define GPFSEL1     ((volatile unsigned int*)(GPIO_BASE + 0x0004))
#define GPPUD   ((volatile unsigned int*)(GPIO_BASE + 0x0094))

#define UART0_BASE        (PERIPHERAL_BASE + 0x201000)
#define UART_DR    ((volatile unsigned int*)(UART0_BASE + 0x00))
#define UART_FR    ((volatile unsigned int*)(UART0_BASE + 0x18))
#define UART_IBRD  ((volatile unsigned int*)(UART0_BASE + 0x24))
#define UART_FBRD  ((volatile unsigned int*)(UART0_BASE + 0x28))
#define UART_LCRH  ((volatile unsigned int*)(UART0_BASE + 0x2C))
#define UART_CR    ((volatile unsigned int*)(UART0_BASE + 0x30))
#define UART_IMSC  ((volatile unsigned int*)(UART0_BASE + 0x38))
#define UART_ICR   ((volatile unsigned int*)(UART0_BASE + 0x44))

#define RXFE        (1 << 4)   // Receive FIFO empty

void uart_init(void) {
    *UART_CR = 0;           // disable UART
    *UART_ICR = 0x7FF;      // clear interrupts
    *UART_IBRD = 26;        // integer baud
    *UART_FBRD = 3;         // fractional baud
    *UART_LCRH = (1 << 4) | (3 << 5); // FIFO enable + 8-bit
    *UART_CR = (1 << 9) | (1 << 8) | (1 << 0); // enable TX, RX, UART
}

void gpio_init_uart(void) {
    // 1. Set GPIO14 and GPIO15 to ALT0 (function “UART0_TXD/RXD”)
    unsigned int val = *GPFSEL1;
    val &= ~((7 << 12) | (7 << 15)); // clear bits for GPIO14,15
    val |=  (4 << 12) | (4 << 15);   // ALT0 = 0b100
    *GPFSEL1 = val;

    // 2. Disable internal pull-up/down resistors on these pins
    val = *GPPUD;
    val &= ~((3 << (2 * 14)) | (3 << (2 * 15))); // set both to 00 (no pulls)
    *GPPUD = val;
}

void uart_putc(char c) {
    while (*UART_FR & (1 << 5)) { } // wait until TXFF == 0
    *UART_DR = c;
}

void uart_puts(const char* s) {
    while (*s) {
        if (*s == '\n') uart_putc('\r');
        uart_putc(*s++);
    }
}

char uart0_getc(void) {
    // flush any old data first (optional)
    // while (!(*UART_FR & RXFE)) (void)UART_DR;

    // Wait until data is present
    while (*UART_FR & RXFE) {
        // spin
    }
    // Read one byte
    return (char)(*UART_DR & 0xFF);
}

void main(void) {
    gpio_init_uart();
    uart_init();
    uart_puts("Hello from bare metal!\n");
    while (1) {
        unsigned char get = uart0_getc();
        char buf[2];                 // 1 char + null terminator
        buf[0] = (char)get;
        buf[1] = '\0';
        const char* str = buf;
        uart_puts(str);
        uart_puts(str);
        uart_puts("\nSended\n");
    }
}