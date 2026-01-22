/**
 * @file viva_static.c
 * @brief Piano robot song data - Static linked list (no malloc)
 * @note Include this file OR the dynamic version, not both!
 */

#include "viva_data.h"

/* ============================================================
 * STATIC LINKED LIST - NO MALLOC REQUIRED
 * ============================================================ */

static ChordNode_t viva_nodes[VIVA_TOTAL_EVENTS];
static uint8_t viva_initialized = 0;

/**
 * @brief Get pointer to static linked list (initializes on first call)
 * @return Pointer to head of linked list
 */
ChordNode_t* viva_get_static_list(void) {
    if (!viva_initialized) {
        for (uint32_t i = 0; i < VIVA_TOTAL_EVENTS; i++) {
            viva_nodes[i].event = viva_data[i];
            viva_nodes[i].next = (i < VIVA_TOTAL_EVENTS - 1) ? &viva_nodes[i + 1] : NULL;
        }
        viva_initialized = 1;
    }
    return &viva_nodes[0];
}

/**
 * @brief Reset static list to start (for replay)
 */
void viva_reset_static_list(void) {
    viva_initialized = 0;
}
