// castle_sealed_painting.inc.c

void bhv_castle_sealed_painting_init(void) {
    u8 intendedLevel = BPARAM1;
    u8 scaleN = BPARAM2;
    u8 scaleD = BPARAM3;
    cur_obj_scale(((f32)scaleN)/((f32)scaleD));
    s16 targetLevel = gOptionsSettings.gameplay.s.randomLevelWarp ? gWarpDestinations[intendedLevel] : intendedLevel;
    if (targetLevel == 0) {
        targetLevel = intendedLevel;
    }
    if (!get_level_is_locked(targetLevel)) {
        obj_mark_for_deletion(o);
    }
}
