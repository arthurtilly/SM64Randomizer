#ifndef SURFACE_TERRAINS_H
#define SURFACE_TERRAINS_H

#include "config.h"

// Surface Types
enum SurfaceTypes {
    SURFACE_NULL = -1,
    SURFACE_DEFAULT,                  // Environment default
    SURFACE_0004,                     // Unused, has no function and has parameters
    SURFACE_VERY_SLIPPERY,            // Very slippery, mostly used for slides
    SURFACE_SLIPPERY,                 // Slippery
    SURFACE_NOT_SLIPPERY,             // Non-slippery, climbable
    SURFACE_HARD,                     // Hard floor (Always has fall damage)
    SURFACE_HARD_SLIPPERY,            // Hard and slippery (Always has fall damage)
    SURFACE_HARD_VERY_SLIPPERY,       // Hard and very slippery (Always has fall damage)
    SURFACE_HARD_NOT_SLIPPERY,        // Hard and Non-slippery (Always has fall damage)
    SURFACE_ICE,                      // Slippery Ice, in snow levels and THI's water floor
    SURFACE_HORIZONTAL_WIND,          // Horizontal wind, has parameters
    SURFACE_FLOWING_WATER,            // Water (flowing), has parameters
    SURFACE_MGR_MUSIC,                // Plays the Merry go round music, see handle_merry_go_round_music in bbh_merry_go_round.inc.c for more details
    SURFACE_NOISE_DEFAULT,            // Default floor with noise
    SURFACE_NOISE_SLIPPERY,           // Slippery floor with noise
    SURFACE_NOISE_VERY_SLIPPERY,      // Very slippery floor with noise, used in CCM
    SURFACE_NOISE_VERY_SLIPPERY_73,   // Very slippery floor with noise, unused
    SURFACE_NOISE_VERY_SLIPPERY_74,   // Very slippery floor with noise, unused
    SURFACE_CLOSE_CAMERA,             // Close camera
    SURFACE_WATER,                    // Water, has no action, used on some waterboxes below
    SURFACE_SHALLOW_QUICKSAND,        // Shallow Quicksand (depth of 10 units)
    SURFACE_LOOK_UP_WARP,             // Look up and warp (Wing cap entrance)
    SURFACE_TIMER_START,              // Timer start (Peach's secret slide)
    SURFACE_TIMER_END,                // Timer stop (Peach's secret slide)
    SURFACE_BOSS_FIGHT_CAMERA,        // Wide camera for BOB and WF bosses
    SURFACE_CAMERA_FREE_ROAM,         // Free roam camera for THI and TTC
    SURFACE_THI3_WALLKICK,            // Surface where there's a wall kick section in THI 3rd area, has no action defined
    SURFACE_CAMERA_8_DIR,             // Surface that enables far camera for platforms, used in THI
    SURFACE_CAMERA_MIDDLE,            // Surface camera that returns to the middle, used on the 4 pillars of SSL
    SURFACE_CAMERA_ROTATE_RIGHT,      // Surface camera that rotates to the right (Bowser 1 & THI)
    SURFACE_CAMERA_ROTATE_LEFT,       // Surface camera that rotates to the left (BOB & TTM)
    SURFACE_NO_CAM_COLLISION,         // Surface with no cam collision flag
    SURFACE_NO_CAM_COLLISION_77,      // Surface with no cam collision flag, unused
    SURFACE_NO_CAM_COL_VERY_SLIPPERY, // Surface with no cam collision flag, very slippery with noise (THI)
    SURFACE_NO_CAM_COL_SLIPPERY,      // Surface with no cam collision flag, slippery with noise (CCM, PSS and TTM slides)
    SURFACE_TTM_VINES,                // TTM vines, has no action defined
    SURFACE_SWITCH,                   // Surface with no cam collision flag, non-slippery with noise, used by switches and Dorrie
    SURFACE_VANISH_CAP_WALLS,         // Vanish cap walls, pass through them with Vanish Cap
    SURFACE_WALL_MISC,                // Used for some walls, Cannon to adjust the camera, and some objects like Warp Pipe
    SURFACE_HANGABLE,                 // Ceiling that Mario can climb on
    SURFACE_SLOW,                     // Slow down Mario, unused

