#include <ultra64.h>
#include <string.h>

#include "sm64.h"
#include "engine/behavior_script.h"
#include "behavior_data.h"
#include "randomizer.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "memory.h"
#include "area.h"
#include "camera.h"
#include "mario.h"
#include "object_list_processor.h"
#include "engine/math_util.h"
#include "text_strings.h"
#include "ingame_menu.h"
#include "print.h"
#include "menu/file_select.h"
#include "save_file.h"
#include "buffers/buffers.h"
#include "segment2.h"

u32 gRandomizerGameSeed;

u8 gIsSetSeed = FALSE;

u8 gIgnoreCollisionDistance = FALSE; // hacky

u8 gNumDynamicAvoidancePoints = 0;
struct AvoidancePoint gDynamicAvoidancePoints[50];

struct OptionsSettings gOptionsSettings;

#include "randomizer_data.h"

u8 gRandomSongs[19] = { SEQ_MENU_TITLE_SCREEN, SEQ_LEVEL_GRASS,          SEQ_LEVEL_INSIDE_CASTLE,    SEQ_LEVEL_WATER,
                      SEQ_LEVEL_HOT,         SEQ_LEVEL_BOSS_KOOPA,     SEQ_LEVEL_SNOW,             SEQ_LEVEL_SLIDE,
                      SEQ_LEVEL_SPOOKY,      SEQ_EVENT_PIRANHA_PLANT,  SEQ_LEVEL_UNDERGROUND,      SEQ_EVENT_POWERUP, 
                      SEQ_EVENT_METAL_CAP,   SEQ_LEVEL_KOOPA_ROAD,     SEQ_EVENT_MERRY_GO_ROUND,   SEQ_EVENT_BOSS,
                      SEQ_EVENT_ENDLESS_STAIRS, SEQ_LEVEL_BOSS_KOOPA_FINAL, SEQ_MENU_FILE_SELECT, 
};

struct nodeInfo gLevelWarps[] = {
    // Level        Area  0xF0  0xF1
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { LEVEL_CASTLE_COURTYARD, 0x01, 0x0A, 0x0B, }, // BBH
    { LEVEL_CASTLE, 0x01, 0x33, 0x65, }, // CCM
    { LEVEL_CASTLE_GROUNDS, 1, 0,  3, }, // Inside Castle
    { LEVEL_CASTLE, 0x03, 0x34, 0x66, }, // HMC
    { LEVEL_CASTLE, 0x03, 0x33, 0x65, }, // SSL
    { LEVEL_CASTLE, 0x01, 0x32, 0x64, }, // BoB
    { LEVEL_CASTLE, 0x02, 0x36, 0x68, }, // SL
    { LEVEL_CASTLE, 0x02, 0x32, 0x64, }, // WDW
    { LEVEL_CASTLE, 0x01, 0x35, 0x67, }, // JRB
    { LEVEL_CASTLE, 0x02, 0x37, 0x69, }, // THI
    { LEVEL_CASTLE, 0x02, 0x35, 0x67, }, // TTC
    { LEVEL_CASTLE, 0x02, 0x3A, 0x6C, }, // RR
    { LEVEL_CASTLE_GROUNDS, 1, 0,  3, }, // Castle Grounds
    { LEVEL_CASTLE, 0x01, 0x24, 0x25, }, // BitDW
    { LEVEL_CASTLE_GROUNDS , 0x01, 0x08, 0x06, }, // VCutM
    { LEVEL_CASTLE, 0x03, 0x36, 0x68, }, // BitFS
    { LEVEL_CASTLE, 0x01, 0x27, 0x28, }, // SA
    { LEVEL_CASTLE, 0x02, 0x00, 0x6B, }, // BitS
    { LEVEL_CASTLE, 0x03, 0x32, 0x64, }, // LLL
    { LEVEL_CASTLE, 0x03, 0x35, 0x67, }, // DDD
    { LEVEL_CASTLE, 0x01, 0x34, 0x66, }, // WF
    { 0, 0, 0, 0, }, // Cake
    { LEVEL_CASTLE_GROUNDS, 1, 0,  3, }, // Castle Courtyard
    { LEVEL_CASTLE, 0x01, 0x26, 0x23, }, // PSS
    { LEVEL_HMC,    0x01, 0x0C, 0x0D, }, // CotMC
    { LEVEL_CASTLE, 0x01, 0x26, 0x23, }, // TotWC
    { 0, 0, 0, 0, },
    { LEVEL_CASTLE, 0x02, 0x38, 0x6D, }, // WMotR
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { 0, 0, 0, 0, },
    { LEVEL_CASTLE, 0x02, 0x34, 0x66, }, // TTM
};

