#include "stats.h"

// Explicit instantiation for the hardware display class.
// Add more instantiations here if you port to another simulator / wrapper.
#include <GxEPD2_GFX.h>
template void drawStatsFace<GxEPD2_GFX>(GxEPD2_GFX&, int, int, const StatsData&);
