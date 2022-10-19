#include <ultra64.h>

#include "sm64.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "area.h"
#include "engine/math_util.h"
#include "raycaster.h"
#include "randomizer.h"

f32 gWallCheckRaycasterSearchDist = WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST;
u8 gWallCheckNumRaySteps = 0;

Vec3f sRaycastDirections[WALL_CHECK_RAYCASTER_NUM_RAYS_TO_CAST];
u8 sRaycasterInitialized = FALSE;

static struct Surface *return_first_wall_collision(struct SurfaceNode *surfaceNode, struct WallCollisionData *data) {
    f32 offset;
    f32 radius = data->radius;
    struct Surface *surf;
    f32 x = data->x;
    f32 y = data->y;
    f32 z = data->z;
    f32 px, pz;
    f32 w1, w2, w3;
    f32 y1, y2, y3;

    while (surfaceNode != NULL)
    {
        surf = surfaceNode->surface;
        surfaceNode = surfaceNode->next;

        if (y < surf->lowerY || y > surf->upperY)
            continue;

        offset = surf->normal.x * x + surf->normal.y * y + surf->normal.z * z + surf->originOffset;

        if (offset < -radius || offset > radius)
            continue;

        px = x;
        pz = z;

        if (surf->flags & SURFACE_FLAG_X_PROJECTION)
        {
            w1 = -surf->vertex1[2];
            w2 = -surf->vertex2[2];
            w3 = -surf->vertex3[2];
            y1 = surf->vertex1[1];
            y2 = surf->vertex2[1];
            y3 = surf->vertex3[1];

            if (surf->normal.x > 0.0f)
            {
                if ((y1 - y) * (w2 - w1) - (w1 - -pz) * (y2 - y1) > 0.0f) continue;
                if ((y2 - y) * (w3 - w2) - (w2 - -pz) * (y3 - y2) > 0.0f) continue;
                if ((y3 - y) * (w1 - w3) - (w3 - -pz) * (y1 - y3) > 0.0f) continue;
            }
            else
            {
                if ((y1 - y) * (w2 - w1) - (w1 - -pz) * (y2 - y1) < 0.0f) continue;
                if ((y2 - y) * (w3 - w2) - (w2 - -pz) * (y3 - y2) < 0.0f) continue;
                if ((y3 - y) * (w1 - w3) - (w3 - -pz) * (y1 - y3) < 0.0f) continue;
            }
        }
        else
        {
            w1 = surf->vertex1[0];
            w2 = surf->vertex2[0];
            w3 = surf->vertex3[0];
            y1 = surf->vertex1[1];
            y2 = surf->vertex2[1];
            y3 = surf->vertex3[1];

            if (surf->normal.z > 0.0f)
            {
                if ((y1 - y) * (w2 - w1) - (w1 - px) * (y2 - y1) > 0.0f) continue;
                if ((y2 - y) * (w3 - w2) - (w2 - px) * (y3 - y2) > 0.0f) continue;
                if ((y3 - y) * (w1 - w3) - (w3 - px) * (y1 - y3) > 0.0f) continue;
            }
            else
            {
                if ((y1 - y) * (w2 - w1) - (w1 - px) * (y2 - y1) < 0.0f) continue;
                if ((y2 - y) * (w3 - w2) - (w2 - px) * (y3 - y2) < 0.0f) continue;
                if ((y3 - y) * (w1 - w3) - (w3 - px) * (y1 - y3) < 0.0f) continue;
            }
        }

        if (surf->type == SURFACE_CAMERA_BOUNDARY)
            continue;
        
        return surf;
    }

    return NULL;
}

static struct Surface *spawn_find_wall_collisions(struct WallCollisionData *colData) {
    s16 cellX, cellZ;
    s16 x = colData->x;
    s16 z = colData->z;
    struct Surface *surf;

    // World (level) consists of a 16x16 grid. Find where the collision is on
    // the grid (round toward -inf)
    cellX = ((x + 0x2000) / 0x400) & 0x0F;
    cellZ = ((z + 0x2000) / 0x400) & 0x0F;

    surf = return_first_wall_collision(gDynamicSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next, colData);
    
    if (surf == NULL)
    {
        surf = return_first_wall_collision(gStaticSurfacePartition[cellZ][cellX][SPATIAL_PARTITION_WALLS].next, colData);
    }
    
    return surf;
}

void vec3s_resolve_wall_collisions(Vec3s pos, f32 radius) {
    Vec3f pos2;
    
    vec3s_to_vec3f(pos2, pos);
    f32_find_wall_collision(&pos2[0], &pos2[1], &pos2[2], 0.0f, radius);
    vec3f_to_vec3s(pos, pos2);
}

