// ddd_warp.inc.c

void bhv_ddd_warp_loop(void) {
    u8 intendedLevel = LEVEL_DDD;
    s16 targetLevel = gOptionsSettings.gameplay.s.randomLevelWarp ? gWarpDestinations[intendedLevel] : intendedLevel;
    if (targetLevel == 0) {
        targetLevel = intendedLevel;
    }
    if (get_level_is_locked(targetLevel)) {
        o->collisionData = segmented_to_virtual(inside_castle_seg7_collision_ddd_warp_3);
        return;
    }

#ifndef UNLOCK_ALL
    if (gDddPaintingStatus & DDD_FLAG_BOWSERS_SUB_BEATEN) {
#endif
        o->collisionData = segmented_to_virtual(inside_castle_seg7_collision_ddd_warp_2);
#ifndef UNLOCK_ALL
    } else {
        o->collisionData = segmented_to_virtual(inside_castle_seg7_collision_ddd_warp);
    }
#endif
}
