#include "audio.h"

const note_t boom_song[] = {

  /* INTRO I */
  /* I A */
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),

  NOTE(rest, r1), /* 7 */

  /* I A */
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8), /* 6 */

  /* I B */
  NOTE(fis3, r1_16), NOTE(rest, r1_16),
  NOTE(fis3, r1_8),
  NOTE(cis3, r1_16), NOTE(rest, r1_16),
  NOTE(cis3, r1_8),
  NOTE(ais3, r1_16), NOTE(rest, r1_16),
  NOTE(ais3, r1_8),
  NOTE(fis3, r1_4), /* 10 */

  /* I A */
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),

  NOTE(rest, r1), /* 7 */

  /* I A */
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),

  NOTE(rest, r1), /* 7 */

  /* INTRO II */
  /* II A */
  NOTE(rest, r1_8 ),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_4 ),
  NOTE(gis4, r1_8 ),
  NOTE(fis4, r1_16), NOTE(rest, r1_16),
  NOTE(gis4, r1_8 ),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_4 ), NOTE(rest, r1_2 ), /* 16 */

  /* II B */
  NOTE(rest, r1_8 ),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_4 ),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(cis5, r1_4 ),
  NOTE(ais4, r1_4 ),
  NOTE(gis4, r1_4 ),
  NOTE(fis4, r1_4 ), /* 16 */

  /* II A */
  NOTE(rest, r1_8 ),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_4 ),
  NOTE(gis4, r1_8 ),
  NOTE(fis4, r1_16), NOTE(rest, r1_16),
  NOTE(gis4, r1_8 ),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_4 ), NOTE(rest, r1_2 ), /* 16 */

  /* II B */
  NOTE(rest, r1_8 ),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(gis4, r1_8 ),
  NOTE(ais4, r1_8 ),
  NOTE(gis4, r1_8 ),
  NOTE(fis4, r1_8 ), NOTE(rest, r1_4 ),

  NOTE(rest, r1_2), /* 13 */

  /* INTRO III */
  /* III A */
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8), /* 6 */

  /* III B */
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(b4, r1_8),
  NOTE(cis5, r1_8),
  NOTE(ais4, r1_8),
  NOTE(fis4, r1_4), /* 10 */

  /* III A */
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8), /* 6 */

  /* III C */
  NOTE(rest, r1_8),
  NOTE(fis3, r1_16), NOTE(rest, r1_16),
  NOTE(fis3, r1_16), NOTE(rest, r1_16),
  NOTE(cis3, r1_8),
  NOTE(fis2, r1_4),
  NOTE(rest, r1_4), /* 8 */

  /* III A */
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8), /* 6 */

  /* III B */
  NOTE(ais4, r1_8),
  NOTE(b4, r1_8),
  NOTE(ais4, r1_8),
  NOTE(b4, r1_8),
  NOTE(cis5, r1_8),
  NOTE(ais4, r1_8),
  NOTE(fis4, r1_4), /* 7 */

  /* III A */
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),
  NOTE(fis4, r1_4),
  NOTE(dis4, r1_8),
  NOTE(cis4, r1_8),

  NOTE(rest, r1_4), /* 7 */

  /* CHORUS I */
  NOTE(ais4, r1_8 ), NOTE(rest, r1_8),
  NOTE(ais4, r1_8 ), NOTE(rest, r1_8),
  NOTE(gis4, r1_8 ), NOTE(rest, r1_8),
  NOTE(fis4, r1_4 ), NOTE(rest, r1_8),
  NOTE(cis4, r1_16), NOTE(rest, r1_16),
  NOTE(cis4, r1_16), NOTE(rest, r1_16),
  NOTE(cis4, r1_16), NOTE(rest, r1_16),
  NOTE(gis4, r1_16), NOTE(rest, r1_16),
  NOTE(gis4, r1_16), NOTE(rest, r1_16),
  NOTE(fis4, r1_4 ), NOTE(rest, r1_8),
  NOTE(dis4, r1_16), NOTE(rest, r1_16),
  NOTE(dis4, r1_16), NOTE(rest, r1_16),
  NOTE(dis4, r1_16), NOTE(rest, r1_16),
  NOTE(fis4, r1_16), NOTE(rest, r1_16),
  NOTE(gis4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r3_16),
  NOTE(fis4, r1_8 ), NOTE(rest, r1_16),
  NOTE(b3,   r1_16), NOTE(rest, r1_16),
  NOTE(b3,   r1_16), NOTE(rest, r1_16),
  NOTE(b3,   r1_16), NOTE(rest, r1_16),
  NOTE(b3,   r1_16), NOTE(rest, r1_16),
  NOTE(b3,   r1_16), NOTE(rest, r1_16),
  NOTE(ais3, r3_16),
  NOTE(fis4, r1_16),

  NOTE(ais4, r1_8 ), NOTE(rest, r1_8),
  NOTE(ais4, r1_8 ), NOTE(rest, r1_8),
  NOTE(gis4, r1_8 ), NOTE(rest, r1_8),
  NOTE(fis4, r1_4 ), NOTE(rest, r1_8),
  NOTE(cis4, r1_16), NOTE(rest, r1_16),
  NOTE(cis4, r1_16), NOTE(rest, r1_16),
  NOTE(cis4, r1_16), NOTE(rest, r1_16),
  NOTE(gis4, r1_16), NOTE(rest, r1_16),
  NOTE(gis4, r1_16), NOTE(rest, r1_16),
  NOTE(fis4, r1_4 ), NOTE(rest, r1_8),
  NOTE(dis4, r1_16), NOTE(rest, r1_16),
  NOTE(dis4, r1_16), NOTE(rest, r1_16),
  NOTE(dis4, r1_16), NOTE(rest, r1_16),
  NOTE(fis4, r1_16), NOTE(rest, r1_16),
  NOTE(gis4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r3_16),
  NOTE(fis4, r1_8 ), NOTE(rest, r1_16),

  NOTE(fis4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(ais4, r1_16), NOTE(rest, r1_16),
  NOTE(gis4, r1_16), NOTE(rest, r1_16),
  NOTE(gis4, r1_16), NOTE(rest, r1_16),
  NOTE(fis4, r1_4),

  NOTE(rest, r1_4), /* 91 */
  NOTE(rest, r1_2),

};
