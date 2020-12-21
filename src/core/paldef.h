/*
 *  Copyright (C) 2010-2021 Fabio Cavallo (aka FHorse)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef PALDEF_H_
#define PALDEF_H_

static const _color_RGB palette_base_pal[64] = {
	{0x80,0x80,0x80}, {0x00,0x00,0xBB}, {0x37,0x00,0xBF}, {0x84,0x00,0xA6},
	{0xBB,0x00,0x6A}, {0xB7,0x00,0x1E}, {0xB3,0x00,0x00}, {0x91,0x26,0x00},
	{0x7B,0x2B,0x00}, {0x00,0x3E,0x00}, {0x00,0x48,0x0D}, {0x00,0x3C,0x22},
	{0x00,0x2F,0x66}, {0x00,0x00,0x00}, {0x05,0x05,0x05}, {0x05,0x05,0x05},

	{0xC8,0xC8,0xC8}, {0x00,0x59,0xFF}, {0x44,0x3C,0xFF}, {0xB7,0x33,0xCC},
	{0xFF,0x33,0xAA}, {0xFF,0x37,0x5E}, {0xFF,0x37,0x1A}, {0xD5,0x4B,0x00},
	{0xC4,0x62,0x00}, {0x3C,0x7B,0x00}, {0x1E,0x84,0x15}, {0x00,0x95,0x66},
	{0x00,0x84,0xC4}, {0x11,0x11,0x11}, {0x09,0x09,0x09}, {0x09,0x09,0x09},

	{0xFF,0xFF,0xFF}, {0x00,0x95,0xFF}, {0x6F,0x84,0xFF}, {0xD5,0x6F,0xFF},
	{0xFF,0x77,0xCC}, {0xFF,0x6F,0x99}, {0xFF,0x7B,0x59}, {0xFF,0x91,0x5F},
	{0xFF,0xA2,0x33}, {0xA6,0xBF,0x00}, {0x51,0xD9,0x6A}, {0x4D,0xD5,0xAE},
	{0x00,0xD9,0xFF}, {0x66,0x66,0x66}, {0x0D,0x0D,0x0D}, {0x0D,0x0D,0x0D},

	{0xFF,0xFF,0xFF}, {0x84,0xBF,0xFF}, {0xBB,0xBB,0xFF}, {0xD0,0xBB,0xFF},
	{0xFF,0xBF,0xEA}, {0xFF,0xBF,0xCC}, {0xFF,0xC4,0xB7}, {0xFF,0xCC,0xAE},
	{0xFF,0xD9,0xA2}, {0xCC,0xE1,0x99}, {0xAE,0xEE,0xB7}, {0xAA,0xF7,0xEE},
	{0xB3,0xEE,0xFF}, {0xDD,0xDD,0xDD}, {0x11,0x11,0x11}, {0x11,0x11,0x11}
};
// Thx to Firebrandx (http://www.firebrandx.com/nespalette.html)
static const _color_RGB palette_firebrandx_unsaturated_v5[64] = {
	{0x6B,0x6B,0x6B}, {0x00,0x1E,0x87}, {0x1F,0x0B,0x96}, {0x3B,0x0C,0x87},
	{0x59,0x0D,0x61}, {0x5E,0x05,0x28}, {0x55,0x11,0x00}, {0x46,0x1B,0x00},
	{0x30,0x32,0x00}, {0x0A,0x48,0x00}, {0x00,0x4E,0x00}, {0x00,0x46,0x19},
	{0x00,0x39,0x5A}, {0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xB2,0xB2,0xB2}, {0x1A,0x53,0xD1}, {0x48,0x35,0xEE}, {0x71,0x23,0xEC},
	{0x9A,0x1E,0xB7}, {0xA5,0x1E,0x62}, {0xA5,0x2D,0x19}, {0x87,0x4B,0x00},
	{0x67,0x69,0x00}, {0x29,0x84,0x00}, {0x03,0x8B,0x00}, {0x00,0x82,0x40},
	{0x00,0x70,0x96}, {0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xFF,0xFF,0xFF}, {0x63,0xAD,0xFD}, {0x90,0x8A,0xFE}, {0xB9,0x77,0xFC},
	{0xE7,0x71,0xFE}, {0xF7,0x6F,0xC9}, {0xF5,0x83,0x6A}, {0xDD,0x9C,0x29},
	{0xBD,0xB8,0x07}, {0x84,0xD1,0x07}, {0x5B,0xDC,0x3B}, {0x48,0xD7,0x7D},
	{0x48,0xC6,0xD8}, {0x55,0x55,0x55}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xFF,0xFF,0xFF}, {0xC4,0xE3,0xFE}, {0xD7,0xD5,0xFE}, {0xE6,0xCD,0xFE},
	{0xF9,0xCA,0xFE}, {0xFE,0xC9,0xF0}, {0xFE,0xD1,0xC7}, {0xF7,0xDC,0xAC},
	{0xE8,0xE8,0x9C}, {0xD1,0xF2,0x9D}, {0xBF,0xF4,0xB1}, {0xB7,0xF5,0xCD},
	{0xB7,0xEB,0xF2}, {0xBE,0xBE,0xBE}, {0x00,0x00,0x00}, {0x00,0x00,0x00}
};
// Thx to Firebrandx (http://www.firebrandx.com/nespalette.html)
static const _color_RGB palette_firebrandx_unsaturated_v6[64] = {
	{0x6B,0x6B,0x6B}, {0x00,0x1E,0x87}, {0x1F,0x0B,0x96}, {0x3B,0x0C,0x87},
	{0x59,0x0D,0x61}, {0x5E,0x05,0x28}, {0x55,0x11,0x00}, {0x46,0x1B,0x00},
	{0x30,0x32,0x00}, {0x0A,0x48,0x00}, {0x00,0x4E,0x00}, {0x00,0x46,0x19},
	{0x00,0x3A,0x58}, {0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xB2,0xB2,0xB2}, {0x1A,0x53,0xD1}, {0x48,0x35,0xEE}, {0x71,0x23,0xEC},
	{0x9A,0x1E,0xB7}, {0xA5,0x1E,0x62}, {0xA5,0x2D,0x19}, {0x87,0x4B,0x00},
	{0x67,0x69,0x00}, {0x29,0x84,0x00}, {0x03,0x8B,0x00}, {0x00,0x82,0x40},
	{0x00,0x78,0x91}, {0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xFF,0xFF,0xFF}, {0x63,0xAD,0xFD}, {0x90,0x8A,0xFE}, {0xB9,0x77,0xFC},
	{0xE7,0x71,0xFE}, {0xF7,0x6F,0xC9}, {0xF5,0x83,0x6A}, {0xDD,0x9C,0x29},
	{0xBD,0xB8,0x07}, {0x84,0xD1,0x07}, {0x5B,0xDC,0x3B}, {0x48,0xD7,0x7D},
	{0x48,0xCC,0xCE}, {0x55,0x55,0x55}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xFF,0xFF,0xFF}, {0xC4,0xE3,0xFE}, {0xD7,0xD5,0xFE}, {0xE6,0xCD,0xFE},
	{0xF9,0xCA,0xFE}, {0xFE,0xC9,0xF0}, {0xFE,0xD1,0xC7}, {0xF7,0xDC,0xAC},
	{0xE8,0xE8,0x9C}, {0xD1,0xF2,0x9D}, {0xBF,0xF4,0xB1}, {0xB7,0xF5,0xCD},
	{0xB7,0xF0,0xEE}, {0xBE,0xBE,0xBE}, {0x00,0x00,0x00}, {0x00,0x00,0x00}
};
// Thx to Firebrandx (http://www.firebrandx.com/nespalette.html)
static const _color_RGB palette_firebrandx_nostalgia_FBX[64] = {
	{0x65,0x65,0x65}, {0x00,0x12,0x7D}, {0x18,0x00,0x8E}, {0x36,0x00,0x82},
	{0x56,0x00,0x5D}, {0x5A,0x00,0x18}, {0x4F,0x05,0x00}, {0x38,0x19,0x00},
	{0x1D,0x31,0x00}, {0x00,0x3D,0x00}, {0x00,0x41,0x00}, {0x00,0x3B,0x17},
	{0x00,0x2E,0x55}, {0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xAF,0xAF,0xAF}, {0x19,0x4E,0xC8}, {0x47,0x2F,0xE3}, {0x6B,0x1F,0xD7},
	{0x93,0x1B,0xAE}, {0x9E,0x1A,0x5E}, {0x97,0x32,0x00}, {0x7B,0x4B,0x00},
	{0x5B,0x67,0x00}, {0x26,0x7A,0x00}, {0x00,0x82,0x00}, {0x00,0x7A,0x3E},
	{0x00,0x6E,0x8A}, {0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xFF,0xFF,0xFF}, {0x64,0xA9,0xFF}, {0x8E,0x89,0xFF}, {0xB6,0x76,0xFF},
	{0xE0,0x6F,0xFF}, {0xEF,0x6C,0xC4}, {0xF0,0x80,0x6A}, {0xD8,0x98,0x2C},
	{0xB9,0xB4,0x0A}, {0x83,0xCB,0x0C}, {0x5B,0xD6,0x3F}, {0x4A,0xD1,0x7E},
	{0x4D,0xC7,0xCB}, {0x4C,0x4C,0x4C}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xFF,0xFF,0xFF}, {0xC7,0xE5,0xFF}, {0xD9,0xD9,0xFF}, {0xE9,0xD1,0xFF},
	{0xF9,0xCE,0xFF}, {0xFF,0xCC,0xF1}, {0xFF,0xD4,0xCB}, {0xF8,0xDF,0xB1},
	{0xED,0xEA,0xA4}, {0xD6,0xF4,0xA4}, {0xC5,0xF8,0xB8}, {0xBE,0xF6,0xD3},
	{0xBF,0xF1,0xF1}, {0xB9,0xB9,0xB9}, {0x00,0x00,0x00}, {0x00,0x00,0x00}
};
// Thx to Firebrandx (http://www.firebrandx.com/nespalette.html)
static const _color_RGB palette_firebrandx_YUV_v3[64] = {
	{0x66,0x66,0x66}, {0x00,0x2A,0x88}, {0x14,0x12,0xA7}, {0x3B,0x00,0xA4},
	{0x5C,0x00,0x7E}, {0x6E,0x00,0x40}, {0x6C,0x07,0x00}, {0x56,0x1D,0x00},
	{0x33,0x35,0x00}, {0x0C,0x48,0x00}, {0x00,0x52,0x00}, {0x00,0x4C,0x18},
	{0x00,0x3E,0x5B}, {0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xAD,0xAD,0xAD}, {0x15,0x5F,0xD9}, {0x42,0x40,0xFF}, {0x75,0x27,0xFE},
	{0xA0,0x1A,0xCC}, {0xB7,0x1E,0x7B}, {0xB5,0x31,0x20}, {0x99,0x4E,0x00},
	{0x6B,0x6D,0x00}, {0x38,0x87,0x00}, {0x0D,0x93,0x00}, {0x00,0x8C,0x47},
	{0x00,0x7A,0xA0}, {0x00,0x00,0x00}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xFF,0xFF,0xFF}, {0x64,0xB0,0xFF}, {0x92,0x90,0xFF}, {0xC6,0x76,0xFF},
	{0xF2,0x6A,0xFF}, {0xFF,0x6E,0xCC}, {0xFF,0x81,0x70}, {0xEA,0x9E,0x22},
	{0xBC,0xBE,0x00}, {0x88,0xD8,0x00}, {0x5C,0xE4,0x30}, {0x45,0xE0,0x82},
	{0x48,0xCD,0xDE}, {0x4F,0x4F,0x4F}, {0x00,0x00,0x00}, {0x00,0x00,0x00},

	{0xFF,0xFF,0xFF}, {0xC0,0xDF,0xFF}, {0xD3,0xD2,0xFF}, {0xE8,0xC8,0xFF},
	{0xFA,0xC2,0xFF}, {0xFF,0xC4,0xEA}, {0xFF,0xCC,0xC5}, {0xF7,0xD8,0xA5},
	{0xE4,0xE5,0x94}, {0xCF,0xEF,0x96}, {0xBD,0xF4,0xAB}, {0xB3,0xF3,0xCC},
	{0xB5,0xEB,0xF2}, {0xB8,0xB8,0xB8}, {0x00,0x00,0x00}, {0x00,0x00,0x00}
};
// RP2C04-0001
static const _color_RGB palette_RP2C04_0001[64] = {
	{0xFF,0xB6,0xB6}, {0xDB,0x6D,0xFF}, {0xFF,0x00,0x00}, {0x92,0x92,0xFF},
	{0x00,0x92,0x92}, {0x24,0x49,0x00}, {0x49,0x49,0x49}, {0xFF,0x00,0x92},
	{0xFF,0xFF,0xFF}, {0x6D,0x6D,0x6D}, {0xFF,0xB6,0x00}, {0xB6,0x00,0x6D},
	{0x92,0x00,0x6D}, {0xDB,0xDB,0x00}, {0x6D,0x49,0x00}, {0xFF,0xFF,0xFF},

	{0x6D,0xB6,0xFF}, {0xDB,0xB6,0x6D}, {0x6D,0x24,0x00}, {0x6D,0xDB,0x00},
	{0x92,0xDB,0xFF}, {0xDB,0xB6,0xFF}, {0xFF,0xDB,0x92}, {0x00,0x49,0xFF},
	{0xFF,0xDB,0x00}, {0x49,0xFF,0xDB}, {0x00,0x00,0x00}, {0x49,0x00,0x00},
	{0xDB,0xDB,0xDB}, {0x92,0x92,0x92}, {0xFF,0x00,0xFF}, {0x00,0x24,0x92},

	{0x00,0x00,0x6D}, {0xB6,0xDB,0xFF}, {0xFF,0xB6,0xFF}, {0x00,0xFF,0x00},
	{0x00,0xFF,0xFF}, {0x00,0x49,0x49}, {0x00,0xB6,0x6D}, {0xB6,0x00,0xFF},
	{0x00,0x00,0x00}, {0x92,0x49,0x00}, {0xFF,0x92,0xFF}, {0xB6,0x24,0x00},
	{0x92,0x00,0xFF}, {0x00,0x00,0xDB}, {0xFF,0x92,0x00}, {0x00,0x00,0x00},

	{0x00,0x00,0x00}, {0x24,0x92,0x00}, {0xB6,0xB6,0xB6}, {0x00,0x6D,0x24},
	{0xB6,0xFF,0x49}, {0x6D,0x49,0xDB}, {0xFF,0xFF,0x00}, {0xDB,0x6D,0x00},
	{0x00,0x49,0x00}, {0x00,0x6D,0xDB}, {0x00,0x92,0x00}, {0x24,0x24,0x24},
	{0xFF,0xFF,0x6D}, {0xFF,0x6D,0xFF}, {0x92,0x6D,0x00}, {0x92,0xFF,0x6D}
};
// RP2C04-0002
static const _color_RGB palette_RP2C04_0002[64] = {
	{0x00,0x00,0x00}, {0xFF,0xB6,0x00}, {0x92,0x6D,0x00}, {0xB6,0xFF,0x49},
	{0x92,0xFF,0x6D}, {0xFF,0x6D,0xFF}, {0x00,0x92,0x92}, {0xB6,0xDB,0xFF},
	{0xFF,0x00,0x00}, {0x92,0x00,0xFF}, {0xFF,0xFF,0x6D}, {0xFF,0x92,0xFF},
	{0xFF,0xFF,0xFF}, {0xDB,0x6D,0xFF}, {0x92,0xDB,0xFF}, {0x00,0x92,0x00},

	{0x00,0x49,0x00}, {0x6D,0xB6,0xFF}, {0xB6,0x24,0x00}, {0xDB,0xDB,0xDB},
	{0x00,0xB6,0x6D}, {0x6D,0xDB,0x00}, {0x49,0x00,0x00}, {0x92,0x92,0xFF},
	{0x49,0x49,0x49}, {0xFF,0x00,0xFF}, {0x00,0x00,0x6D}, {0x49,0xFF,0xDB},
	{0xDB,0xB6,0xFF}, {0x6D,0x49,0x00}, {0x00,0x00,0x00}, {0x6D,0x49,0xDB},

	{0x92,0x00,0x6D}, {0xFF,0xDB,0x92}, {0xFF,0x92,0x00}, {0xFF,0xB6,0xFF},
	{0x00,0x6D,0xDB}, {0x6D,0x24,0x00}, {0xB6,0xB6,0xB6}, {0x00,0x00,0xDB},
	{0xB6,0x00,0xFF}, {0xFF,0xDB,0x00}, {0x6D,0x6D,0x6D}, {0x24,0x49,0x00},
	{0x00,0x49,0xFF}, {0x00,0x00,0x00}, {0xDB,0xDB,0x00}, {0xFF,0xFF,0xFF},

	{0xDB,0xB6,0x6D}, {0x24,0x24,0x24}, {0x00,0xFF,0x00}, {0xDB,0x6D,0x00},
	{0x00,0x49,0x49}, {0x00,0x24,0x92}, {0xFF,0x00,0x92}, {0x24,0x92,0x00},
	{0x00,0x00,0x00}, {0x00,0xFF,0xFF}, {0x92,0x49,0x00}, {0xFF,0xFF,0x00},
	{0xFF,0xB6,0xB6}, {0xB6,0x00,0x6D}, {0x00,0x6D,0x24}, {0x92,0x92,0x92},
};
// RP2C04-0003
static const _color_RGB palette_RP2C04_0003[64] = {
	{0xB6,0x00,0xFF}, {0xFF,0x6D,0xFF}, {0x92,0xFF,0x6D}, {0xB6,0xB6,0xB6},
	{0x00,0x92,0x00}, {0xFF,0xFF,0xFF}, {0xB6,0xDB,0xFF}, {0x24,0x49,0x00},
	{0x00,0x24,0x92}, {0x00,0x00,0x00}, {0xFF,0xDB,0x92}, {0x6D,0x49,0x00},
	{0xFF,0x00,0x92}, {0xDB,0xDB,0xDB}, {0xDB,0xB6,0x6D}, {0x92,0xDB,0xFF},

	{0x92,0x92,0xFF}, {0x00,0x92,0x92}, {0xB6,0x00,0x6D}, {0x00,0x49,0xFF},
	{0x24,0x92,0x00}, {0x92,0x6D,0x00}, {0xDB,0x6D,0x00}, {0x00,0xB6,0x6D},
	{0x6D,0x6D,0x6D}, {0x6D,0x49,0xDB}, {0x00,0x00,0x00}, {0x00,0x00,0xDB},
	{0xFF,0x00,0x00}, {0xB6,0x24,0x00}, {0xFF,0x92,0xFF}, {0xFF,0xB6,0xB6},

	{0xDB,0x6D,0xFF}, {0x00,0x49,0x00}, {0x00,0x00,0x6D}, {0xFF,0xFF,0x00},
	{0x24,0x24,0x24}, {0xFF,0xB6,0x00}, {0xFF,0x92,0x00}, {0xFF,0xFF,0xFF},
	{0x6D,0xDB,0x00}, {0x92,0x00,0x6D}, {0x6D,0xB6,0xFF}, {0xFF,0x00,0xFF},
	{0x00,0x6D,0xDB}, {0x92,0x92,0x92}, {0x00,0x00,0x00}, {0x6D,0x24,0x00},

	{0x00,0xFF,0xFF}, {0x49,0x00,0x00}, {0xB6,0xFF,0x49}, {0xFF,0xB6,0xFF},
	{0x92,0x49,0x00}, {0x00,0xFF,0x00}, {0xDB,0xDB,0x00}, {0x49,0x49,0x49},
	{0x00,0x6D,0x24}, {0x00,0x00,0x00}, {0xDB,0xB6,0xFF}, {0xFF,0xFF,0x6D},
	{0x92,0x00,0xFF}, {0x49,0xFF,0xDB}, {0xFF,0xDB,0x00}, {0x00,0x49,0x49},
};
// RP2C04-0004
static const _color_RGB palette_RP2C04_0004[64] = {
	{0x92,0x6D,0x00}, {0x6D,0x49,0xDB}, {0x00,0x92,0x92}, {0xDB,0xDB,0x00},
	{0x00,0x00,0x00}, {0xFF,0xB6,0xB6}, {0x00,0x24,0x92}, {0xDB,0x6D,0x00},
	{0xB6,0xB6,0xB6}, {0x6D,0x24,0x00}, {0x00,0xFF,0x00}, {0x00,0x00,0x6D},
	{0xFF,0xDB,0x92}, {0xFF,0xFF,0x00}, {0x00,0x92,0x00}, {0xB6,0xFF,0x49},

	{0xFF,0x6D,0xFF}, {0x49,0x00,0x00}, {0x00,0x49,0xFF}, {0xFF,0x92,0xFF},
	{0x00,0x00,0x00}, {0x49,0x49,0x49}, {0xB6,0x24,0x00}, {0xFF,0x92,0x00},
	{0xDB,0xB6,0x6D}, {0x00,0xB6,0x6D}, {0x92,0x92,0xFF}, {0x24,0x92,0x00},
	{0x92,0x00,0x6D}, {0x00,0x00,0x00}, {0x92,0xFF,0x6D}, {0x6D,0xB6,0xFF},

	{0xB6,0x00,0x6D}, {0x00,0x6D,0x24}, {0x92,0x49,0x00}, {0x00,0x00,0xDB},
	{0x92,0x00,0xFF}, {0xB6,0x00,0xFF}, {0x6D,0x6D,0x6D}, {0xFF,0x00,0x92},
	{0x00,0x49,0x49}, {0xDB,0xDB,0xDB}, {0x00,0x6D,0xDB}, {0x00,0x49,0x00},
	{0x24,0x24,0x24}, {0xFF,0xFF,0x6D}, {0x92,0x92,0x92}, {0xFF,0x00,0xFF},

	{0xFF,0xB6,0xFF}, {0xFF,0xFF,0xFF}, {0x6D,0x49,0x00}, {0xFF,0x00,0x00},
	{0xFF,0xDB,0x00}, {0x49,0xFF,0xDB}, {0xFF,0xFF,0xFF}, {0x92,0xDB,0xFF},
	{0x00,0x00,0x00}, {0xFF,0xB6,0x00}, {0xDB,0x6D,0xFF}, {0xB6,0xDB,0xFF},
	{0x6D,0xDB,0x00}, {0xDB,0xB6,0xFF}, {0x00,0xFF,0xFF}, {0x24,0x49,0x00},
};
#endif /* PALDEF_H_ */
