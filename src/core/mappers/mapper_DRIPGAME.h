/*
 *  Copyright (C) 2010-2022 Fabio Cavallo (aka FHorse)
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

#ifndef MAPPER_DRIPGAME_H_
#define MAPPER_DRIPGAME_H_

#include "common.h"

typedef struct _dripgame_channel {
	SWORD out;
	BYTE fifo[256];
	BYTE full;
	BYTE empty;
	WORD freq;
	WORD vol;
	WORD timer;
	struct _dripgame_channel_pos {
		BYTE read;
		BYTE write;
	} pos;
} _dripgame_channel;
typedef struct _dripgame {
	BYTE dipswitch;
	BYTE control;
	BYTE prg;
	BYTE chr[4];
	BYTE extended_attributes[2][0x400];
	_dripgame_channel channel[2];
	struct _dripgame_irq {
		BYTE enabled;
		BYTE latch;
		WORD counter;
	} irq;
} _dripgame;

extern _dripgame dripgame;

void map_init_DRIPGAME(void);
void extcl_after_mapper_init_DRIPGAME(void);
void extcl_cpu_wr_mem_DRIPGAME(WORD address, BYTE value);
BYTE extcl_cpu_rd_mem_DRIPGAME(WORD address, BYTE openbus, BYTE before);
BYTE extcl_save_mapper_DRIPGAME(BYTE mode, BYTE slot, FILE *fp);
BYTE extcl_rd_nmt_DRIPGAME(WORD address);
void extcl_cpu_every_cycle_DRIPGAME(void);

#endif /* MAPPER_DRIPGAME_H_ */
