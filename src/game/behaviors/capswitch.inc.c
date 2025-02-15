// capswitch.inc.c

static s32 sCapSaveFlags[] = {
    SAVE_FLAG_HAVE_WING_CAP,
    SAVE_FLAG_HAVE_METAL_CAP,
    SAVE_FLAG_HAVE_VANISH_CAP,
    SAVE_FLAG_HAVE_KOOPA_SHELL,
};

int count_pressed_switches(void) {
    s32 count = 0;

    for (int i = 0; i < 4; i++) {
        if (save_file_get_flags() & sCapSaveFlags[i]) {
            count++;
        }
    }

    return count;
}

void cap_switch_act_init(void) {
    o->oAnimState = o->oBehParams2ndByte;
    cur_obj_scale(0.5f);
    o->oPosY += 71.0f;

    if (!IS_120_STAR || (gMarioState->numStars < CAP_SWITCH_THRESHOLD)) {
        mark_obj_for_deletion(o);
        return;
    }

    if (count_pressed_switches() >= 2 && !(save_file_get_flags() & sCapSaveFlags[o->oBehParams2ndByte])) {
        mark_obj_for_deletion(o);
        return;
    }

    o->prevObj = spawn_object_relative_with_scale(0, 0, -71, 0, 0.5f, o, MODEL_CAP_SWITCH_BASE, bhvCapSwitchBase);
#ifdef ENABLE_VANILLA_LEVEL_SPECIFIC_CHECKS
    if (gCurrLevelNum != LEVEL_UNKNOWN_32) {
        if (save_file_get_flags() & sCapSaveFlags[o->oBehParams2ndByte]) {
            o->oAction = CAP_SWITCH_ACT_IDLE_PRESSED;
            o->header.gfx.scale[1] = 0.1f;
        } else {
            o->oAction = CAP_SWITCH_ACT_IDLE_UNPRESSED;
        }
    } else {
        o->oAction = CAP_SWITCH_ACT_IDLE_UNPRESSED;
    }
#else
    if (save_file_get_flags() & sCapSaveFlags[o->oBehParams2ndByte]) {
        o->oAction = CAP_SWITCH_ACT_IDLE_PRESSED;
        o->header.gfx.scale[1] = 0.1f;
    } else {
        o->oAction = CAP_SWITCH_ACT_IDLE_UNPRESSED;
    }
#endif
}

void cap_switch_act_idle_unpressed(void) {
    if (count_pressed_switches() >= 2) {
        spawn_mist_particles();
        mark_obj_for_deletion(o);
        mark_obj_for_deletion(o->prevObj);
        return;
    }
    if (cur_obj_is_mario_on_platform()) {
        save_file_set_flags(sCapSaveFlags[o->oBehParams2ndByte]);
        o->oAction = CAP_SWITCH_ACT_BEING_PRESSED;
        cur_obj_play_sound_2(SOUND_GENERAL_ACTIVATE_CAP_SWITCH);
    }
}

void cap_switch_act_being_pressed(void) {
    if (o->oTimer < 5) {
        cur_obj_scale_over_time(SCALE_AXIS_Y, 4, 0.5f, 0.1f);
        if (o->oTimer == 4) {
            cur_obj_shake_screen(SHAKE_POS_SMALL);
            spawn_mist_particles();
            spawn_triangle_break_particles(60, MODEL_CARTOON_STAR, 0.3f, o->oBehParams2ndByte);
#if ENABLE_RUMBLE
            queue_rumble_data(5, 80);
#endif
        }
    } else {
        //! Neither of these flags are defined in this function so they do nothing.
        //  On an extra note, there's a specific check for this cutscene and 
        //  there's no dialog defined since the cutscene itself calls the dialog.
        // s32 dialogResponse = cur_obj_update_dialog_with_cutscene(MARIO_DIALOG_LOOK_FRONT, 
        //     (DIALOG_FLAG_TEXT_RESPONSE | DIALOG_FLAG_UNK_CAPSWITCH), CUTSCENE_CAP_SWITCH_PRESS, 0);
        // if (dialogResponse) {
        save_file_do_save(gCurrSaveFileNum - 1);
        o->oAction = CAP_SWITCH_ACT_IDLE_PRESSED;
    }
}

// dead function
void cap_switch_act_idle_pressed(void) {
}

ObjActionFunc sCapSwitchActions[] = {
    cap_switch_act_init,
    cap_switch_act_idle_unpressed,
    cap_switch_act_being_pressed,
    cap_switch_act_idle_pressed,
};

void bhv_cap_switch_loop(void) {
    cur_obj_call_action_function(sCapSwitchActions);
}
