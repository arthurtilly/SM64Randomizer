// orange_number.inc.c

void bhv_orange_number_init(void) {
    o->oAnimState = o->oBehParams2ndByte;
    o->oVelY = 26.0f;
}

void bhv_orange_number_loop(void) {
    if (o->oOrangeNumType == 0) {
        o->oPosY += o->oVelY;
        o->oVelY -= 2.0f;

        if (o->oVelY < -21.0f) {
            o->oVelY = 14.0f;
        }

        if (o->oTimer == 35) {
            struct Object *sparkleObj = spawn_object(o, MODEL_SPARKLES, bhvCoinSparklesSpawner);
            sparkleObj->oPosY -= 30.0f;
            obj_mark_for_deletion(o);
        }
    } else {
        o->oPosX = o->parentObj->oPosX;
        o->oPosY = o->parentObj->oPosY + (o->oOrangeNumType == 1 ? 150 : 250);
        o->oPosZ = o->parentObj->oPosZ;
    }
}