    SURFACE_SAFE_FLOORS_GENERAL,

    SURFACE_BURNING,                  // Lava / Frostbite (in SL), but is used mostly for Lava
    SURFACE_DEEP_QUICKSAND,           // Quicksand (lethal, slow, depth of 160 units)
    SURFACE_INSTANT_QUICKSAND,        // Quicksand (lethal, instant)
    SURFACE_DEEP_MOVING_QUICKSAND,    // Moving quicksand (flowing, depth of 160 units)
    SURFACE_SHALLOW_MOVING_QUICKSAND, // Moving quicksand (flowing, depth of 25 units)
    SURFACE_QUICKSAND,                // Moving quicksand (60 units)
    SURFACE_MOVING_QUICKSAND,         // Moving quicksand (flowing, depth of 60 units)
    SURFACE_INSTANT_MOVING_QUICKSAND, // Quicksand (lethal, flowing)

    SURFACE_SAFE_FLOORS_HARD,

    SURFACE_DEATH_PLANE,              // Death floor
    SURFACE_INTANGIBLE,               // Intangible (Separates BBH mansion from merry-go-round, for room usage)
    SURFACE_INSTANT_WARP_1B,          // Instant warp to another area, used to warp between areas in WDW and the endless stairs to warp back
    SURFACE_INSTANT_WARP_1C,          // Instant warp to another area, used to warp between areas in WDW
    SURFACE_INSTANT_WARP_1D,          // Instant warp to another area, used to warp between areas in DDD, SSL and TTM
    SURFACE_INSTANT_WARP_1E,          // Instant warp to another area, used to warp between areas in DDD, SSL and TTM
    SURFACE_WARP,                     // Surface warp
    SURFACE_VERTICAL_WIND,            // Death at bottom with vertical wind
    SURFACE_CAMERA_BOUNDARY,          // Intangible Area, only used to restrict camera movement
    SURFACE_NEW_WATER,
    SURFACE_NEW_WATER_BOTTOM,

