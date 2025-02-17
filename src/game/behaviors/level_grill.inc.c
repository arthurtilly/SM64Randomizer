// level_grill.inc.c

void bhv_level_grill_init(void) {
    u8 intendedLevel = BPARAM1;
    u8 large = BPARAM2;
    if (large) {
        obj_scale_xyz(o, 2.5f, 2.5f, 2.5f);
    }
    u32 locked = get_intended_level_is_locked(intendedLevel);
    if (intendedLevel == LEVEL_BITFS && !(save_file_get_star_flags(gCurrSaveFileNum - 1, COURSE_NUM_TO_INDEX(COURSE_DDD)) & BOARD_BOWSERS_SUB)) {
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
