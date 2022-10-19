#include "randomizer.h"

u8 gRequiredStars[STAR_REQ_MAX];

u8 sRemainingWarpsStatic[23] = {4,5,7,8,9,10,11,12,13,14,15,17,18,19,20,22,23,24,27,28,29,31,36};
u8 sRemainingWarpsTemp[23];
u8 sB1WarpsStatic[9] = { LEVEL_BOB, LEVEL_WF, LEVEL_CCM, LEVEL_JRB, LEVEL_PSS, LEVEL_BITDW, LEVEL_BBH, LEVEL_TOTWC, LEVEL_SA };
u8 sB1WarpsTemp[9];
u8 sB2WarpsStatic[7] = { LEVEL_SSL, LEVEL_LLL, LEVEL_HMC, LEVEL_COTMC, LEVEL_DDD, LEVEL_BITFS, LEVEL_VCUTM };
u8 sB2WarpsTemp[7];
u8 sB3WarpsStatic[7] = { LEVEL_WDW, LEVEL_TTM, LEVEL_SL, LEVEL_THI, LEVEL_RR, LEVEL_TTC, LEVEL_WMOTR };
u8 sB3WarpsTemp[7];
u8 gWarpDestinationsStatic[37] = {0,0,0,0,4,5,0,7,8,9,10,11,12,13,14,15,0,17,18,19,20,0,22,23,24,0,0,27,28,29,0,31,0,0,0,0,36};
u8 gWarpDestinations[37];

// Use bhvStub for avoidance points for all objects.
// Use bhv<OBJECT> for points that only those OBJECTs should avoid.
struct AvoidancePoint jrbAvoidancePoints[] = {
    {bhvBobombBuddyOpensCannon, {1300, 1946, 2206}, 700.0f, 50.0f, AVOIDANCE_SAFETY_MED} // Under the Mr. I vanilla spawn
};

struct AvoidancePoint ccmAvoidancePoints[] = {
    {bhvSmallPenguin, {-4963, -4750, 0}, 6000.0f, 600.0f, AVOIDANCE_SAFETY_HARD}, // Part 1 of WKWW Pengu fixes
    {bhvSmallPenguin, {-850, -3560, -3350}, 2500.0f, 1500.0f, AVOIDANCE_SAFETY_HARD}, // Part 2 of WKWW Pengu fixes

    {bhvStub, {-4230, -1250, 1813}, 200.0f, 200.0f, AVOIDANCE_SAFETY_ALL} // Snowman's body platform
};

struct AvoidancePoint bbhAvoidancePoints[] = {
    {bhvStub, {0, 0, 0}, 100.0f, 100.0f, AVOIDANCE_SAFETY_ALL} // In the BBH Pillar in the main room
};

struct AvoidancePoint lllAvoidancePoints[] = {
    {bhvStub, {-3200, 10, 3456}, 100.0f, 50.0f, AVOIDANCE_SAFETY_ALL} // Under the Mr. I vanilla spawn
};

struct AvoidancePoint sslPyrAvoidancePoints[] = {
    {bhvStub, {0, 4962, 256}, 600.0f, 50.0f, AVOIDANCE_SAFETY_ALL},    // In Elevator
    {bhvStub, {0, 300, -1200}, 1000.0f, 1000.0f, AVOIDANCE_SAFETY_ALL} // Past Eyerok Loading Zone
};

struct AvoidancePoint dddAvoidancePoints[] = {
    {bhvTreasureChestBottom, {-3177, -4600, 100}, 650.0f, 150.0f, AVOIDANCE_SAFETY_ALL} // Under Whirlpool
};

struct AvoidancePoint wdwAvoidancePoints[] = {
    {bhvStub, {4558, 0, -3789}, 1000.0f, 150.0f, AVOIDANCE_SAFETY_HARD} // In cage
};

struct AvoidancePoint wdwTownAvoidancePoints[] = {
    {bhvExclamationBox, {2254, -1692, 3790}, 1100.0f, 1000.0f, AVOIDANCE_SAFETY_ALL}, // Corner of WDW Town
    {bhvStub,      {2254, -2559, 894}, 1200.0f, 50.0f, AVOIDANCE_SAFETY_ALL}, // In a building
};

struct AvoidancePoint rrAvoidancePoints[] = {
    {bhvBobombBuddyOpensCannon, {5020, 3819, 4050}, 1500.0f, 50.0f, AVOIDANCE_SAFETY_ALL} // Cannon Only Platform
};

struct AvoidancePoint wmotrAvoidancePoints[] = {
    {bhvBobombBuddyOpensCannon, {-3000, 4537, -4000}, 1500.0f, 100.0f, AVOIDANCE_SAFETY_ALL} // Highest Platform
};