    SURFACE_PAINTING_WOBBLE_A6 = 0xA6,         // 0x00A6 // Painting wobble (BoB Left)
    SURFACE_PAINTING_WOBBLE_A7,         // 0x00A7 // Painting wobble (BoB Middle)
    SURFACE_PAINTING_WOBBLE_A8,         // 0x00A8 // Painting wobble (BoB Right)
    SURFACE_PAINTING_WOBBLE_A9,         // 0x00A9 // Painting wobble (CCM Left)
    SURFACE_PAINTING_WOBBLE_AA,         // 0x00AA // Painting wobble (CCM Middle)
    SURFACE_PAINTING_WOBBLE_AB,         // 0x00AB // Painting wobble (CCM Right)
    SURFACE_PAINTING_WOBBLE_AC,         // 0x00AC // Painting wobble (WF Left)
    SURFACE_PAINTING_WOBBLE_AD,         // 0x00AD // Painting wobble (WF Middle)
    SURFACE_PAINTING_WOBBLE_AE,         // 0x00AE // Painting wobble (WF Right)
    SURFACE_PAINTING_WOBBLE_AF,         // 0x00AF // Painting wobble (JRB Left)
    SURFACE_PAINTING_WOBBLE_B0,         // 0x00B0 // Painting wobble (JRB Middle)
    SURFACE_PAINTING_WOBBLE_B1,         // 0x00B1 // Painting wobble (JRB Right)
    SURFACE_PAINTING_WOBBLE_B2,         // 0x00B2 // Painting wobble (LLL Left)
    SURFACE_PAINTING_WOBBLE_B3,         // 0x00B3 // Painting wobble (LLL Middle)
    SURFACE_PAINTING_WOBBLE_B4,         // 0x00B4 // Painting wobble (LLL Right)
    SURFACE_PAINTING_WOBBLE_B5,         // 0x00B5 // Painting wobble (SSL Left)
    SURFACE_PAINTING_WOBBLE_B6,         // 0x00B6 // Painting wobble (SSL Middle)
    SURFACE_PAINTING_WOBBLE_B7,         // 0x00B7 // Painting wobble (SSL Right)
    SURFACE_PAINTING_WOBBLE_B8,         // 0x00B8 // Painting wobble (Unused - Left)
    SURFACE_PAINTING_WOBBLE_B9,         // 0x00B9 // Painting wobble (Unused - Middle)
    SURFACE_PAINTING_WOBBLE_BA,         // 0x00BA // Painting wobble (Unused - Right)
    SURFACE_PAINTING_WOBBLE_BB,         // 0x00BB // Painting wobble (DDD - Left), makes the painting wobble if touched
    SURFACE_PAINTING_WOBBLE_BC,         // 0x00BC // Painting wobble (Unused, DDD - Middle)
    SURFACE_PAINTING_WOBBLE_BD,         // 0x00BD // Painting wobble (Unused, DDD - Right)
    SURFACE_PAINTING_WOBBLE_BE,         // 0x00BE // Painting wobble (WDW Left)
    SURFACE_PAINTING_WOBBLE_BF,         // 0x00BF // Painting wobble (WDW Middle)
    SURFACE_PAINTING_WOBBLE_C0,         // 0x00C0 // Painting wobble (WDW Right)
    SURFACE_PAINTING_WOBBLE_C1,         // 0x00C1 // Painting wobble (THI Tiny - Left)
    SURFACE_PAINTING_WOBBLE_C2,         // 0x00C2 // Painting wobble (THI Tiny - Middle)
    SURFACE_PAINTING_WOBBLE_C3,         // 0x00C3 // Painting wobble (THI Tiny - Right)
    SURFACE_PAINTING_WOBBLE_C4,         // 0x00C4 // Painting wobble (TTM Left)
    SURFACE_PAINTING_WOBBLE_C5,         // 0x00C5 // Painting wobble (TTM Middle)
    SURFACE_PAINTING_WOBBLE_C6,         // 0x00C6 // Painting wobble (TTM Right)
    SURFACE_PAINTING_WOBBLE_C7,         // 0x00C7 // Painting wobble (Unused, TTC - Left)
    SURFACE_PAINTING_WOBBLE_C8,         // 0x00C8 // Painting wobble (Unused, TTC - Middle)
    SURFACE_PAINTING_WOBBLE_C9,         // 0x00C9 // Painting wobble (Unused, TTC - Right)
    SURFACE_PAINTING_WOBBLE_CA,         // 0x00CA // Painting wobble (Unused, SL - Left)
    SURFACE_PAINTING_WOBBLE_CB,         // 0x00CB // Painting wobble (Unused, SL - Middle)
    SURFACE_PAINTING_WOBBLE_CC,         // 0x00CC // Painting wobble (Unused, SL - Right)
    SURFACE_PAINTING_WOBBLE_CD,         // 0x00CD // Painting wobble (THI Huge - Left)
    SURFACE_PAINTING_WOBBLE_CE,         // 0x00CE // Painting wobble (THI Huge - Middle)
    SURFACE_PAINTING_WOBBLE_CF,         // 0x00CF // Painting wobble (THI Huge - Right)
    SURFACE_PAINTING_WOBBLE_D0,         // 0x00D0 // Painting wobble (HMC & COTMC - Left), makes the painting wobble if touched
    SURFACE_PAINTING_WOBBLE_D1,         // 0x00D1 // Painting wobble (Unused, HMC & COTMC - Middle)
    SURFACE_PAINTING_WOBBLE_D2,         // 0x00D2 // Painting wobble (Unused, HMC & COTMC - Right)
    SURFACE_PAINTING_WARP_D3,           // 0x00D3 // Painting warp (BoB Left)
    SURFACE_PAINTING_WARP_D4,           // 0x00D4 // Painting warp (BoB Middle)
    SURFACE_PAINTING_WARP_D5,           // 0x00D5 // Painting warp (BoB Right)
    SURFACE_PAINTING_WARP_D6,           // 0x00D6 // Painting warp (CCM Left)
    SURFACE_PAINTING_WARP_D7,           // 0x00D7 // Painting warp (CCM Middle)
    SURFACE_PAINTING_WARP_D8,           // 0x00D8 // Painting warp (CCM Right)
    SURFACE_PAINTING_WARP_D9,           // 0x00D9 // Painting warp (WF Left)
    SURFACE_PAINTING_WARP_DA,           // 0x00DA // Painting warp (WF Middle)
    SURFACE_PAINTING_WARP_DB,           // 0x00DB // Painting warp (WF Right)
    SURFACE_PAINTING_WARP_DC,           // 0x00DC // Painting warp (JRB Left)
    SURFACE_PAINTING_WARP_DD,           // 0x00DD // Painting warp (JRB Middle)
    SURFACE_PAINTING_WARP_DE,           // 0x00DE // Painting warp (JRB Right)
    SURFACE_PAINTING_WARP_DF,           // 0x00DF // Painting warp (LLL Left)
    SURFACE_PAINTING_WARP_E0,           // 0x00E0 // Painting warp (LLL Middle)
    SURFACE_PAINTING_WARP_E1,           // 0x00E1 // Painting warp (LLL Right)
    SURFACE_PAINTING_WARP_E2,           // 0x00E2 // Painting warp (SSL Left)
    SURFACE_PAINTING_WARP_E3,           // 0x00E3 // Painting warp (SSL Medium)
    SURFACE_PAINTING_WARP_E4,           // 0x00E4 // Painting warp (SSL Right)
    SURFACE_PAINTING_WARP_E5,           // 0x00E5 // Painting warp (Unused - Left)
    SURFACE_PAINTING_WARP_E6,           // 0x00E6 // Painting warp (Unused - Medium)
    SURFACE_PAINTING_WARP_E7,           // 0x00E7 // Painting warp (Unused - Right)
    SURFACE_PAINTING_WARP_E8,           // 0x00E8 // Painting warp (DDD - Left)
    SURFACE_PAINTING_WARP_E9,           // 0x00E9 // Painting warp (DDD - Middle)
    SURFACE_PAINTING_WARP_EA,           // 0x00EA // Painting warp (DDD - Right)
    SURFACE_PAINTING_WARP_EB,           // 0x00EB // Painting warp (WDW Left)
    SURFACE_PAINTING_WARP_EC,           // 0x00EC // Painting warp (WDW Middle)
    SURFACE_PAINTING_WARP_ED,           // 0x00ED // Painting warp (WDW Right)
    SURFACE_PAINTING_WARP_EE,           // 0x00EE // Painting warp (THI Tiny - Left)
    SURFACE_PAINTING_WARP_EF,           // 0x00EF // Painting warp (THI Tiny - Middle)
    SURFACE_PAINTING_WARP_F0,           // 0x00F0 // Painting warp (THI Tiny - Right)
    SURFACE_PAINTING_WARP_F1,           // 0x00F1 // Painting warp (TTM Left)
    SURFACE_PAINTING_WARP_F2,           // 0x00F2 // Painting warp (TTM Middle)
    SURFACE_PAINTING_WARP_F3,           // 0x00F3 // Painting warp (TTM Right)
    SURFACE_TTC_PAINTING_1,             // 0x00F4 // Painting warp (TTC Left)
    SURFACE_TTC_PAINTING_2,             // 0x00F5 // Painting warp (TTC Medium)
    SURFACE_TTC_PAINTING_3,             // 0x00F6 // Painting warp (TTC Right)
    SURFACE_PAINTING_WARP_F7,           // 0x00F7 // Painting warp (SL Left)
    SURFACE_PAINTING_WARP_F8,           // 0x00F8 // Painting warp (SL Middle)
    SURFACE_PAINTING_WARP_F9,           // 0x00F9 // Painting warp (SL Right)
    SURFACE_PAINTING_WARP_FA,           // 0x00FA // Painting warp (THI Huge - Left)
    SURFACE_PAINTING_WARP_FB,           // 0x00FB // Painting warp (THI Huge - Middle)
    SURFACE_PAINTING_WARP_FC,           // 0x00FC // Painting warp (THI Huge - Right)
    SURFACE_WOBBLING_WARP,              // 0x00FD // Pool warp (HMC & DDD)
    SURFACE_00FE,                       // 0x00FE // Unused
    SURFACE_TRAPDOOR,                   // 0x00FF // Bowser Left trapdoor, has no action defined
};

