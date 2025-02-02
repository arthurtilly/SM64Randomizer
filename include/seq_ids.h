#ifndef SEQ_IDS_H
#define SEQ_IDS_H

// Sometimes a sequence id is represented as one of the below ids (the base id),
// optionally OR'd with SEQ_VARIATION.
#define SEQ_BASE_ID 0x7f
#define SEQ_VARIATION 0x80
#define SEQUENCE_NONE 0xFF

#define SEQ_MENU_GAME_OVER (SEQ_MENU_TITLE_SCREEN | SEQ_VARIATION)

enum SeqId {
    SEQ_SOUND_PLAYER,                 // 0x00
    SEQ_EVENT_CUTSCENE_COLLECT_STAR,  // 0x01
    SEQ_MENU_STAR_SELECT,             // 0x02
    SEQ_EVENT_KOOPA_MESSAGE,          // 0x03
    SEQ_EVENT_CUTSCENE_CREDITS,       // 0x04
    SEQ_EVENT_SOLVE_PUZZLE,           // 0x05
    SEQ_EVENT_TOAD_MESSAGE,           // 0x06
    SEQ_EVENT_CUTSCENE_VICTORY,       // 0x07
    SEQ_EVENT_CUTSCENE_ENDING,        // 0x08
    SEQ_EVENT_CUTSCENE_COLLECT_KEY,   // 0x09
    SEQ_EVENT_CUTSCENE_STAR_SPAWN,    // 0x0A
    SEQ_EVENT_HIGH_SCORE,             // 0x0B
    SEQ_EVENT_ENDLESS_STAIRS,         // 0x0C
    SEQ_EVENT_MERRY_GO_ROUND,         // 0x0D
    SEQ_MENU_TITLE_SCREEN,            // 0x0E
    SEQ_LEVEL_GRASS,                  // 0x0F
    SEQ_LEVEL_INSIDE_CASTLE,          // 0x10
    SEQ_LEVEL_WATER,                  // 0x11
    SEQ_LEVEL_HOT,                    // 0x12
    SEQ_LEVEL_BOSS_KOOPA,             // 0x13
    SEQ_LEVEL_SNOW,                   // 0x14
    SEQ_LEVEL_SLIDE,                  // 0x15
    SEQ_LEVEL_SPOOKY,                 // 0x16
    SEQ_EVENT_PIRANHA_PLANT,          // 0x17
    SEQ_LEVEL_UNDERGROUND,            // 0x18
    SEQ_EVENT_POWERUP,                // 0x19
    SEQ_EVENT_METAL_CAP,              // 0x1A
    SEQ_LEVEL_KOOPA_ROAD,             // 0x1B
    SEQ_EVENT_RACE,                   // 0x1C
    SEQ_EVENT_BOSS,                   // 0x1D
    SEQ_LEVEL_BOSS_KOOPA_FINAL,       // 0x1E
    SEQ_MENU_FILE_SELECT,             // 0x1F
    SEQ_LEVEL_20,
    SEQ_LEVEL_21,
    SEQ_LEVEL_22,
    SEQ_LEVEL_23,
    SEQ_LEVEL_24,
    SEQ_LEVEL_25,
    SEQ_LEVEL_26,
    SEQ_LEVEL_27,
    SEQ_LEVEL_28,
    SEQ_LEVEL_29,
    SEQ_LEVEL_2A,
    SEQ_LEVEL_2B,
    SEQ_LEVEL_2C,
    SEQ_LEVEL_2D,
    SEQ_LEVEL_2E,
    SEQ_LEVEL_2F,
    SEQ_LEVEL_30,
    SEQ_LEVEL_31,
    SEQ_LEVEL_32,
    SEQ_LEVEL_33,
    SEQ_LEVEL_34,
    SEQ_LEVEL_35,
    SEQ_LEVEL_36,
    SEQ_LEVEL_37,
    SEQ_LEVEL_38,
    SEQ_LEVEL_39,
    SEQ_LEVEL_3A,
    SEQ_LEVEL_3B,
    SEQ_LEVEL_3C,
    SEQ_LEVEL_3D,
    SEQ_LEVEL_3E,
    SEQ_LEVEL_3F,
    SEQ_LEVEL_40,
    SEQ_LEVEL_41,
    SEQ_LEVEL_42,
    SEQ_LEVEL_43,
    SEQ_LEVEL_44,
    SEQ_LEVEL_45,
    SEQ_LEVEL_46,
    SEQ_LEVEL_47,
    SEQ_LEVEL_48,
    SEQ_LEVEL_49,
    SEQ_LEVEL_4A,
    SEQ_LEVEL_4B,
    SEQ_LEVEL_4C,
    SEQ_LEVEL_4D,
    SEQ_LEVEL_4E,
    SEQ_LEVEL_4F,
    SEQ_LEVEL_50,
    SEQ_LEVEL_51,
    SEQ_LEVEL_52,
    SEQ_LEVEL_53,
    SEQ_LEVEL_54,
    SEQ_LEVEL_55,
    SEQ_LEVEL_56,
    SEQ_LEVEL_57,
    SEQ_LEVEL_58,
    SEQ_LEVEL_59,
    SEQ_LEVEL_5A,
    SEQ_LEVEL_5B,
    SEQ_COUNT
};

#endif // SEQ_IDS_H
