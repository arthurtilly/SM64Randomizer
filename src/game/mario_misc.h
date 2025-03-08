#ifndef MARIO_MISC_H
#define MARIO_MISC_H

#include <PR/ultratypes.h>

#include "macros.h"
#include "types.h"
#include "config.h"
#include "dialog_ids.h"

extern struct GraphNodeObject gMirrorMario;
extern struct MarioBodyState gBodyStates[2];

#define TOAD_STAR_1_DIALOG DIALOG_082
#define TOAD_STAR_2_DIALOG DIALOG_076
#define TOAD_STAR_3_DIALOG DIALOG_083

#ifdef KEEP_MARIO_HEAD
Gfx *geo_draw_mario_head_goddard(s32 callContext, struct GraphNode *node, Mat4 *mtx);
#endif
void bhv_toad_message_loop(void);
void bhv_toad_message_init(void);
void bhv_unlock_door_star_init(void);
void bhv_unlock_door_star_loop(void);
Gfx *geo_mirror_mario_set_alpha(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_switch_mario_eyes(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_mario_tilt_torso(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_mario_head_rotation(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_switch_mario_hand(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_mario_hand_foot_scaler(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_switch_mario_alpha(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_switch_mario_env_map(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_mario_revert_dither(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_switch_mario_cap_on_off(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_mario_rotate_wing_cap_wings(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_switch_mario_hand_grab_pos(s32 callContext, struct GraphNode *node, Mat4 *mtx);
Gfx *geo_render_mirror_mario(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);
Gfx *geo_mirror_mario_backface_culling(s32 callContext, struct GraphNode *node, UNUSED Mat4 *mtx);

#endif // MARIO_MISC_H
