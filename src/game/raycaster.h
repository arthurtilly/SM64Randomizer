#ifndef _RAYCASTER_H
#define _RAYCASTER_H

#define WALL_CHECK_RAYCASTER_NUM_RAYS_TO_CAST 6
#define WALL_CHECK_RAYCASTER_STEP_SIZE 100.0f
#define WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST 500.0f

extern f32 gWallCheckRaycasterSearchDist;
extern u8 gWallCheckNumRaySteps;

extern void vec3s_resolve_wall_collisions(Vec3s pos, f32 radius);
extern void update_raycaster_params(void);
extern u8 is_safe_near_walls(Vec3s pos, u8 killOnOob);

#endif
