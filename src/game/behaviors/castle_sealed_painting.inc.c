// castle_sealed_painting.inc.c

void bhv_castle_sealed_painting_init(void) {
    u8 intendedLevel = BPARAM1;
    u8 scaleN = BPARAM2;
    u8 scaleD = BPARAM3;
    cur_obj_scale(((f32)scaleN)/((f32)scaleD));
    if (!get_intended_level_is_locked(intendedLevel)) {
        obj_mark_for_deletion(o);
    }
}

void bhv_castle_sealed_painting_loop(void) {
    if (!(gMarioStates[0].action & ACT_FLAG_INTANGIBLE)) {
        load_object_collision_model();
    }
}
