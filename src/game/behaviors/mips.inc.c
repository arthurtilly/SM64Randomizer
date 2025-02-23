#include "game/randomizer.h"
#include "game/area.h"
#include "game/save_file.h"
#include "game/object_list_processor.h"
#include "game/obj_behaviors.h"
#include "game/interaction.h"
#include "behavior_data.h"

/**
 * Behavior for MIPS (everyone's favorite yellow rabbit).
 */

/**
 * Initializes MIPS' physics parameters and checks if he should be active,
 * hiding him if necessary.
 */
void bhv_mips_init(void) {
    // Retrieve star flags for Castle Secret Stars on current save file.
    u8 starFlags = save_file_get_star_flags(gCurrSaveFileNum - 1, COURSE_NUM_TO_INDEX(COURSE_NONE));

    if (GET_BPARAM1(o->oBehParams) == MIPS_BP_STAR_1
        && !(starFlags & SAVE_FLAG_TO_STAR_FLAG(SAVE_FLAG_COLLECTED_MIPS_STAR_1))) {
        o->oBehParams2ndByte    = MIPS_BP_STAR_1;
        o->oMipsForwardVelocity = 40.0f;
    } else if (GET_BPARAM1(o->oBehParams) == MIPS_BP_STAR_2
        && !(starFlags & SAVE_FLAG_TO_STAR_FLAG(SAVE_FLAG_COLLECTED_MIPS_STAR_2))) {
        o->oBehParams2ndByte    = MIPS_BP_STAR_2;
        o->oMipsForwardVelocity = 45.0f;
    } else {
        // No MIPS stars are available, hide MIPS.
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }

    vec3f_copy(&o->oMipsSafeFloorVec, &o->oPosVec);
}

static s32 mips_get_floor_class(void) {
    s32 floorClass;

    floorClass = SURFACE_CLASS_DEFAULT;

    if (o->oFloor != NULL) {
        switch (o->oFloor->type) {
            case SURFACE_NOT_SLIPPERY:
            case SURFACE_HARD_NOT_SLIPPERY:
            case SURFACE_SWITCH:
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
            case SURFACE_NOISE_VERY_SLIPPERY_73:
            case SURFACE_NOISE_VERY_SLIPPERY_74:
            case SURFACE_NOISE_VERY_SLIPPERY:
            case SURFACE_NO_CAM_COL_VERY_SLIPPERY:
                floorClass = SURFACE_CLASS_VERY_SLIPPERY;
                break;
        }
    }

    return floorClass;
}

static u32 mips_floor_is_slippery(void) {
    f32 normY;

    if ((gCurrentArea->terrainType & TERRAIN_MASK) == TERRAIN_SLIDE && o->oFloor->normal.y < COS1) {
        return TRUE;
    }

    switch (mips_get_floor_class()) {
        case SURFACE_CLASS_VERY_SLIPPERY: normY = COS10; break;
        case SURFACE_CLASS_SLIPPERY:      normY = COS20; break;
        default:                          normY = COS38; break;
        case SURFACE_CLASS_NOT_SLIPPERY:  normY = 0.0f;  break;
    }

    return o->oFloor->normal.y <= normY;
}

static u32 mips_is_safe_floor(void) {
    if (o->oFloor == NULL || mips_floor_is_slippery()) {
        return FALSE;
    }
    if (o->oFloorType > SURFACE_SAFE_FLOORS_GENERAL) {
        return FALSE;
    }
    struct AreaParams *areaParams = &(*sLevelParams[gCurrLevelNum - 4])[gCurrAreaIndex - 1];
    if (!(areaParams->areaParamFlags & AREA_PARAM_FLAG_CHANGING_WATER_LEVEL) && find_water_level(o->oPosX, o->oPosZ) > o->oPosY) {
        return FALSE;
    }
    return TRUE;
}

