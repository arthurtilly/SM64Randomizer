#ifndef _RANDOMIZER_H
#define _RANDOMIZER_H


#include "seq_ids.h"
#include "engine/math_util.h"

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
    FLOOR_SAFETY_LOW, // Most hovering objects
    FLOOR_SAFETY_MEDIUM, // ! boxes, nonstop stars - won't spawn over dangerous surfaces
    FLOOR_SAFETY_HIGH // Grounded objects and start warp - won't spawn over slippery or steep floors
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

#define AREA_PARAM_FLAG_CHANGING_WATER_LEVEL (1 << 0)

struct OptionsSettings {
    union {
        struct {
            u32 padding:14; // to make sure settings ids are small

            u32 keepStructure:1;
            u32 randomLevelWarp:1;
            u32 adjustedExits:2;
            u32 objectRandomization:1;
            u32 nonstopMode:2;
            u32 safeSpawns:2;
            u32 randomizeStarSpawns:1;
            u32 randomStarDoorCounts:2;
            u32 randomLevelSpawn:1;
            u32 starDoorRequirement:4;
            u32 demonOn:1;
        } s;
        u32 w;
    } gameplay;
    union {
        struct {
            u32 marioColors:2;
            u32 musicOn:2;
            u32 skyboxOn:1;
            u32 coinsOn:1;
            u32 starColors:2;
        } s;
        u32 w;
    } cosmetic;
};

enum StarColors {
    STAR_COLOR_OFF,
    STAR_COLOR_PER_STAR,
    STAR_COLOR_PER_LEVEL,
    STAR_COLOR_GLOBAL
};

extern s32 curPreset;
extern struct OptionsSettings gOptionsSettings;
extern struct OptionsSettings gPresets[];

extern struct nodeInfo gLevelWarps[];
extern u8 gRequiredStars[];
extern u32 gRandomizerGameSeed;
extern u8 gIsSetSeed;
extern u8 gIgnoreCollisionDistance;
extern u8 gWarpDestinations[];
extern AreaParamsArray *sLevelParams[];
extern u8 gRandomSongs[19];
extern u8 gSkyboxIndex;
extern u8 gStarDoorReqLUT[];
extern struct AvoidancePoint gDynamicAvoidancePoints[50];
extern u8 gNumDynamicAvoidancePoints;

extern void print_generic_text_ascii_buf(s16 x, s16 y, char *buf);
extern void print_generic_text_ascii(s16 x, s16 y, const char *src);
extern void get_safe_position(struct Object *, Vec3s, f32, f32, tinymt32_t *, u8, u32);
extern f32 get_val_in_range_uniform(f32, f32, tinymt32_t *);
extern void init_randomizer(s32 fileNum);
extern u8 get_nonrandom_level();
extern void print_seed_and_options_data(void);
extern void set_mario_rando_colors(void);
extern void init_star_color(struct Object *star, s32 courseID, s32 starID);

#endif
