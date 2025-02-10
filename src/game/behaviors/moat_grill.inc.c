// moat_grill.inc.c

void bhv_moat_grills_loop(void) {
    u32 locked = !(save_file_get_flags() & SAVE_FLAG_MOAT_DRAINED);

    if (get_intended_level_is_locked(LEVEL_VCUTM)) {
        locked = TRUE;
    }
    if (!locked) {
        cur_obj_set_model(MODEL_NONE);
    } else {
        load_object_collision_model();
    }
}
