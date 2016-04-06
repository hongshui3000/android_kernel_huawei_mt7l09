/*
********************************************************************************
*
*      GSM AMR-NB speech codec   R98   Version 7.6.0   December 12, 2001
*                                R99   Version 3.3.0
*                                REL-4 Version 4.1.0
*
********************************************************************************
*
*      File             : pow2.c
*      Purpose          : Table for az_lsf()
*      $Id $
*
********************************************************************************
*/
#include "codec_op_etsi.h"
#include "cnst.h"

/*-------------------------------------------------------------*
 *  Table for az_lsf()                                         *
 *                                                             *
 * grid[0] = 1.0;                                              *
 * grid[grid_points+1] = -1.0;                                 *
 * for (i = 1; i < grid_points; i++)                           *
 *   grid[i] = cos((6.283185307*i)/(2.0*grid_points));         *
 *                                                             *
 *-------------------------------------------------------------*/

#define grid_points 60

static const Word16 grid[grid_points + 1] =
{
    32760, 32723, 32588, 32364, 32051, 31651,
    31164, 30591, 29935, 29196, 28377, 27481,
    26509, 25465, 24351, 23170, 21926, 20621,
    19260, 17846, 16384, 14876, 13327, 11743,
    10125, 8480, 6812, 5126, 3425, 1714,
    0, -1714, -3425, -5126, -6812, -8480,
    -10125, -11743, -13327, -14876, -16384, -17846,
    -19260, -20621, -21926, -23170, -24351, -25465,
    -26509, -27481, -28377, -29196, -29935, -30591,
    -31164, -31651, -32051, -32364, -32588, -32723,
    -32760
};