#define SURFACE_IS_NEW_WATER(cmd)               (((cmd) == SURFACE_NEW_WATER) || ((cmd) == SURFACE_NEW_WATER_BOTTOM))

#define SURFACE_IS_QUICKSAND(cmd)  ((cmd) == SURFACE_SHALLOW_QUICKSAND || \
                                    (cmd) == SURFACE_INSTANT_QUICKSAND || \
                                    (cmd) == SURFACE_DEEP_MOVING_QUICKSAND || \
                                    (cmd) == SURFACE_SHALLOW_MOVING_QUICKSAND || \
                                    (cmd) == SURFACE_QUICKSAND || \
                                    (cmd) == SURFACE_MOVING_QUICKSAND || \
                                    (cmd) == SURFACE_INSTANT_MOVING_QUICKSAND)

#define SURFACE_IS_NOT_HARD(cmd)   ((cmd) != SURFACE_HARD && \
                                    (cmd) != SURFACE_HARD_VERY_SLIPPERY && \
                                    (cmd) != SURFACE_HARD_SLIPPERY && \
                                    (cmd) != SURFACE_HARD_NOT_SLIPPERY)

#define SURFACE_IS_PAINTING_WOBBLE(cmd)         (((cmd) >= SURFACE_PAINTING_WOBBLE_A6) && ((cmd) <= SURFACE_PAINTING_WOBBLE_D2))
#define SURFACE_IS_PAINTING_WOBBLE_LEFT(cmd)    ((((cmd) - SURFACE_PAINTING_WOBBLE_A6) % 3) == 0)
#define SURFACE_IS_PAINTING_WOBBLE_MIDDLE(cmd)  ((((cmd) - SURFACE_PAINTING_WOBBLE_A7) % 3) == 0)
#define SURFACE_IS_PAINTING_WOBBLE_RIGHT(cmd)   ((((cmd) - SURFACE_PAINTING_WOBBLE_A8) % 3) == 0)
#define SURFACE_IS_PAINTING_WARP(cmd)           (((cmd) >= SURFACE_PAINTING_WARP_D3) && ((cmd) < SURFACE_WOBBLING_WARP)) // skips SURFACE_WOBBLING_WARP
#define SURFACE_IS_PAINTING_WARP_LEFT(cmd)      ((((cmd) - SURFACE_PAINTING_WARP_D3  ) % 3) == 0)
#define SURFACE_IS_PAINTING_WARP_MIDDLE(cmd)    ((((cmd) - SURFACE_PAINTING_WARP_D4  ) % 3) == 0)
#define SURFACE_IS_PAINTING_WARP_RIGHT(cmd)     ((((cmd) - SURFACE_PAINTING_WARP_D5  ) % 3) == 0)
#define SURFACE_IS_INSTANT_WARP(cmd)            (((cmd) >= SURFACE_INSTANT_WARP_1B) && ((cmd) <= SURFACE_INSTANT_WARP_1E))
#define SURFACE_IS_WARP(cmd)                    (((cmd) == SURFACE_LOOK_UP_WARP) || ((cmd) == SURFACE_WOBBLING_WARP) || SURFACE_IS_PAINTING_WARP(cmd) || SURFACE_IS_INSTANT_WARP(cmd))
#define SURFACE_IS_UNSAFE(cmd)                  (((cmd) == SURFACE_BURNING) || SURFACE_IS_QUICKSAND(cmd) || SURFACE_IS_WARP(cmd))

