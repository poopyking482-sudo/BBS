#include "net_d.h"
#include "kernel/include/kio_ports.h"
#include "syslogd.h"

#define INTEL_VEND 0x8086
#define E1000_DEV  0x100E

#define REG_CTRL 0x0000
#define REG_RCTL 0x0100
#define REG_TCTL 0x0400

#define mmio_write32(addr, val) (*(volatile uint32_t *)(addr) = (val))
#define mmio_read32(addr)       (*(volatile uint32_t *)(addr))

static uint8_t *e1000_mmio_base = 0;

static inline void e1000_write(uint32_t reg, uint32_t val) {
    mmio_write32(e1000_mmio_base + reg, val);
}

static inline uint32_t e1000_read(uint32_t reg) {
    return mmio_read32(e1000_mmio_base + reg);
}

void net_init(uint16_t io_base) {
    (void)io_base;
    e1000_mmio_base = (uint8_t *)pci_get_bar0(INTEL_VEND, E1000_DEV);

    uint32_t ctrl = e1000_read(REG_CTRL);
    e1000_write(REG_CTRL, ctrl | (1 << 5));

    uint32_t rctl = e1000_read(REG_RCTL);
    e1000_write(REG_RCTL, rctl | (1 << 1) | (1 << 15) | (1 << 26));

    syslogd_log(LOG_INFO, "net_d", "net Daemon launched");
}

void net_send_packet(uint8_t *data, uint32_t len) {
    syslogd_log(LOG_DEBUG, "net_d", "packet sent");
}

void net_handle_receive(void) {
    syslogd_log(LOG_INFO, "net_d", "packet received");
}
