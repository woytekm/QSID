//
// Q-SID OS
//
// MIDI definitions and variables
//


// array translating MIDI note to SID oscillator register value
// (equal tempered scale - from SID reference)

uint16_t G_MIDI_to_SID_reg[95] = { 274, 291, 308, 325, 346, 366, 388, 411, 435, 461, 489, 518,
                                   549, 581, 616, 652, 691, 732, 776, 822, 871, 923, 978, 1036,
                                   1097, 1163, 1232, 1305, 1383, 1465, 1552, 1644, 1742, 1845 1955,
                                   2071, 2195, 2325, 2463, 2610, 2765, 2930, 3104, 3288, 3484, 3691,
                                   3910, 4143, 4389, 4650, 4927, 5220, 5530, 5859, 6207, 6577, 6968,
                                   7382, 7821, 8286, 8779, 9301, 9854, 10440, 11060, 11718, 12415,
                                   13153, 13935, 14764, 15642, 16572, 17557, 18601, 19709, 20897,
                                   22121, 23436, 24830, 26306, 27871, 29528, 31234, 33144, 35115, 
                                   37203, 39415, 41759, 44242, 46873, 49660, 52613, 55741, 59056,
                                   62657 };


// midi.h end
