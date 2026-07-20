#include "include/sys_ipc.h"
#include "init/sys_io.h"

// Simple internal message queue slot
typedef struct {
    message_t msg;
    uint8_t is_occupied;
} ipc_slot_t;

#define MAX_IPC_QUEUE 16
static ipc_slot_t g_ipc_queue[MAX_IPC_QUEUE];

int kipc_send(uint32_t target_pid, message_t *msg) {
    if (!msg) return -1;

    // Find an empty slot in the queue
    for (int i = 0; i < MAX_IPC_QUEUE; i++) {
        if (!g_ipc_queue[i].is_occupied) {
            g_ipc_queue[i].msg = *msg;
            g_ipc_queue[i].msg.target_pid = target_pid;
            g_ipc_queue[i].is_occupied = 1;

            io_print("[k] IPC Message enqueued.\n");
            return 0; // Success
        }
    }

    io_print("[k] IPC Queue Full!\n");
    return -1; // Queue full
}

int kipc_recv(uint32_t sender_pid, message_t *msg) {
    if (!msg) return -1;

    // Scan queue for messages intended for the current target or matching sender
    for (int i = 0; i < MAX_IPC_QUEUE; i++) {
        if (g_ipc_queue[i].is_occupied) {
            if (sender_pid == IPC_ANY_SENDER || g_ipc_queue[i].msg.sender_pid == sender_pid) {
                *msg = g_ipc_queue[i].msg;
                g_ipc_queue[i].is_occupied = 0; // Clear slot
                return 0; // Success
            }
        }
    }

    return -1; // No message available yet
}
