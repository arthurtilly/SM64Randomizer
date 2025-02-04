// exotic.inc.c

struct ExoticObject {
    u32 model;
    const void *bhv;
};

struct ExoticObject sExoticObjects[] = {
    { MODEL_HEAVE_HO, bhvHeaveHo },
    { MODEL_SPINDRIFT, bhvSpindrift },
    { MODEL_MR_BLIZZARD_HIDDEN, bhvMrBlizzard },
    { MODEL_PIRANHA_PLANT, bhvFirePiranhaPlant },
    { MODEL_AMP, bhvHomingAmp },
    { MODEL_SCUTTLEBUG, bhvScuttlebug },
    { MODEL_MR_I_BODY, bhvMrI },
    { MODEL_BULLY, bhvSmallBully },
    { MODEL_ENEMY_LAKITU, bhvEnemyLakitu },
    { MODEL_SKEETER, bhvSkeeter },
    { MODEL_BOO, bhvGhostHuntBoo },
    { MODEL_WHOMP, bhvSmallWhomp },
    { MODEL_THWOMP, bhvThwomp },
    { MODEL_SNUFIT, bhvSnufit },
    { MODEL_BOWLING_BALL, bhvFireSpitter },
    { MODEL_CHUCKYA, bhvChuckya },
    { MODEL_CHUCKYA, bhvChuckya },
};

void bhv_exotic(void) {
    int index = random_u16_seeded(o->pointerSeed) % ARRAY_COUNT(sExoticObjects);
    struct Object *obj = spawn_object(o, sExoticObjects[index].model, sExoticObjects[index].bhv);
    obj->pointerSeed = o->pointerSeed;

    if (index == 10) {
        OR_BPARAM1(obj->oBehParams, COIN_INSIDE_BOO_BP_YELLOW_COIN);
    }
}
