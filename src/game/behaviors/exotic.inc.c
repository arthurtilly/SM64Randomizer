// exotic.inc.c

struct ExoticObject {
    u32 model;
    void *bhv;
};

struct ExoticObject sExoticObjects[] = {
    { MODEL_BOWLING_BALL, bhvFireSpitter },
};

void bhv_exotic(void) {
    struct Object *obj = spawn_object(o, sExoticObjects[0].model, sExoticObjects[0].bhv);
    obj->pointerSeed = o->pointerSeed;
}
