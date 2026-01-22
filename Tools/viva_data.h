/**
 * @file viva_data.h
 * @brief Piano robot song data - Auto-generated from MIDI
 * 
 * Total chord events: 727
 * Unique notes: 24
 */

#ifndef VIVA_DATA_H
#define VIVA_DATA_H

#include <stdint.h>

/* ============================================================
 * POSITION MACROS - DEFINE THESE BASED ON YOUR HARDWARE!
 * Each macro should be the physical position/step count for
 * the robot actuator to press that piano key.
 * ============================================================ */

#ifndef CS3_POS
#define CS3_POS      (13U)  /* C#3 - MIDI 49 */
#endif

#ifndef DS3_POS
#define DS3_POS      (15U)  /* D#3 - MIDI 51 */
#endif

#ifndef F3_POS
#define F3_POS       (17U)  /* F3 - MIDI 53 */
#endif

#ifndef GS3_POS
#define GS3_POS      (20U)  /* G#3 - MIDI 56 */
#endif

#ifndef AS3_POS
#define AS3_POS      (22U)  /* A#3 - MIDI 58 */
#endif

#ifndef C4_POS
#define C4_POS       (24U)  /* C4 - MIDI 60 */
#endif

#ifndef CS4_POS
#define CS4_POS      (25U)  /* C#4 - MIDI 61 */
#endif

#ifndef DS4_POS
#define DS4_POS      (27U)  /* D#4 - MIDI 63 */
#endif

#ifndef F4_POS
#define F4_POS       (29U)  /* F4 - MIDI 65 */
#endif

#ifndef G4_POS
#define G4_POS       (31U)  /* G4 - MIDI 67 */
#endif

#ifndef GS4_POS
#define GS4_POS      (32U)  /* G#4 - MIDI 68 */
#endif

#ifndef AS4_POS
#define AS4_POS      (34U)  /* A#4 - MIDI 70 */
#endif

#ifndef C5_POS
#define C5_POS       (36U)  /* C5 - MIDI 72 */
#endif

#ifndef CS5_POS
#define CS5_POS      (37U)  /* C#5 - MIDI 73 */
#endif

#ifndef DS5_POS
#define DS5_POS      (39U)  /* D#5 - MIDI 75 */
#endif

#ifndef F5_POS
#define F5_POS       (41U)  /* F5 - MIDI 77 */
#endif

#ifndef G5_POS
#define G5_POS       (43U)  /* G5 - MIDI 79 */
#endif

#ifndef GS5_POS
#define GS5_POS      (44U)  /* G#5 - MIDI 80 */
#endif

#ifndef AS5_POS
#define AS5_POS      (46U)  /* A#5 - MIDI 82 */
#endif

#ifndef C6_POS
#define C6_POS       (48U)  /* C6 - MIDI 84 */
#endif

#ifndef CS6_POS
#define CS6_POS      (49U)  /* C#6 - MIDI 85 */
#endif

#ifndef DS6_POS
#define DS6_POS      (51U)  /* D#6 - MIDI 87 */
#endif

#ifndef F6_POS
#define F6_POS       (53U)  /* F6 - MIDI 89 */
#endif

#ifndef G6_POS
#define G6_POS       (55U)  /* G6 - MIDI 91 */
#endif


/* ============================================================
 * DATA STRUCTURES
 * ============================================================ */

#define MAX_CHORD_NOTES    10
#define VIVA_TOTAL_EVENTS  727

/**
 * @brief Represents a chord event (notes played simultaneously)
 */
typedef struct ChordEvent {
    uint8_t  num_notes;                   
    uint8_t  positions[MAX_CHORD_NOTES]; 
    uint16_t duration_ms;
    uint16_t delay_to_next_ms;
} ChordEvent_t;

/**
 * @brief Linked list node for chord sequence
 */
typedef struct ChordNode {
    ChordEvent_t event;
    struct ChordNode* next;
} ChordNode_t;

/* ============================================================
 * FUNCTION DECLARATIONS
 * ============================================================ */

extern const ChordEvent_t viva_data[VIVA_TOTAL_EVENTS];

ChordNode_t* viva_create_linked_list(void);
void viva_free_linked_list(ChordNode_t* head);
uint32_t viva_get_total_duration_ms(void);

#endif /* VIVA_DATA_H */