void convert_from_ascii(char *buf) {
    int i = 0;
    char c;
    while (buf[i] != 0) {
        c = buf[i];
        buf[i] = (((c) >= '0' && (c) <= '9') ? ((c) - '0') :
            ((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 0x0A) :
            ((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 0x24) : 
            ((c) == ' ') ? 0x9E : ((c) == '\n' ? 0xFE : ((c) == ',' ? 0x6F : c)));
        i++;
    }
    buf[i] = 0xFF;
}

// Print ascii string as a dialog string by converting it to charmap
// (only supports letters numbers and spaces)
void print_generic_text_ascii_buf(s16 x, s16 y, char *buf) {
    convert_from_ascii(buf);
    print_generic_string(x, y, (u8 *)buf);
}

// Print const string directly by strcpying into buffer (max length 30)
void print_generic_text_ascii(s16 x, s16 y, const char *src) {
    char buf[250];
    char *dest = buf;
    while (*src != 0) {
        *dest++ = *src++;
    }
    *dest=0;
    print_generic_text_ascii_buf(x, y, buf);
}

char *presetStrings[] = {
    "Default",
    "", // default plus
    "Extreme",
    "Easy",
    "New Game",
    "Calm",
    "Sprint"
};

s32 curPreset = 0;

struct OptionsSettings gPresets[] = {
    {{{0, /* pad */ 0, 1, 0, 1, 0, 1, 1, 1, 1,  7, 0}}, {{0, 0, 0, 0, 0}}},
    {{{0, /* pad */ 0, 1, 0, 1, 0, 1, 1, 1, 1,  7, 0}}, {{1, 1, 1, 1, 1}}},
    {{{0, /* pad */ 1, 1, 1, 1, 0, 2, 1, 1, 1, 10, 0}}, {{2, 1, 1, 1, 2}}},
    {{{0, /* pad */ 0, 1, 0, 1, 1, 0, 0, 2, 0,  5, 0}}, {{0, 0, 1, 0, 0}}},
    {{{0, /* pad */ 1, 1, 1, 1, 1, 1, 0, 1, 0,  8, 0}}, {{1, 1, 0, 1, 3}}},
    {{{0, /* pad */ 1, 0, 0, 0, 0, 1, 0, 0, 0,  7, 0}}, {{0, 0, 0, 0, 0}}},
    {{{0, /* pad */ 0, 1, 0, 1, 2, 1, 1, 2, 1,  3, 0}}, {{1, 0, 1, 0, 1}}},
};

unsigned char textVersion2[] = { TEXT_CURR_VERSION };

void print_seed_and_options_data(void) {
    char buf[20];
    s32 ypos = (gIsConsole ? 10 : 4);
    u32 i;
    u32 verXPos;
    gSPDisplayList(gDisplayListHead++, dl_ia_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gDialogTextAlpha);
    
    sprintf(buf, "%s Seed", (gIsSetSeed ? "Set" : "Random"));
    print_generic_text_ascii_buf(8, ypos + 28, buf);
    sprintf(buf, "Seed\xE6 %05d", gRandomizerGameSeed);
    print_generic_text_ascii_buf(8, ypos + 14, buf);
    
    for (i = 0; i < ARRAY_COUNT(gPresets); i++) {
        if (gOptionsSettings.gameplay.w == gPresets[i].gameplay.w) {
            sprintf(buf, "Preset\xE6 %s", presetStrings[i]);
            print_generic_text_ascii_buf(8,ypos,buf);
            goto presetFound; // don't kill me please
        }
    }
    
    sprintf(buf, "Settings ID\xE6 %d", gOptionsSettings.gameplay.w);
    print_generic_text_ascii_buf(8,ypos,buf);

presetFound:
    verXPos = 310 - 2*(310 - get_str_x_pos_from_center(310,textVersion2,0));
    print_generic_string(verXPos, ypos, textVersion2);

    gSPDisplayList(gDisplayListHead++, dl_ia_text_end);
}

static s32 find_floor_slipperiness(struct Surface *floor) {
    s32 floorClass = SURFACE_CLASS_DEFAULT;

    if (floor) {
        switch (floor->type) {
            case SURFACE_NOT_SLIPPERY:
            case SURFACE_HARD_NOT_SLIPPERY:
                floorClass = SURFACE_CLASS_NOT_SLIPPERY;
                break;

            case SURFACE_SLIPPERY:
            case SURFACE_NOISE_SLIPPERY:
            case SURFACE_HARD_SLIPPERY:
            case SURFACE_NO_CAM_COL_SLIPPERY:
                floorClass = SURFACE_CLASS_SLIPPERY;
                break;

            case SURFACE_VERY_SLIPPERY:
            case SURFACE_ICE:
            case SURFACE_HARD_VERY_SLIPPERY:
            case SURFACE_NOISE_VERY_SLIPPERY:
                floorClass = SURFACE_CLASS_VERY_SLIPPERY;
                break;
        }
    }

    return floorClass;
}

static u8 is_floor_safe(struct Surface *floor, u8 floorSafeLevel,
                        u32 randPosFlags) { // Checks if floor triangle can be spawned on
    s32 slipperiness;
    f32 norm;

    if (((floorSafeLevel == FLOOR_SAFETY_HIGH) || (gCurrLevelNum == LEVEL_DDD))
        && (floor->flags & SURFACE_FLAG_DYNAMIC))
        return FALSE; // grounded objects / DDD objects can't spawn on platforms

    switch(gOptionsSettings.gameplay.s.safeSpawns){
        case SPAWN_SAFETY_SAFE:
            norm = 0.85f;
            break;
        case SPAWN_SAFETY_HARD:
            norm = 0.3f;
            break;
        default:
            norm = 0.7f;
    }

    if ((floorSafeLevel == FLOOR_SAFETY_HIGH) || (randPosFlags & RAND_TYPE_SAFE)) {
        norm = 0.95f;
    }

    if (floor->normal.y <= norm) {
        return FALSE;
    }

    slipperiness = find_floor_slipperiness(floor);
    if ((randPosFlags & RAND_TYPE_SAFE) && (floorSafeLevel == FLOOR_SAFETY_HIGH)
        && ((slipperiness == SURFACE_CLASS_SLIPPERY) || (slipperiness == SURFACE_CLASS_VERY_SLIPPERY))) {

        // This code kills some spawns, assuming the most slippery case. This code would
        // probably be better to refactor based off slipperiness in general.
        if (floor->normal.y <= 0.99f) {
            return FALSE; // Don't spawn on slippery surfaces if you are a warp or safe object
        }
    }

    if (floor->type < SURFACE_SAFE_FLOORS_GENERAL) {
        return TRUE;
    }

    if ((gOptionsSettings.gameplay.s.safeSpawns == SPAWN_SAFETY_HARD) && (floorSafeLevel == FLOOR_SAFETY_LOW) && !(randPosFlags & RAND_TYPE_SAFE)) {
        if (floor->type < SURFACE_SAFE_FLOORS_HARD) {
            return TRUE;
        }
    }

    return FALSE;
}

// Checks if near a specific avoidance point
static u32 check_avoidance_point(Vec3s pos, struct Object *obj, struct AvoidancePoint *avoidancePoint) {
    void *behavior = segmented_to_virtual(avoidancePoint->behavior);
        
    if(((avoidancePoint->safety == AVOIDANCE_SAFETY_ALL) 
        || ((avoidancePoint->safety == AVOIDANCE_SAFETY_MED) && (gOptionsSettings.gameplay.s.safeSpawns == SPAWN_SAFETY_SAFE))
        || ((avoidancePoint->safety == AVOIDANCE_SAFETY_HARD) && (gOptionsSettings.gameplay.s.safeSpawns != SPAWN_SAFETY_HARD)))){
    } else {
        return FALSE;
    }

    if (behavior != segmented_to_virtual(bhvStub) && obj->behavior != behavior) {
        return FALSE;
    }

    if ((sqr(pos[0] - avoidancePoint->pos[0]) + sqr(pos[2] - avoidancePoint->pos[2]) < sqr(avoidancePoint->radius))
        && (pos[1] > avoidancePoint->pos[1]) && (pos[1] < avoidancePoint->pos[1] + avoidancePoint->height)) {
        return TRUE;
    }
    return FALSE;
}

// Checks if near any avoidance point
static u32 is_in_avoidance_point(Vec3s pos, struct AreaParams *areaParams, struct Object *obj) {
    struct AvoidancePoint *avoidancePoint;

    for (u32 i = 0; i < areaParams->numAvoidancePoints; i++) {
        avoidancePoint = &(*areaParams->avoidancePoints)[i];
        if (check_avoidance_point(pos, obj, avoidancePoint)) {
            return TRUE;
        }
    }
    for (u32 i = 0; i < gNumDynamicAvoidancePoints; i++) {
        if (check_avoidance_point(pos, obj, &gDynamicAvoidancePoints[i])) {
            return TRUE;
        }
    }

    return FALSE;
}

u32 raycast_wall_check(Vec3s pos) {
    s16 yaw = 0;
    struct Surface *surf;
    Vec3f checkPos;
    vec3_copy_y_off(checkPos, pos, 50.0f);
    Vec3f hitPos;

    for (u32 i = 0; i < 8; i++) {
        Vec3f rayDir;
        vec3f_set(rayDir, 0x10000 * sins(yaw), 0, 0x10000 * coss(yaw));

        find_surface_on_ray(checkPos, rayDir, &surf, hitPos,
            (RAYCAST_FIND_FLOOR | RAYCAST_FIND_WALL | RAYCAST_FIND_CEIL));

        if (surf != NULL) {
            f32 det = vec3f_dot(&surf->normal.x, rayDir);
            if (det > 0) {
                return FALSE;
            }
        }

        yaw += 0x2000;
    }
    return TRUE;
}

static void vec3s_resolve_wall_collisions(Vec3s pos, f32 radius) {
    Vec3f pos2;
    
    vec3s_to_vec3f(pos2, pos);
    f32_find_wall_collision(&pos2[0], &pos2[1], &pos2[2], 0.0f, radius);
    vec3f_to_vec3s(pos, pos2);
}

void create_dynamic_avoidance_point(Vec3f pos, f32 radius, f32 height, f32 downOffset) {
    struct AvoidancePoint *newPoint = &gDynamicAvoidancePoints[gNumDynamicAvoidancePoints];
    newPoint->pos[0] = pos[0];
    newPoint->pos[1] = pos[1] - downOffset;
    newPoint->pos[2] = pos[2];
    newPoint->radius = radius;
    newPoint->height = height;
    newPoint->safety = AVOIDANCE_SAFETY_ALL;
    newPoint->behavior = bhvStub;
    gNumDynamicAvoidancePoints++;
}

void get_safe_position(struct Object *obj, Vec3s pos, f32 minHeightRange, f32 maxHeightRange, tinymt32_t *randomState,
                       u8 floorSafeLevel, u32 randPosFlags) {
    struct AreaParams *areaParams = &(*sLevelParams[gCurrLevelNum - 4])[gCurrAreaIndex - 1];
    f32 minX, maxX, minY, maxY, minZ, maxZ, minHeight, maxHeight, waterLevel, lowFloorHeight, cHeight,
        highFloorHeight;
    u32 objCanBeUnderwater;
    struct Surface *lowFloor, *ceil, *highFloor;

    f32 wallRadius = 50.f;
    if (randPosFlags & RAND_TYPE_SPAWN_FAR_FROM_WALLS) {
        wallRadius = 300.f;
    }

    if (areaParams == NULL) {
        pos[0] = 0;
        pos[1] = 5000;
        pos[2] = 0;
        return;
    }

    minX = areaParams->minX;
    maxX = areaParams->maxX;
    minY = areaParams->minY;
    maxY = areaParams->maxY;
    minZ = areaParams->minZ;
    maxZ = areaParams->maxZ;

    // Handle special cases for bounds

    // THI wiggler cave
    if ((gCurrCourseNum == COURSE_THI) && (gCurrAreaIndex == 3)) {
        if (randPosFlags & RAND_TYPE_THI_A3_ABOVE_MESH)
            minY = 2200;
        else
            maxY = 1750;
    // PSS
    } else if ((gCurrCourseNum == COURSE_PSS)) {
        if (randPosFlags & RAND_TYPE_SPAWN_TOP_OF_SLIDE) {
            minY = 6100;
            minX = 3100;
        } else if (randPosFlags & RAND_TYPE_SPAWN_BOTTOM_OF_SLIDE) {
            maxY = -3500;
            minZ = 4000;
        } else
            minY = -1000;
    // CCM slide
    } else if ((gCurrCourseNum == COURSE_CCM) && (gCurrAreaIndex == 2)) {
        if (randPosFlags & RAND_TYPE_SPAWN_TOP_OF_SLIDE) {
            minY = 6600;
            maxX = -4800;
        } else if (randPosFlags & RAND_TYPE_SPAWN_BOTTOM_OF_SLIDE) {
            maxY = -3900;
            maxZ = -6400;
        }
    }

    if (gOptionsSettings.gameplay.s.nonstopMode == 1) {
        if ((obj->behavior == segmented_to_virtual(bhvStar))
         || (obj->behavior == segmented_to_virtual(bhvStarSpawnCoordinates))
         || (obj->behavior == segmented_to_virtual(bhvHiddenRedCoinStar))
         || (obj->behavior == segmented_to_virtual(bhvHiddenStar))) {
            floorSafeLevel = FLOOR_SAFETY_MEDIUM;
            randPosFlags |= RAND_TYPE_SAFE;
        }
    }

    while (TRUE) {
        u32 dangerShiftedOverHighFloor = FALSE;

        // Generate random position
        pos[0] = get_val_in_range_uniform(minX, maxX, randomState);
        pos[1] = get_val_in_range_uniform(minY, maxY, randomState);
        pos[2] = get_val_in_range_uniform(minZ, maxZ, randomState);

        lowFloorHeight = find_floor(pos[0], pos[1] + 20, pos[2], &lowFloor);

        if (lowFloor == NULL)
            continue;

        if ((pos[1] - lowFloorHeight) > (gCurrCourseNum == COURSE_BBH ? 350 : 800))
            continue;

        if (lowFloorHeight + 20 <= maxY) {
            pos[1] = lowFloorHeight + 20;
        }

        // Move out of any walls. This has to be done here because otherwise
        // there's the possibility of being pushed out of the wall into OoB or a ceiling
        vec3s_resolve_wall_collisions(
            pos, wallRadius);

        lowFloorHeight = find_floor(pos[0], pos[1], pos[2], &lowFloor);

        if ((pos[1] - lowFloorHeight) > (gCurrCourseNum == COURSE_BBH ? 350 : 800))
            continue;

        pos[1] = lowFloorHeight;

        if (lowFloor == NULL)
            continue;

        if (!is_floor_safe(lowFloor, floorSafeLevel, randPosFlags))
            continue;

        // Snap to ground and check if safe
        objCanBeUnderwater =
            (randPosFlags
                 & (RAND_TYPE_CAN_BE_UNDERWATER | RAND_TYPE_MUST_BE_UNDERWATER)
             || (areaParams->areaParamFlags & AREA_PARAM_FLAG_CHANGING_WATER_LEVEL));
        waterLevel = find_water_level(pos[0], pos[2]);
        minHeight = pos[1] + minHeightRange;
        maxHeight = pos[1] + maxHeightRange;

        // Let objects spawn anywhere in water
        if (floorSafeLevel != FLOOR_SAFETY_HIGH
            || (randPosFlags & RAND_TYPE_MUST_BE_UNDERWATER)) {
            if ((objCanBeUnderwater && (waterLevel > maxHeight)
                 && !(areaParams->areaParamFlags & AREA_PARAM_FLAG_CHANGING_WATER_LEVEL))
                || (randPosFlags & RAND_TYPE_MUST_BE_UNDERWATER))
                maxHeight = waterLevel;
        }

        // For the start warp, always spawn above the water
        if ((obj->behavior == segmented_to_virtual(bhvSpinAirborneWarp)) && (waterLevel > pos[1])) {
            minHeight = waterLevel + minHeightRange;
            maxHeight = waterLevel + maxHeightRange;
        }

        // Prevent objects from spawning too high above water in BBH
        if ((gCurrCourseNum == COURSE_BBH) && (pos[1] < waterLevel) && (maxHeight > waterLevel))
            maxHeight = waterLevel + 100.f;

        // Check if max height has gone above the level bounds
        if (maxHeight > maxY) {
            maxHeight = maxY;
        }

        // On Dangerous setting, some objects can spawn in midair in levels with wing cap
        if ((gOptionsSettings.gameplay.s.safeSpawns == SPAWN_SAFETY_HARD) && 
            ((gCurrCourseNum == COURSE_BOB) || ((gCurrCourseNum == COURSE_SSL) && (gCurrAreaIndex == 1))) && // Only in BoB and SSL
            (randPosFlags & RAND_TYPE_HARD_HEIGHT) && ((tinymt32_generate_u32(randomState) & 2) == 0)){ // 1/4 chance
            maxHeight = maxY;            
        }

        pos[1] = get_val_in_range_uniform(minHeight, maxHeight, randomState);

        if ((gOptionsSettings.gameplay.s.safeSpawns == SPAWN_SAFETY_HARD) &&
            (floorSafeLevel == FLOOR_SAFETY_LOW) && !(randPosFlags & RAND_TYPE_SAFE)) {
            Vec3f oldPos;
            vec3s_to_vec3f(oldPos, pos);
            pos[0] += get_val_in_range_uniform(-200, 200, randomState);
            pos[2] += get_val_in_range_uniform(-200, 200, randomState);

            vec3s_resolve_wall_collisions(
                pos, wallRadius);

            // Make sure it doesnt shift through a surface
            Vec3f rayDir;
            vec3s_to_vec3f(rayDir, pos);
            vec3f_sub(rayDir, oldPos);
            struct Surface *surf;
            Vec3f hitPos;
            find_surface_on_ray(oldPos, rayDir, &surf, hitPos,
            (RAYCAST_FIND_FLOOR | RAYCAST_FIND_WALL | RAYCAST_FIND_CEIL));

            if (surf != NULL) {
                continue;
            }
            
            waterLevel = find_water_level(pos[0], pos[2]);

            lowFloorHeight = find_floor(pos[0], pos[1], pos[2], &lowFloor);
            if (lowFloor == NULL) {
                continue;
            }
            if ((pos[1] - lowFloorHeight) > 800.f) {
                dangerShiftedOverHighFloor = TRUE;
            }
        }

        // Start checking if position is valid

        // Ceiling check
        cHeight = find_ceil(pos[0], lowFloorHeight + 80, pos[2], &ceil);

        if (pos[1] > cHeight - 100.f) // If in a ceiling, cancel spawn
            continue;

        if (dangerShiftedOverHighFloor & (pos[1] > cHeight - 200.f)) // If no ground nearby and too close to the ceiling
            continue;

        // Floor Check
        highFloorHeight = find_floor(pos[0], cHeight - 80, pos[2],
                                     &highFloor); // Find floor under object assuming 80 units of space

        if ((highFloorHeight > (pos[1] + 20))
            && ((highFloorHeight - pos[1])
                < 1500)) // If under floor and not large distance, deny height
            continue;

        if ((pos[1] - highFloorHeight) < (minHeightRange - 50.f)) {
            continue;
        }

        if (!objCanBeUnderwater && (waterLevel > pos[1]))
            continue;

        if ((randPosFlags & RAND_TYPE_MUST_BE_UNDERWATER) && (waterLevel < pos[1]))
            continue;

        if (randPosFlags & RAND_TYPE_LIMITED_BBH_HMC_SPAWNS) {
            if ((gCurrCourseNum == COURSE_BBH) && (lowFloor->room == 9)) {
                continue;
            } else if ((gCurrCourseNum == COURSE_HMC) && (lowFloor->room == 8)) {
                continue;
            }
        }

        if (is_in_avoidance_point(pos, areaParams, obj))
            continue;

        // Wall Check
        if (!raycast_wall_check(pos))
            continue;

        // Spawn avoidance point if needed
        if ((randPosFlags & RAND_TYPE_CREATE_AVOIDANCE_POINT) && (gNumDynamicAvoidancePoints < 50)) {
            Vec3f fpos;
            vec3_copy(fpos, pos);
            create_dynamic_avoidance_point(fpos, 100.f, 200.f, 50.f);
        }

        return;
    }
}

// Only uniform if used for floats. [min, max)
f32 get_val_in_range_uniform(f32 min, f32 max, tinymt32_t *randomState) {
    if (min > max)
        return min;
    return (tinymt32_generate_float(randomState) * (max - min)) + min;
}

u16 calulate_star_total(u32 level) {
    switch (gWarpDestinations[level]) {
        case LEVEL_PSS:
        return 2;

        case LEVEL_BITDW:
        case LEVEL_SA:
        case LEVEL_TOTWC:
        case LEVEL_COTMC:
        case LEVEL_BITFS:
        case LEVEL_VCUTM:
        case LEVEL_BITS: // Just to be thorough
        return 1;

        case LEVEL_HMC:
        return calulate_star_total(LEVEL_COTMC) + 7; // Is fine since gWarpDestinations[LEVEL_COTMC] =/= LEVEL_HMC
        break;

        case LEVEL_BOB:
        case LEVEL_SSL:
        return 6; // tryna watch out for wing cap
        case LEVEL_WMOTR:
        return 0; // same

        default:
        return 7;
    }
}

// Get the maximum requirement for a star door based on:
// BitS requirement
// If no keep structure, layer (0 for easily accessible doors, 1 for doors behind other doors)
// If keep structure, section (0 for lobby, 1 for basement, 2 for upstairs)
// Min factor (usually / 2 for important doors and / 3 for most )
// Maximum stars available at this point, if lower than regular max
u8 get_star_requirement(u8 layer, u8 section, u8 maxAvailable, u8 factor, tinymt32_t *randomState) {
    u8 bitsStars = gStarDoorReqLUT[gOptionsSettings.gameplay.s.starDoorRequirement];
    u8 maxStars = 0;
    u8 starReq;

    if (bitsStars == 0) bitsStars = 70;

    if (gOptionsSettings.gameplay.s.keepStructure) {
        switch (section) {
        case 0:
            maxStars = (u8)(bitsStars*0.2f); // lobby
            break;
        case 1:
            maxStars = (u8)(bitsStars*0.4f); // basement
            break;
        case 2:
            maxStars = (u8)(bitsStars*0.7f); // upstairs
        }
    } else {
        if (layer == 0) {
            maxStars = (u8)(bitsStars*0.35f); // layer 1
        } else {
            maxStars = (u8)(bitsStars*0.8f); // layer 2
        }
    }
    starReq = get_val_in_range_uniform(maxStars / factor, maxStars, randomState);
    return MIN(starReq, get_val_in_range_uniform(MAX(maxAvailable - 5, 0), maxAvailable, randomState));
}

void randomize_star_doors() {
    u16 starTotal = calulate_star_total(LEVEL_BOB);
    u8 i, tmp, index;
    u8 starLevels[15] = {LEVEL_WF, LEVEL_PSS, LEVEL_JRB /* + SA */, LEVEL_CCM, LEVEL_BITDW,
                        /* MIPS + moat */ 8, LEVEL_BBH, LEVEL_TOTWC, LEVEL_HMC, /*BitFS*/ LEVEL_DDD, LEVEL_SL, LEVEL_THI, /*Tippy*/ 50, /*Key 2 ( TTM + WDW) */ 30, 70};
    u8 firstFloor[7] = {STAR_REQ_WF, STAR_REQ_PSS, STAR_REQ_JRB, STAR_REQ_CCM, STAR_REQ_BITDW, STAR_REQ_BBH, STAR_REQ_TOTWC};
    tinymt32_t randomState;
    tinymt32_init(&randomState, gRandomizerGameSeed);
    
    for (i = 0; i <= 6; i++){
        index = get_val_in_range_uniform(i, 6, &randomState);
        tmp = firstFloor[i];
        firstFloor[i] = firstFloor[index];
        firstFloor[index] = tmp;

        gRequiredStars[firstFloor[i]] = get_star_requirement(0, 0, starTotal, 3, &randomState);
        starTotal += calulate_star_total(starLevels[firstFloor[i]]);
    }

    gRequiredStars[STAR_REQ_BASEMENT] = get_star_requirement(0, 1, starTotal, 2, &randomState);
    starTotal += calulate_star_total(LEVEL_SSL);
    starTotal += calulate_star_total(LEVEL_LLL);
    starTotal += calulate_star_total(LEVEL_VCUTM);

    gRequiredStars[STAR_REQ_HMC] = get_star_requirement(1, 1, starTotal, 3, &randomState);
    starTotal += calulate_star_total(LEVEL_HMC);

    gRequiredStars[STAR_REQ_DDD] = get_star_requirement(0, 1, starTotal, 3, &randomState);
    starTotal += calulate_star_total(LEVEL_DDD);

    gRequiredStars[STAR_REQ_UPSTAIRS] = get_star_requirement(0, 2, starTotal, 2, &randomState);
    starTotal += calulate_star_total(LEVEL_WDW);
    starTotal += calulate_star_total(LEVEL_TTM);

    gRequiredStars[STAR_REQ_SL] = get_star_requirement(1, 2, starTotal, 3, &randomState);
    gRequiredStars[STAR_REQ_THI] = get_star_requirement(1, 2, starTotal, 3, &randomState);
    starTotal += calulate_star_total(LEVEL_SL);
    starTotal += calulate_star_total(LEVEL_THI);

    gRequiredStars[STAR_REQ_TIPPY] = get_star_requirement(1, 2, starTotal, 2, &randomState);

    if (gOptionsSettings.gameplay.s.keepStructure) {
        gRequiredStars[STAR_REQ_BASEMENT] = 0;
        gRequiredStars[STAR_REQ_UPSTAIRS] = 0;
    }
}

static void init_required_stars() {
    u32 i;
    u8 defaultStar[STAR_REQ_MAX] = {
        1, // STAR_REQ_WF
        1, // STAR_REQ_PSS
        3, // STAR_REQ_JRB
        3, // STAR_REQ_CCM
        8, // STAR_REQ_BITDW
        8, // STAR_REQ_BASEMENT
        12, // STAR_REQ_BBH
        10, // STAR_REQ_TOTWC
        0, // STAR_REQ_HMC
        30, // STAR_REQ_DDD
        0, // STAR_REQ_SL
        0, // STAR_REQ_THI
        50, // STAR_REQ_TIPPY
        30, // STAR_REQ_UPSTAIRS
        70 // STAR_REQ_BITS
    }; 

    switch (gOptionsSettings.gameplay.s.randomStarDoorCounts) {
        case 1:
            randomize_star_doors();
            break;
        case 0:
            for (i = 0; i < sizeof(gRequiredStars); i++) {
                gRequiredStars[i] = defaultStar[i];
            }
            if (gOptionsSettings.gameplay.s.keepStructure) {
                gRequiredStars[STAR_REQ_BASEMENT] = 0;
                gRequiredStars[STAR_REQ_UPSTAIRS] = 0;
            }
            break;
        case 2:
            for (i = 0; i < sizeof(gRequiredStars); i++) {
                gRequiredStars[i] = 0;
            }
            break;
    }
    gRequiredStars[STAR_REQ_BITS] = gStarDoorReqLUT[gOptionsSettings.gameplay.s.starDoorRequirement]; // Final Bowser Door is special.
}

static void copy_remaining_warps() { // Initialises sRemainingWarpsTemp for use with the warp scrambler
    int i;
    for (i = 0; i < ARRAY_COUNT(sRemainingWarpsTemp); i++) {
        sRemainingWarpsTemp[i] = sRemainingWarpsStatic[i];
    }
    for (i = 0; i < ARRAY_COUNT(sB1WarpsTemp); i++) {
        sB1WarpsTemp[i] = sB1WarpsStatic[i];
    }
    for (i = 0; i < ARRAY_COUNT(sB2WarpsTemp); i++) {
        sB2WarpsTemp[i] = sB2WarpsStatic[i];
        sB3WarpsTemp[i] = sB3WarpsStatic[i];
    }
    for (i = 0; i < ARRAY_COUNT(gWarpDestinations); i++) {
        gWarpDestinations[i] = gWarpDestinationsStatic[i];
    }
}

static void init_warp_scramble() {
    int currentEntrance, index = 0;
    int warpsProcessed = 0, lobbyWarpsProcessed = 0, basementWarpsProcessed = 0, upstairsWarpsProcessed = 0;
    u8 tmp;
    tinymt32_t randomState;
    u16 failedScrambles = 0;
    tinymt32_init(&randomState, gRandomizerGameSeed);

    copy_remaining_warps();

    // Iterate backwards over every level
    // currentEntrance: Level ID of the entrance currently being randomized
    // gWarpDestinations[currentEntrance]: Level ID of the level the current entrance leads to.
    //     Will be the same as currentEntrance until set, or 0 if it doesnt exist.
    for (currentEntrance = 36; currentEntrance > 0; currentEntrance--) {
        if (failedScrambles > 50) {
            copy_remaining_warps();
            failedScrambles = 0;
            warpsProcessed = 0;
            lobbyWarpsProcessed = 0;
            basementWarpsProcessed = 0;
            upstairsWarpsProcessed = 0;
            currentEntrance = 36;
        }

        if (gWarpDestinations[currentEntrance] != 0) { // Ignore non-existant levels
            if (!gOptionsSettings.gameplay.s.keepStructure || gOptionsSettings.gameplay.s.adjustedExits) {
                index = (u8) get_val_in_range_uniform(warpsProcessed, 23, &randomState);

                // Forbidden cases
                // WMotR can't spawn in the BoB painting
                if ((currentEntrance == LEVEL_BOB) && (sRemainingWarpsTemp[index] == LEVEL_WMOTR)) {
                    currentEntrance += 1;
                    failedScrambles += 1;
                    continue;
                }

                // HMC and DDD can't spawn in the CotMC or BitFS entrances
                if ((currentEntrance == LEVEL_COTMC) || (currentEntrance == LEVEL_BITFS)) {
                    if ((sRemainingWarpsTemp[index] == LEVEL_HMC) || (sRemainingWarpsTemp[index] == LEVEL_DDD)) {
                        currentEntrance += 1;
                        failedScrambles += 1;
                        continue;
                    }
                }

                // If Keep Structure is on, additional requirements for Bowser levels
                if (gOptionsSettings.gameplay.s.keepStructure) {
                    // If the level is BitDW, it must spawn in the lobby
                    if (sRemainingWarpsTemp[index] == LEVEL_BITDW) {
                        if ((currentEntrance != LEVEL_BOB) &&
                            (currentEntrance != LEVEL_JRB) &&
                            (currentEntrance != LEVEL_WF) &&
                            (currentEntrance != LEVEL_CCM) &&
                            (currentEntrance != LEVEL_PSS) &&
                            (currentEntrance != LEVEL_BITDW) &&
                            (currentEntrance != LEVEL_SA) &&
                            (currentEntrance != LEVEL_TOTWC) &&
                            (currentEntrance != LEVEL_BBH)) {
                            currentEntrance += 1;
                            failedScrambles += 1;
                            continue;
                        }
                    }
                    // If the level is BitFS, it must spawn in the basement, and not in CotMC
                    if (sRemainingWarpsTemp[index] == LEVEL_BITFS) {
                        if ((currentEntrance != LEVEL_HMC) &&
                            (currentEntrance != LEVEL_LLL) &&
                            (currentEntrance != LEVEL_SSL) &&
                            (currentEntrance != LEVEL_DDD) &&
                            (currentEntrance != LEVEL_VCUTM)) {
                            currentEntrance += 1;
                            failedScrambles += 1;
                            continue;
                        }
                    }
                }

                // Use simple scramble algorithm to scramble the list
                tmp = sRemainingWarpsTemp[warpsProcessed];
                sRemainingWarpsTemp[warpsProcessed] = sRemainingWarpsTemp[index];
                sRemainingWarpsTemp[index] = tmp;

                gWarpDestinations[currentEntrance] = sRemainingWarpsTemp[warpsProcessed];
                warpsProcessed++;

            } else { // Keep Structure ON
                switch (currentEntrance) {
                    // Lobby courses
                    case LEVEL_BOB:
                    case LEVEL_JRB:
                    case LEVEL_WF:
                    case LEVEL_CCM:
                    case LEVEL_PSS:
                    case LEVEL_BITDW:
                    case LEVEL_SA:
                    case LEVEL_TOTWC:
                    case LEVEL_BBH:
                        index = (u8) get_val_in_range_uniform(lobbyWarpsProcessed, 9, &randomState);

                        // Courses can't lead to themselves
                        if (currentEntrance == sB1WarpsTemp[index]) {
                            currentEntrance += 1;
                            failedScrambles += 1;
                            continue;
                        }

                        tmp = sB1WarpsTemp[lobbyWarpsProcessed];
                        sB1WarpsTemp[lobbyWarpsProcessed] = sB1WarpsTemp[index];
                        sB1WarpsTemp[index] = tmp;

                        gWarpDestinations[currentEntrance] = sB1WarpsTemp[lobbyWarpsProcessed];
                        lobbyWarpsProcessed++;
                        break;

                    // Basement courses
                    case LEVEL_SSL:
                    case LEVEL_HMC:
                    case LEVEL_LLL:
                    case LEVEL_COTMC:
                    case LEVEL_DDD:
                    case LEVEL_BITFS:
                    case LEVEL_VCUTM:
                        index = (u8) get_val_in_range_uniform(basementWarpsProcessed, 7, &randomState);

                        // HMC, DDD and BitFS can't spawn in the CotMC or BitFS entrances
                        if ((currentEntrance == LEVEL_COTMC) || (currentEntrance == LEVEL_BITFS)) {
                            if ((sB2WarpsTemp[index] == LEVEL_HMC) || (sB2WarpsTemp[index] == LEVEL_DDD) || (sB2WarpsTemp[index] == LEVEL_BITFS)) {
                                currentEntrance += 1;
                                failedScrambles += 1;
                                continue;
                            }
                        }

                        // Courses can't lead to themselves
                        if (currentEntrance == sB2WarpsTemp[index]) {
                            currentEntrance += 1;
                            failedScrambles += 1;
                            continue;
                        }

                        tmp = sB2WarpsTemp[basementWarpsProcessed];
                        sB2WarpsTemp[basementWarpsProcessed] = sB2WarpsTemp[index];
                        sB2WarpsTemp[index] = tmp;

                        gWarpDestinations[currentEntrance] = sB2WarpsTemp[basementWarpsProcessed];
                        basementWarpsProcessed++;
                        break;

                    // Upstairs courses
                    default:
                        index = (u8) get_val_in_range_uniform(upstairsWarpsProcessed, 7, &randomState);

                        // Courses can't lead to themselves
                        if (currentEntrance == sB3WarpsTemp[index]) {
                            currentEntrance += 1;
                            failedScrambles += 1;
                            continue;
                        }

                        tmp = sB3WarpsTemp[upstairsWarpsProcessed];
                        sB3WarpsTemp[upstairsWarpsProcessed] = sB3WarpsTemp[index];
                        sB3WarpsTemp[index] = tmp;

                        gWarpDestinations[currentEntrance] = sB3WarpsTemp[upstairsWarpsProcessed];
                        upstairsWarpsProcessed++;
                        break;
                }

            }
        }
    }
}

void init_randomizer(s32 fileNum) {
    save_main_menu_data();
    save_file_set_seed_and_options(fileNum);
    init_warp_scramble();
    init_required_stars();
}

// stolen from stackoverflow
f32 hue_to_rgb(f32 p, f32 q, f32 t) {
    if (t < 0.f)
        t += 1.f;
    if (t > 1.f)
        t -= 1.f;

    if (t < 1/6.f)
        return p + (q - p) * 6 * t;
    if (t < 1/2.f)
        return q;
    if (t < 2/3.f)
        return p + (q - p) * (2/3.f - t) * 6;
    return p;
}

void hsl_to_rgb(u8 h, u8 s, u8 l, u8 *RGB) {
    f32 r,g,b;
    f32 hf = h / 255.0f;
    f32 sf = s / 255.0f;
    f32 lf = l / 255.0f;

    if (s == 0.f) {
        r = g = b = lf;
    } else {
        f32 q = (lf < 1/2.f) ? (lf * (1 + sf)) : (lf + sf - lf * sf);
        f32 p = 2 * lf - q;
        r = hue_to_rgb(p, q, hf + 1/3.f);
        g = hue_to_rgb(p, q, hf);
        b = hue_to_rgb(p, q, hf - 1/3.f);
    }

    RGB[0] = r * 255;
    RGB[1] = g * 255;
    RGB[2] = b * 255;
}

void get_random_color(u8 *RGB, tinymt32_t *randomState) {
    u32 rand = tinymt32_generate_u32(randomState);
    hsl_to_rgb(rand & 0xFF,
               0xFF,
               ((rand >> 8) & 0x7F) + ((rand >> 16) & 0x7F),
               RGB);
}

void init_star_color(struct Object *star, s32 courseID, s32 starID) {
    s32 index;
    switch (gOptionsSettings.cosmetic.s.starColors) {
        case STAR_COLOR_OFF:
            star->oStarColor = 0xFFFF29;
            return;
        case STAR_COLOR_PER_STAR:
            index = courseID * 8 + starID;
            break;
        case STAR_COLOR_PER_LEVEL:
            index = courseID;
            break;
        case STAR_COLOR_GLOBAL:
            index = 0;
            break;
    }
    tinymt32_t randomState;
    tinymt32_init(&randomState, index * 0x20000 + gRandomizerGameSeed);

    u8 RGB[3];
    get_random_color(RGB, &randomState);
    star->oStarColor = (RGB[0] << 16) | (RGB[1] << 8) | RGB[2];
}

void set_mario_light(Lights1 *light, u8 r, u8 g, u8 b) {
    light->a.l.col[0] = r / 2;
    light->a.l.col[1] = g / 2;
    light->a.l.col[2] = b / 2;
    light->a.l.colc[0] = light->a.l.col[0];
    light->a.l.colc[1] = light->a.l.col[1];
    light->a.l.colc[2] = light->a.l.col[2];
    light->l[0].l.col[0] = r;
    light->l[0].l.col[1] = g;
    light->l[0].l.col[2] = b;
    light->l[0].l.colc[0] = light->l[0].l.col[0];
    light->l[0].l.colc[1] = light->l[0].l.col[1];
    light->l[0].l.colc[2] = light->l[0].l.col[2];
}

void set_mario_light_random(Lights1 *light, tinymt32_t *randomState) {
    u8 RGB[3];
    get_random_color(RGB, randomState);
    u8 r = RGB[0];
    u8 g = RGB[1];
    u8 b = RGB[2];
    set_mario_light(light, r, g, b);
}

extern Lights1 mario_blue_lights_group;
extern Lights1 mario_red_lights_group;
extern Lights1 mario_white_lights_group;
extern Lights1 mario_brown1_lights_group;
extern Lights1 mario_beige_lights_group;
extern Lights1 mario_brown2_lights_group;
// 4 vertex colors each
extern Vtx coin_seg3_vertex_yellow[];
extern Vtx coin_seg3_vertex_red[];
extern Vtx coin_seg3_vertex_blue[];

void set_coin_color(u8 r, u8 g, u8 b, Vtx *d) {
    Vtx *a = segmented_to_virtual(d);
    u32 i;
    for (i = 0; i < 4; i++) {
        a[0].v.cn[0] = r;
        a[1].v.cn[0] = r;
        a[2].v.cn[0] = r;
        a[3].v.cn[0] = r;
        a[0].v.cn[1] = g;
        a[1].v.cn[1] = g;
        a[2].v.cn[1] = g;
        a[3].v.cn[1] = g;
        a[0].v.cn[2] = b;
        a[1].v.cn[2] = b;
        a[2].v.cn[2] = b;
        a[3].v.cn[2] = b;
    }
}

f32 RMSE(u8 r1, u8 r2, u8 g1, u8 g2, u8 b1, u8 b2) {
    f32 r, g, b;
    r = r1 - r2;
    g = g1 - g2;
    b = b1 - b2;
    return sqrtf(r * r + g * g + b * b);
}

#define MINDIFF 140.f //might be infinite loop idk how this works
void set_mario_rando_colors(void) {
    tinymt32_t randomState;

    if (gOptionsSettings.cosmetic.s.marioColors) {
        if (gRandomizerGameSeed == 2401) {
            set_mario_light(segmented_to_virtual(&mario_red_lights_group), 0, 255, 0);
        } else {
            tinymt32_init(&randomState, gRandomizerGameSeed);

            set_mario_light_random(segmented_to_virtual(&mario_blue_lights_group), &randomState);
            set_mario_light_random(segmented_to_virtual(&mario_red_lights_group), &randomState);
            set_mario_light_random(segmented_to_virtual(&mario_white_lights_group), &randomState);
            set_mario_light_random(segmented_to_virtual(&mario_brown1_lights_group), &randomState);
            if (gOptionsSettings.cosmetic.s.marioColors == 2) {
                set_mario_light_random(segmented_to_virtual(&mario_beige_lights_group), &randomState);
                set_mario_light_random(segmented_to_virtual(&mario_brown2_lights_group), &randomState);
            }
        }
    }

    if (gOptionsSettings.cosmetic.s.coinsOn) {
        u8 yellows[3];
        u8 reds[3];
        u8 blues[3];
        tinymt32_init(&randomState, gRandomizerGameSeed + 1);

        get_random_color(yellows, &randomState);
        set_coin_color(yellows[0], yellows[1], yellows[2], coin_seg3_vertex_yellow);

        get_random_color(reds, &randomState);
        while (RMSE(yellows[0], reds[0], yellows[1], reds[1], yellows[2], reds[2]) < MINDIFF) {
            get_random_color(reds, &randomState);
        }
        set_coin_color(reds[0], reds[1], reds[2], coin_seg3_vertex_red);

        get_random_color(blues, &randomState);
        while ((RMSE(yellows[0], reds[0], yellows[1], reds[1], yellows[2], reds[2]) < MINDIFF)
               || (RMSE(yellows[0], blues[0], yellows[1], blues[1], yellows[2], blues[2]) < MINDIFF)
               || (RMSE(reds[0], blues[0], reds[1], blues[1], reds[2], blues[2]) < MINDIFF)) {
            get_random_color(blues, &randomState);
        }
        set_coin_color(blues[0], blues[1], blues[2], coin_seg3_vertex_blue);
    }
}