enum SurfaceClass {
    SURFACE_CLASS_DEFAULT,
    SURFACE_CLASS_VERY_SLIPPERY,
    SURFACE_CLASS_SLIPPERY,
    SURFACE_CLASS_NOT_SLIPPERY
};

enum SurfaceFlags {
    SURFACE_FLAGS_NONE            = (0 << 0), // 0x0000
    SURFACE_FLAG_DYNAMIC          = (1 << 0), // 0x0001
    SURFACE_FLAG_NO_CAM_COLLISION = (1 << 1), // 0x0002
    SURFACE_FLAG_X_PROJECTION     = (1 << 2), // 0x0004
};

// These are effectively unique "surface" types like those defined higher
// And they are used as collision commands to load certain functions
enum TerrainLoadCmd {
    TERRAIN_LOAD_VERTICES = 0x40, // Begins vertices list for collision triangles
    TERRAIN_LOAD_CONTINUE,        // Stop loading vertices but continues to load other collision commands
    TERRAIN_LOAD_END,             // End the collision list
    TERRAIN_LOAD_OBJECTS,         // Loads in certain objects for level start
    TERRAIN_LOAD_ENVIRONMENT      // Loads water/HMC gas
};

#define TERRAIN_LOAD_IS_SURFACE_TYPE_LOW(cmd)  (cmd <  0x40)
#define TERRAIN_LOAD_IS_SURFACE_TYPE_HIGH(cmd) (cmd >= 0x65)