struct AvoidancePoint bitfsAvoidancePoints[] = {
    {bhvStub, {6772, 2900, 106}, 500.0f, 50.0f, AVOIDANCE_SAFETY_ALL}, // In Bowser Warp
    {bhvStub, {4440, -665, 280}, 700.0f, 50.0f, AVOIDANCE_SAFETY_HARD}, // Above Cage
    {bhvStub, {6772, 4000, 106}, 800.0f, 500.0f, AVOIDANCE_SAFETY_MED}, // Above Bowser Warp
};

struct AvoidancePoint icBasAvoidancePoints[] = {
    {bhvStub, {6000, -1074, 2000}, 1000.0f, 50.0f, AVOIDANCE_SAFETY_ALL}, // Behind DDD Painting
    {bhvStub, {-1023, -1074, 589}, 400.0f, 50.0f, AVOIDANCE_SAFETY_ALL}   // Behind First Key Door
};

struct AvoidancePoint icUpAvoidancePoints[] = {
    {bhvStub, {-1963, 2240, 4815}, 1000.0f, 100.0f, AVOIDANCE_SAFETY_ALL}, // RR side under first stair
    {bhvStub, {1250, 2240, 4815}, 1000.0f, 100.0f, AVOIDANCE_SAFETY_ALL}, // WMotR side under first stair

    {bhvStub, {2975, 2430, 5884}, 300.0f, 200.0f, AVOIDANCE_SAFETY_ALL}, // WMotR entrance

    {bhvStub, {3135, 1613, 3141}, 300.0f, 100.0f, AVOIDANCE_SAFETY_ALL}, // SL Pillar
    {bhvStub, {3793, 1613, 2754}, 300.0f, 100.0f, AVOIDANCE_SAFETY_ALL}, // SL Pillar
    {bhvStub, {3230, 1613, 1328}, 300.0f, 100.0f, AVOIDANCE_SAFETY_ALL}, // SL Pillar
    {bhvStub, {3249, 1613, 294}, 300.0f, 100.0f, AVOIDANCE_SAFETY_ALL}, // SL Pillar
    {bhvStub, {3942, 1613, 709}, 300.0f, 100.0f, AVOIDANCE_SAFETY_ALL}, // SL Pillar

    {bhvStub, {-236, 4265, -87}, 3826.0f, 2130.0f, AVOIDANCE_SAFETY_ALL}, // Endless staircase area
};

#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))

