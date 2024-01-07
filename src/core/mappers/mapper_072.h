/*
 *  Copyright (C) 2010-2024 Fabio Cavallo (aka FHorse)
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

#ifndef MAPPER_072_H_
#define MAPPER_072_H_

#include "common.h"

typedef struct _m072 {
	BYTE prg;
	BYTE chr;
	BYTE reg;
	struct _m072_snd {
		BYTE speech;
		BYTE playing;
		SWORD out;
	} snd;
} _m072;

extern _m072 m072;

void map_init_072(void);
void extcl_after_mapper_init_072(void);
void extcl_mapper_quit_072(void);
void extcl_cpu_wr_mem_072(BYTE nidx, WORD address, BYTE value);
BYTE extcl_save_mapper_072(BYTE mode, BYTE slot, FILE *fp);
void extcl_cpu_every_cycle_072(BYTE nidx);

#endif /* MAPPER_072_H_ */
