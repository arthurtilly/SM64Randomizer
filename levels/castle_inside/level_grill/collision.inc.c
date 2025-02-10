// 0x07078DE4 - 0x07078E14
const Collision inside_castle_seg7_collision_level_grill[] = {
    COL_INIT(),
    COL_VERTEX_INIT(0x4),
    COL_VERTEX(154,  0, -154),
    COL_VERTEX(-154, 0, -154),
    COL_VERTEX(154,  0, 154),
    COL_VERTEX(-154, 0, 154),
    COL_TRI_INIT(SURFACE_DEFAULT, 2),
    COL_TRI(0, 1, 2),
    COL_TRI(1, 3, 2),
    COL_TRI_STOP(),
    COL_END(),
};
