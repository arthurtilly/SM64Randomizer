#ifndef _RANDOMIZER_H
#define _RANDOMIZER_H


#include "seq_ids.h"

enum AvoidanceSafety {
    AVOIDANCE_SAFETY_ALL,
    AVOIDANCE_SAFETY_MED,
    AVOIDANCE_SAFETY_HARD
};

struct AvoidancePoint {
    const uintptr_t *behavior;
    Vec3s pos;
    f32 radius;
    f32 height;
    u32 safety;
};

typedef struct AvoidancePoint AvoidancePointArray[];

struct AreaParams {
    f32 minX;
    f32 maxX;
    f32 minY;
    f32 maxY;
    f32 minZ;
    f32 maxZ;

    u32 areaParamFlags;

    f32 wallCheckRaycasterSearchDist;

    u32 numAvoidancePoints;
    AvoidancePointArray *avoidancePoints;
};

typedef struct AreaParams AreaParamsArray[];

enum FloorSafeLevels {
    FLOOR_SAFE_GROUNDED,
    FLOOR_SAFE_HOVERING
};

enum RandomizerSafety {
    SPAWN_SAFETY_SAFE,
    SPAWN_SAFETY_DEFAULT,
    SPAWN_SAFETY_HARD
};

enum StarDoorReqIDs {
    STAR_REQ_WF,
    STAR_REQ_PSS,
    STAR_REQ_JRB,
    STAR_REQ_CCM,
    STAR_REQ_BITDW,
    STAR_REQ_BASEMENT,
    STAR_REQ_BBH,
    STAR_REQ_TOTWC,
    STAR_REQ_HMC,
    STAR_REQ_DDD,
    STAR_REQ_SL,
    STAR_REQ_THI,
    STAR_REQ_TIPPY,
    STAR_REQ_UPSTAIRS,
    STAR_REQ_BITS,
    STAR_REQ_MAX
};

struct nodeInfo {
    u8 level;
    u8 area;
    u8 f0;
    u8 f1;
};

#define RAND_POSITION_FLAG_CAN_BE_UNDERWATER (1 << 0)
#define RAND_POSITION_FLAG_THI_A3_ABOVE_MESH (1 << 1)
#define RAND_POSITION_FLAG_SPAWN_TOP_OF_SLIDE (1 << 2)
#define RAND_POSITION_FLAG_SPAWN_BOTTOM_OF_SLIDE (1 << 3)
#define RAND_POSITION_FLAG_BBH_HMC_LIMITED_ROOMS (1 << 4)
#define RAND_POSITION_FLAG_SPAWN_FAR_FROM_WALLS (1 << 5)
#define RAND_POSITION_FLAG_SAFE (1 << 6)
#define RAND_POSITION_FLAG_MUST_BE_UNDERWATER (1 << 7)
#define RAND_POSITION_FLAG_HARD_HEIGHT (1 << 8)

#define AREA_PARAM_FLAG_CHANGING_WATER_LEVEL (1 << 0)

struct OptionsSettings {
    union {
        struct {
            u16 keepStructure:1;
            u16 randomLevelWarp:1;
            u16 adjustedExits:1;
            u16 onlyKeyObjects:1;
            u16 nonstopMode:2;
            u16 safeSpawns:2;
            u16 randomizeStarSpawns:1;
            u16 randomStarDoorCounts:2;
            u16 randomLevelSpawn:1;
            u16 starDoorRequirement:3;
            u16 demonOn:1;
        } s;
        u16 w;
    } gameplay;
    union {
        struct {
            u8 marioColors:2;
            u8 musicOn:2;
            u8 skyboxOn:1;
            u8 coinsOn:1;
        } s;
        u8 w;
    } cosmetic;
};

extern s32 curPreset;
extern struct OptionsSettings gOptionsSettings;
extern struct OptionsSettings gPresets[];

extern struct nodeInfo gLevelWarps[];
extern u16 gRandomSeed16;
extern u8 gRequiredStars[];
extern u16 gRandomizerGameSeed;
extern u16 gRandomizerTempSeed;
extern u8 gIsSetSeed;
extern u8 gIgnoreCollisionDistance;
extern u8 gWarpDestinations[];
extern AreaParamsArray *sLevelParams[];
extern u8 gRandomSongs[19];
extern u8 gSkyboxIndex;
extern u8 multicolorOn;
extern u8 gStarDoorReqLUT[];

extern u16 gUsedSeeds[100];
extern s32 gSpawnCounter;

extern void print_generic_text_ascii_buf(s16 x, s16 y, char *buf);
extern void print_generic_text_ascii(s16 x, s16 y, const char *src);
extern void get_safe_position(struct Object *, Vec3s, f32, f32, u16 *, u8, u32);
extern f32 get_val_in_range_uniform(f32, f32, u16 *);
extern void init_randomizer(s32 fileNum);
extern u8 get_nonrandom_level();
extern void print_seed_and_options_data(void);
extern void set_mario_rando_colors(void);

#endif