// Main courses
struct AreaParams bobParams[] = {
    {-8192, 8192, 0, 4500, -8192, 8192, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // BoB
};
struct AreaParams wfParams[] = {
    {-3300, 5100, 256, 6500, -4100, 5700, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // WF
};
struct AreaParams jrbParams[] = {
    {-7800, 8000, -5200, 3000, -7500, 8000, 0, 1000, ARRAY_SIZE(jrbAvoidancePoints), &jrbAvoidancePoints}, // JRB main area
    {-1000, 1200, -300, 1500, -3000, 3500, AREA_PARAM_FLAG_CHANGING_WATER_LEVEL, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // JRB sunken ship
};
struct AreaParams ccmParams[] = {
    {-6500, 6500, -4600, 4600, -5500, 6500, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, ARRAY_SIZE(ccmAvoidancePoints), &ccmAvoidancePoints}, // CCM main area
    {-7500, 7500, -5800, 7400, -7000, 7000, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // CCM slide
};
struct AreaParams bbhParams[] = {
    {-4200, 5500, -3000, 3500, -4000, 6700, 0, 800, ARRAY_SIZE(bbhAvoidancePoints), &bbhAvoidancePoints} // BBH
};
struct AreaParams hmcParams[] = {
    {-7800, 7600, -6300, 3000, -8192, 8192, 0, 800, 0, NULL} // HMC
};
struct AreaParams lllParams[] = {
    {-8192, 8192, 0, 1600, -8192, 8192, 0, 300, ARRAY_SIZE(lllAvoidancePoints), &lllAvoidancePoints}, // LLL main area
    {-3000, 3000, 0, 5000, -3000, 3000, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // LLL volcano
};
struct AreaParams sslParams[] = {
    {-8192, 8192, -250, 2000, -8192, 8192, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL}, // SSL main area
    {-4000, 4000, -250, 5500, -4200, 6600, 0, 700, ARRAY_SIZE(sslPyrAvoidancePoints), &sslPyrAvoidancePoints}, // SSL pyramid
    {-1000, 1000, -1500, -500, -4000, -1800, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // SSL eyerok
};
struct AreaParams dddParams[] = {
    {-6500, 2000, -5200, 1000, -3500, 3500, 0, 1000, ARRAY_SIZE(dddAvoidancePoints), &dddAvoidancePoints}, // DDD starting area
    {-1500, 7000, -4000, 1600, -4000, 6000, 0, 1000, 0, NULL} // DDD sub area
};
struct AreaParams slParams[] = {
    {-7891, 6868, 800, 5500, -7891, 7892, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL}, // SL main area
    {-2000, 2000, 0, 650, -2000, 2500, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // SL igloo
};
struct AreaParams wdwParams[] = {
    {-3800, 4500, 0, 5000, -3800, 4500, AREA_PARAM_FLAG_CHANGING_WATER_LEVEL, 800, ARRAY_SIZE(wdwAvoidancePoints), &wdwAvoidancePoints}, // WDW starting area
    {-3800, 2300, -2500, 250, -2300, 3800, AREA_PARAM_FLAG_CHANGING_WATER_LEVEL, 1200, ARRAY_SIZE(wdwTownAvoidancePoints), &wdwTownAvoidancePoints} // WDW downtown
};
struct AreaParams ttmParams[] = {
    {-4100, 5300, -4500, 3200, -5300, 6700, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL}, // TTM main area
    {-2600, 8192, -200, 8192, -2900, 8192, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL}, // TTM slide first section
    {-8192, 8192, -7000, 6300, -8192, 8192, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL}, // TTM slide second section
    {-8192, 8192, -8192, 5500, -8192, 8192, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // TTM slide third section
};
struct AreaParams thiParams[] = {
    {-8192, 8192, -3400, 5000, -8192, 8192, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL}, // THI huge island
    {-2500, 2500, -1100, 2000, -5000, 2500, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL}, // THI small island
    {-2000, 2000, 500, 2500, -2000, 2000, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // THI wiggler's cave
};
struct AreaParams ttcParams[] = {
    {-2000, 3000, -5200, 7500, -2000, 3000, 0, 500, 0, NULL} // TTC
};
struct AreaParams rrParams[] = {
    {-8000, 7500, -4600, 7200, -7000, 7500, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, ARRAY_SIZE(rrAvoidancePoints), &rrAvoidancePoints} // RR
};

// Secret courses
struct AreaParams pssParams[] = {
    {-7700, 6000, -4500, 7500, -7200, 6600, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // PSS
};
struct AreaParams saParams[] = {
    {-2750, 2750, -4250, -250, -2750, 2750, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // SA
};
struct AreaParams wmotrParams[] = {
    {-4200, 4600, -2750, 5500, -5200, 6000, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, ARRAY_SIZE(wmotrAvoidancePoints), &wmotrAvoidancePoints} // WMotR
};
struct AreaParams totwcParams[] = {
    {-1500, 1500, -2100, -500, -1500, 1500, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // TotWC
};
struct AreaParams cotmcParams[] = {
    {-5000, 1400, -600, 1500, -7500, 2000, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // CotMC
};
struct AreaParams vcutmParams[] = {
    {-6500, 5000, -3300, 6500, -6500, 1700, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // VCutM
};
struct AreaParams bitdwParams[] = {
    {-8000, 7500, -3400, 3800, -3000, 4400, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // BitDW
};
struct AreaParams bitfsParams[] = {
    {-8000, 8000, -3000, 6000, -2500, 1500, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, ARRAY_SIZE(bitfsAvoidancePoints), &bitfsAvoidancePoints} // BitFS
};
struct AreaParams bitsParams[] = {
    {-7500, 7600, -5000, 7000, -7000, 500, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // BitS
};

// Castle areas
struct AreaParams cgParams[] = {
    {-8192, 8192, -500, 7500, -7000, 7000, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // Castle grounds
};
struct AreaParams ccParams[] = {
    {-3700, 3700, -200, 500, -3800, 500, 0, WALL_CHECK_RAYCASTER_DEFAULT_SEARCH_DIST, 0, NULL} // Castle courtyard
};
struct AreaParams icParams[] = {
    {-7500, 4300, -1000, 600, -5000, 2200, 0, 1000, 0, NULL}, // Castle foyer
    {-7300, 4300, 500, 5300, -3700, 7000, 0, 2000, ARRAY_SIZE(icUpAvoidancePoints), &icUpAvoidancePoints}, // Castle upstairs
    {-4200, 7800, -2500, -500, -4000, 3000, 0, 1000, ARRAY_SIZE(icBasAvoidancePoints), &icBasAvoidancePoints} // Castle basement
};

AreaParamsArray *sLevelParams[] = {
    &bbhParams,
    &ccmParams,
    &icParams,
    &hmcParams,
    &sslParams,
    &bobParams,
    &slParams,
    &wdwParams,
    &jrbParams,
    &thiParams,
    &ttcParams,
    &rrParams,
    &cgParams,
    &bitdwParams,
    &vcutmParams,
    &bitfsParams,
    &saParams,
    &bitsParams,
    &lllParams,
    &dddParams,
    &wfParams,
    NULL,
    &ccParams,
    &pssParams,
    &cotmcParams,
    &totwcParams,
    NULL,
    &wmotrParams,
    NULL,
    NULL,
    NULL,
    NULL,
    &ttmParams
};
