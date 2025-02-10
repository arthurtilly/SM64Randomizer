// level_grill.inc.c

void bhv_level_grill_init(void) {
    u8 intendedLevel = BPARAM1;
    u8 large = BPARAM2;
    if (large) {
        obj_scale_xyz(o, 2.5f, 2.5f, 2.5f);
    }
    u32 locked = get_intended_level_is_locked(intendedLevel);
    if (intendedLevel == LEVEL_BITFS && !(gDddPaintingStatus & DDD_FLAG_BOWSERS_SUB_BEATEN)) {
        locked = TRUE;
    }
    if (!locked) {
        obj_mark_for_deletion(o);
    }
}

void bhv_level_grill_loop(void) {
    if (!(gMarioStates[0].action & ACT_FLAG_INTANGIBLE)) {
        load_object_collision_model();
    }
}