void bhv_mips_act_wait_for_nearby_mario(void) {
    if (o->oDistanceToMario < 500.0f) {
        cur_obj_init_animation(1);
        o->oAction = MIPS_ACT_RUN;
        o->oForwardVel = 10.0f;
        o->oMoveAngleYaw = o->oAngleToMario + 0x8000;
    } else if (o->oDistanceToMario > 1000.0f && !mips_is_safe_floor()) {
        f32 homeDistX = o->oMipsSafeFloorX - o->oPosX;
        f32 homeDistZ = o->oMipsSafeFloorZ - o->oPosZ;
        s16 angleTowardsHome = atan2s(homeDistZ, homeDistX);

        if (!is_point_within_radius_of_mario(o->oMipsSafeFloorX, o->oMipsSafeFloorY, o->oMipsSafeFloorZ, 500)) {
            cur_obj_init_animation(1);
            o->oAction = MIPS_ACT_RETURN_TO_SAFE_GROUND;
            o->oForwardVel = 10.0f;
            o->oMoveAngleYaw = angleTowardsHome;
        }
    }
}

static void mips_hard_turn(void) {
    o->oForwardVel -= 20.0f;
    if (o->oForwardVel < 10.0f) {
        o->oForwardVel = 10.0f;
    }
}

static u32 mips_avoid_walls_and_edges(void) {
    if (o->oMoveFlags & OBJ_MOVE_HIT_WALL) {
        s16 angleDiff = abs_angle_diff(o->oMoveAngleYaw, o->oWallAngle);
        if (angleDiff > 0x4000) {
            if (angleDiff > 0x6000) {
                o->oMoveAngleYaw = approach_angle(o->oWallAngle, o->oMoveAngleYaw, 0x8000 - angleDiff);
                mips_hard_turn();
            } else {
                o->oMoveAngleYaw = approach_angle(o->oWallAngle, o->oMoveAngleYaw, 0x4000);
            }
        }
        return TRUE;
    } else if (o->oMoveFlags & OBJ_MOVE_HIT_EDGE) {
        s16 angle;
        for (angle = 0x400; angle <= 0x8000; angle += 0x400) {
            f32 dx, dz;
            f32 leftTurnFloorHeight, rightTurnFloorHeight;

            // Check left turn
            dx = o->oForwardVel * sins(o->oMoveAngleYaw + angle);
            dz = o->oForwardVel * coss(o->oMoveAngleYaw + angle);
            leftTurnFloorHeight = find_floor_height(o->oPosX + dx, o->oPosY, o->oPosZ + dz);

            // Check right turn
            dx = o->oForwardVel * sins(o->oMoveAngleYaw - angle);
            dz = o->oForwardVel * coss(o->oMoveAngleYaw - angle);
            rightTurnFloorHeight = find_floor_height(o->oPosX + dx, o->oPosY, o->oPosZ + dz);

            f32 heightCheck = o->oFloorHeight - 50.0f;

            if (leftTurnFloorHeight >= heightCheck && rightTurnFloorHeight >= heightCheck) {
                o->oMoveAngleYaw += leftTurnFloorHeight > rightTurnFloorHeight ? angle : -angle;
                break;
            } else if (leftTurnFloorHeight >= heightCheck) {
                o->oMoveAngleYaw += angle;
                break;
            } else if (rightTurnFloorHeight >= heightCheck) {
                o->oMoveAngleYaw -= angle;
                break;
            }
        }
        if (angle > 0x2000) {
            mips_hard_turn();
        }
        return TRUE;
    }
    return FALSE;
}

static void mips_run_sound(void) {
    // Play sounds during walk animation.
    // if (cur_obj_check_if_near_animation_end() && (collisionFlags & OBJ_COL_FLAG_UNDERWATER)) {
    //     cur_obj_play_sound_2(SOUND_OBJ_MIPS_RABBIT_WATER);
    //     spawn_object(o, MODEL_NONE, bhvShallowWaterSplash);
    // } else 
    if (cur_obj_check_if_near_animation_end()) {
        cur_obj_play_sound_2(SOUND_OBJ_MIPS_RABBIT);
    }
}

void bhv_mips_act_run(void) {
    cur_obj_forward_vel_approach_upward(o->oMipsForwardVelocity, 2.0f);

    if (mips_is_safe_floor()) {
        vec3f_copy(&o->oMipsSafeFloorVec, &o->oPosVec);
    }

    if (!mips_avoid_walls_and_edges()) {
        s16 goalYaw = o->oAngleToMario + 0x8000;
        cur_obj_rotate_yaw_toward(goalYaw, 0x800);
    }

    if (o->oDistanceToMario > 1000.0f) {
        o->oAction = MIPS_ACT_WAIT_FOR_ANIMATION_DONE;
    }

    mips_run_sound();
}