// Terrain types defined by the level script command terrain_type (cmd_31)
enum TerrainType {
    TERRAIN_GRASS,
    TERRAIN_STONE,
    TERRAIN_SNOW,
    TERRAIN_SAND,
    TERRAIN_SPOOKY,
    TERRAIN_WATER,
    TERRAIN_SLIDE,
    TERRAIN_MASK = 0x7
};

// These collision commands are unique "surface" types like those defined higher

// Collision Data Routine Initiate
#define COL_INIT() TERRAIN_LOAD_VERTICES

// Collision Vertices Read Initiate
#define COL_VERTEX_INIT(vtxNum) vtxNum

// Collision Vertex
#define COL_VERTEX(x, y, z) x, y, z

// Collision Tris Initiate
#define COL_TRI_INIT(surfType, triNum) surfType, triNum

// Collision Tri
#ifdef ALL_SURFACES_HAVE_FORCE
#define COL_TRI(v1, v2, v3) v1, v2, v3, 0
#else
#define COL_TRI(v1, v2, v3) v1, v2, v3
#endif

// Collision Tri With Special Params
#define COL_TRI_SPECIAL(v1, v2, v3, param) v1, v2, v3, param

// Collision Tris Stop Loading
#define COL_TRI_STOP() TERRAIN_LOAD_CONTINUE

// End Collision Data
#define COL_END() TERRAIN_LOAD_END

// Special Object Initiate
#define COL_SPECIAL_INIT(num) TERRAIN_LOAD_OBJECTS, num

// Water Boxes Initiate
#define COL_WATER_BOX_INIT(num) TERRAIN_LOAD_ENVIRONMENT, num

// Water Box
#define COL_WATER_BOX(id, x1, z1, x2, z2, y) id, x1, z1, x2, z2, y

#endif // SURFACE_TERRAINS_H