static void init_raycaster(void) {
    u32 ray;
    u32 angleBetweenRays = 0x10000 / WALL_CHECK_RAYCASTER_NUM_RAYS_TO_CAST;
    
    // Initialize gWallCheckNumRaySteps (gWallCheckRaycasterSearchDist is not a const so we have to do it programmatically)
    if (gWallCheckNumRaySteps == 0) {
        gWallCheckNumRaySteps = gWallCheckRaycasterSearchDist / WALL_CHECK_RAYCASTER_STEP_SIZE;
    }

    // Fill sRaycastDirections
    for (ray = 0; ray < WALL_CHECK_RAYCASTER_NUM_RAYS_TO_CAST; ray++) {
        sRaycastDirections[ray][0] = WALL_CHECK_RAYCASTER_STEP_SIZE * coss(ray * angleBetweenRays);
        sRaycastDirections[ray][1] = 0.0f;
        sRaycastDirections[ray][2] = WALL_CHECK_RAYCASTER_STEP_SIZE * sins(ray * angleBetweenRays);
    }
    
    sRaycasterInitialized = TRUE;
}

void update_raycaster_params(void) {
    if (gCurrLevelNum < 4 || sLevelParams[gCurrLevelNum - 4] == NULL) {
        return;
    }
    
    gWallCheckRaycasterSearchDist = (*sLevelParams[gCurrLevelNum - 4])[gCurrAreaIndex - 1].wallCheckRaycasterSearchDist;
    gWallCheckNumRaySteps = gWallCheckRaycasterSearchDist / WALL_CHECK_RAYCASTER_STEP_SIZE;
}

u8 is_safe_near_walls(Vec3s pos, u8 killOnOob) {
    /*
        1. cast WALL_CHECK_RAYCASTER_NUM_RAYS_TO_CAST rays out evenly from pos until they hit ceiling, oob, or a wall
        2. discard the ones that hit ceiling or oob
        3. for each ray, take the dot product of the ray/direction vector with the wall's normal
        4. if there is a positive dot product, return false
        5. return true
    */
    
    s8 i, j;
    
    f32 rayX;
    f32 rayY = pos[1] + 15;
    f32 rayZ;
    
    f32 directionX;
    f32 directionZ;
    
    struct Surface *lowFloor, *highFloor, *ceil;
    f32 floorHeight, ceilHeight, upperFloorHeight;
    
    struct WallCollisionData wallCollision;
    struct Surface *rayWall = NULL;
    
    f32 normX, normZ;
    
    // Initialize raycaster if necessary
    if (sRaycasterInitialized == FALSE)
        init_raycaster();
    
    wallCollision.x = pos[0],
    wallCollision.z = pos[2],
    
    // Rays only move in X or Z directions, since walls are vertical, so we dont need to update this
    wallCollision.y = rayY,
    
    // Thickness of the walls
    wallCollision.radius = 75.0f;
    
    // If the point is already in a wall, it is not safe
    if (spawn_find_wall_collisions(&wallCollision) != NULL)
        return FALSE;

    // Raycast
    for (i = -1; ++i < WALL_CHECK_RAYCASTER_NUM_RAYS_TO_CAST;) {
        // Create ray
        rayX = pos[0],
        rayZ = pos[2];
        
        // Create direction vector
        directionX = sRaycastDirections[i][0],
        directionZ = sRaycastDirections[i][2];
        
        // Check if ray is in a wall yet, if not, advance it until it hits the limit
        // If it ends up in a ceiling or OoB, disregard the ray
        // Then check the wall's normal to determine if the ray collided with the back of it
        for (j = -1; ++j < gWallCheckNumRaySteps;) {
            rayX += directionX;
            rayZ += directionZ;
            
            floorHeight = find_floor(rayX, rayY, rayZ, &lowFloor);
            
            // Check if the ray is in a ceiling
            ceilHeight = find_ceil(rayX, floorHeight + 80, rayZ, &ceil);
            if (ceilHeight < (rayY + 100)) {
                break;
            }
            
            // See if the ray passed under a floor
            upperFloorHeight = find_floor(rayX, ceilHeight - 80, rayZ, &highFloor);
            if (upperFloorHeight > rayY && (upperFloorHeight - rayY) < 750) {
                break;
            }
            
            // Check if the ray is in OoB
            if (lowFloor == NULL) {
                if (killOnOob) {
                    return FALSE;
                }
                
                break;
            }
            
            // Create collision parameters
            wallCollision.x = rayX,
            wallCollision.z = rayZ;
            
            // Check for walls
            if ((rayWall = spawn_find_wall_collisions(&wallCollision)) != NULL) 
                break;
        }
        
        // Check if the ray collided with the back of the wall
        if (rayWall == NULL)
            continue;
        
        normX = rayWall->normal.x,
        normZ = rayWall->normal.z;
        
        // Take dot product; positive means the ray is aligned with the wall normal,
        // and is therefore behind it.
        if (((rayX - pos[0]) * normX + (rayZ - pos[2]) * normZ) > 0)
            return FALSE;
    }
    
    return TRUE;
}