void bhv_mips_act_return_to_safe_ground(void) {
    if (o->oDistanceToMario < 500.0f) {
        o->oAction = MIPS_ACT_RUN;
        o->oForwardVel = o->oMipsForwardVelocity;
        o->oMoveAngleYaw = o->oAngleToMario + 0x8000;
    }

    if (mips_is_safe_floor()) {
        o->oAction = MIPS_ACT_WAIT_FOR_ANIMATION_DONE;
        return;
    }

    if (obj_return_home_if_safe(o, o->oMipsSafeFloorX, o->oMipsSafeFloorY, o->oMipsSafeFloorZ, 500)) {
        o->oAction = MIPS_ACT_WAIT_FOR_ANIMATION_DONE;
    }

    u32 respawn = FALSE;
    u32 flags = OBJ_MOVE_ON_GROUND | OBJ_MOVE_ABOVE_DEATH_BARRIER;
    if ((o->oMoveFlags & flags) == flags) {
        respawn = TRUE;
    }

    if (o->activeFlags & ACTIVE_FLAG_FAR_AWAY) {
        respawn = TRUE;
    }
    
    if (respawn) {
        cur_obj_hide();
        cur_obj_set_pos_to_home_and_stop();
        cur_obj_init_animation(0);
        o->oAction = MIPS_ACT_WAIT_TO_RESPAWN;
    }

    mips_avoid_walls_and_edges();

    mips_run_sound();
}

void bhv_mips_act_wait_to_respawn(void) {
    if (o->oDistanceToMario > o->oDrawingDistance) {
        cur_obj_unhide();
        o->oAction = MIPS_ACT_WAIT_FOR_NEARBY_MARIO;
    }
}

/**
 * Seems to wait until the current animation is done, then go idle.
 */
void bhv_mips_act_wait_for_animation_done(void) {
    if (cur_obj_check_if_near_animation_end()) {
        o->oForwardVel = 0.0f;
        cur_obj_init_animation(0);
        o->oAction = MIPS_ACT_WAIT_FOR_NEARBY_MARIO;
    }
}

/**
 * Handles MIPS falling down after being thrown.
 */
void bhv_mips_act_fall_down(void) {
    s16 collisionFlags = object_step();

    o->header.gfx.animInfo.animFrame = 0;

    if (collisionFlags & OBJ_COL_FLAG_GROUNDED) {
        o->oAction = MIPS_ACT_WAIT_FOR_ANIMATION_DONE;

        o->oFlags |= OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
        o->oMoveAngleYaw = o->oFaceAngleYaw;

        if (collisionFlags & OBJ_COL_FLAG_UNDERWATER) {
            spawn_object(o, MODEL_NONE, bhvShallowWaterSplash);
        }
    }
}

/**
 * Idle loop, after you catch MIPS and put him down.
 */
void bhv_mips_act_idle(void) {
    o->oForwardVel = 0.0f;

    // Spawn a star if he was just picked up for the first time.
    if (o->oMipsStarStatus == MIPS_STAR_STATUS_SHOULD_SPAWN_STAR) {
        bhv_spawn_star_no_level_exit((o->oBehParams2ndByte + 3) | STAR_BP_FLAG_COURSE_NONE);
        o->oMipsStarStatus = MIPS_STAR_STATUS_ALREADY_SPAWNED_STAR;
    }
}

ObjActionFunc sMipsActions[] = {
    bhv_mips_act_wait_for_nearby_mario,
    bhv_mips_act_run,
    bhv_mips_act_wait_for_animation_done,
    bhv_mips_act_fall_down,
    bhv_mips_act_idle,
    bhv_mips_act_return_to_safe_ground,
    bhv_mips_act_wait_to_respawn,
};

