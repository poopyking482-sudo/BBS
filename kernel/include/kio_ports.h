#ifndef KIO_PORTS_H
#define KIO_PORTS_H

// output byte to port
static inline void outb(unsigned short port, unsigned char val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// input byte from port
static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// output word (16-bit) to port
static inline void outw(unsigned short port, unsigned short val) {
    __asm__ __volatile__ ("outw %0, %1" : : "a"(val), "Nd"(port));
}

// input word (16-bit) from port
static inline unsigned short inw(unsigned short port) {
    unsigned short ret;
    __asm__ __volatile__ ("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// port I/O wait (delay for slow hardware buses)
static inline void io_wait(void) {
    outb(0x80, 0); // Write dummy byte to unused port 0x80
}

#endif // KIO_PORTS_H
