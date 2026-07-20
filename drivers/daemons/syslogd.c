#include "syslogd.h"
#include "init/include/sys_io.h"

static const char *level_strings[] = {
    "[INFO]",
    "[WARN]",
    "[ERROR]",
    "[DEBUG]"
};

void syslogd_init(void) {
    io_print("[syslogd] Logging daemon initialized.\n");
}

void syslogd_log(uint8_t level, const char *origin, const char *msg) {
    if (level > LOG_DEBUG) level = LOG_INFO;

    // Output formatted log line: [LEVEL] [origin]: message
    io_print(level_strings[level]);
    io_print(" [");
    io_print(origin);
    io_print("]: ");
    io_print(msg);
    io_print("\n");
}