static void mips_update_floor(void) {
    o->oMoveFlags &= ~(OBJ_MOVE_ABOVE_LAVA | OBJ_MOVE_ABOVE_DEATH_BARRIER);

    struct Surface *floor = cur_obj_update_floor_height_and_get_floor();
    o->oFloor = floor;

    if (floor != NULL) {
        SurfaceType floorType = floor->type;
        if (floorType == SURFACE_BURNING) {
            o->oMoveFlags |= OBJ_MOVE_ABOVE_LAVA;
        } else if ((floorType == SURFACE_DEATH_PLANE) || (floorType == SURFACE_VERTICAL_WIND)) {
            //! This maybe misses SURFACE_WARP
            o->oMoveFlags |= OBJ_MOVE_ABOVE_DEATH_BARRIER;
        }

        o->oFloorType = floorType;
        o->oFloorRoom = floor->room;
    } else {
        o->oFloorType = SURFACE_DEFAULT;
        o->oFloorRoom = 0;
    }
}

/**
 * Handles all the actions MIPS does when he is not held.
 */
void bhv_mips_free(void) {
    f32 prevVec[3];
    vec3f_copy(&prevVec, &o->oPosVec);
    cur_obj_update_floor_and_walls();
    if (o->oMoveFlags & OBJ_MOVE_IN_AIR) {
        vec3f_copy(&o->oPosVec, &prevVec);
        mips_update_floor();
        o->oMoveFlags &= ~OBJ_MOVE_IN_AIR;
        o->oMoveFlags |= OBJ_MOVE_HIT_EDGE;
    }
    cur_obj_call_action_function(sMipsActions);
    cur_obj_move_standard(-78);
    o->oPosY = find_floor_height(o->oPosX, o->oPosY, o->oPosZ);
}

/**
 * Handles MIPS being held by Mario.
 */
void bhv_mips_held(void) {
    s16 dialogID;

    o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;
    cur_obj_init_animation(4); // Held animation.
    cur_obj_set_pos_relative(gMarioObject, 0, 60.0f, 100.0f);
    cur_obj_become_intangible();

    // If MIPS hasn't spawned his star yet...
    if (o->oMipsStarStatus == MIPS_STAR_STATUS_HAVENT_SPAWNED_STAR) {
        // Choose dialog based on which MIPS encounter this is.
        if (o->oBehParams2ndByte == MIPS_BP_STAR_1) {
            dialogID = DIALOG_084;
        } else {
            dialogID = DIALOG_162;
        }

        if (set_mario_npc_dialog(MARIO_DIALOG_LOOK_FRONT) == MARIO_DIALOG_STATUS_SPEAK) {
            o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
            if (cutscene_object_with_dialog(CUTSCENE_DIALOG, o, dialogID)) {
                o->oInteractionSubtype |= INT_SUBTYPE_DROP_IMMEDIATELY;
                o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
                o->oMipsStarStatus = MIPS_STAR_STATUS_SHOULD_SPAWN_STAR;
                set_mario_npc_dialog(MARIO_DIALOG_STOP);
            }
        }
    }
}

/**
 * Handles MIPS being dropped by Mario.
 */
void bhv_mips_dropped(void) {
    cur_obj_get_dropped();
    o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
    cur_obj_init_animation(0);
    o->oHeldState = HELD_FREE;
    cur_obj_become_tangible();
    o->oForwardVel = 3.0f;
    o->oAction = MIPS_ACT_IDLE;
}

/**
 * Handles MIPS being thrown by Mario.
 */
void bhv_mips_thrown(void) {
    cur_obj_enable_rendering();
    o->header.gfx.node.flags &= ~GRAPH_RENDER_INVISIBLE;
    o->oHeldState = HELD_FREE;
    o->oFlags &= ~OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW;
    cur_obj_init_animation(2);
    cur_obj_become_tangible();
    o->oForwardVel = 25.0f;
    o->oVelY = 20.0f;
    o->oAction = MIPS_ACT_FALL_DOWN;
}

/**
 * MIPS' main loop.
 */
void bhv_mips_loop(void) {
    // Determine what to do based on MIPS' held status.
    switch (o->oHeldState) {
        case HELD_FREE:
            bhv_mips_free();
            break;

        case HELD_HELD:
            bhv_mips_held();
            break;

        case HELD_THROWN:
            bhv_mips_thrown();
            break;

        case HELD_DROPPED:
            bhv_mips_dropped();
            break;
    }
}
