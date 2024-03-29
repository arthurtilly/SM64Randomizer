#include "game/geo_misc.h"

// 0x16000EA0
const GeoLayout star_geo[] = {
   GEO_SHADOW(SHADOW_CIRCLE_4_VERTS, 0x9B, 100),
   GEO_OPEN_NODE(),
      GEO_SCALE(0x00, 16384),
      GEO_OPEN_NODE(),
         GEO_ASM(LAYER_OCCLUDE_SILHOUETTE_OPAQUE, geo_set_star_color),
         GEO_DISPLAY_LIST(LAYER_OCCLUDE_SILHOUETTE_OPAQUE, star_seg3_dl_body),
         GEO_DISPLAY_LIST(LAYER_OCCLUDE_SILHOUETTE_ALPHA, star_seg3_dl_eyes),
      GEO_CLOSE_NODE(),
   GEO_CLOSE_NODE(),
   GEO_END(),
};
