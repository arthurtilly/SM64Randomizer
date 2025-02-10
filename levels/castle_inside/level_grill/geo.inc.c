const GeoLayout castle_geo_level_grill[] = {
   GEO_CULLING_RADIUS(300),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_ALPHA, inside_castle_seg7_dl_level_grill),
   GEO_CLOSE_NODE(),
   GEO_END(),
};

const GeoLayout castle_geo_level_grill_large[] = {
   GEO_CULLING_RADIUS(300),
   GEO_OPEN_NODE(),
      GEO_DISPLAY_LIST(LAYER_ALPHA, inside_castle_seg7_dl_level_grill_large),
   GEO_CLOSE_NODE(),
   GEO_END(),
};