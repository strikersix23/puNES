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

#include <string.h>
#include "mappers.h"
#include "info.h"
#include "mem_map.h"
#include "irqA12.h"
#include "save_slot.h"

INLINE static void prg_fix_356(BYTE value);
INLINE static void prg_swap_356(WORD address, WORD value);
INLINE static void chr_fix_356(BYTE value);
INLINE static void chr_swap_356(WORD address, WORD value);
INLINE static void mirroring_fix_356(void);

struct _m356 {
	BYTE index;
	BYTE chr_writable;
	BYTE reg[5];
	WORD mmc3[8];
} m356;

void map_init_356(void) {
	EXTCL_AFTER_MAPPER_INIT(356);
	EXTCL_CPU_WR_MEM(356);
	EXTCL_SAVE_MAPPER(356);
	EXTCL_WR_CHR(356);
	EXTCL_CPU_EVERY_CYCLE(MMC3);
	EXTCL_PPU_000_TO_34X(MMC3);
	EXTCL_PPU_000_TO_255(MMC3);
	EXTCL_PPU_256_TO_319(MMC3);
	EXTCL_PPU_320_TO_34X(MMC3);
	EXTCL_UPDATE_R2006(MMC3);
	mapper.internal_struct[0] = (BYTE *)&m356;
	mapper.internal_struct_size[0] = sizeof(m356);
	mapper.internal_struct[1] = (BYTE *)&mmc3;
	mapper.internal_struct_size[1] = sizeof(mmc3);

	memset(&mmc3, 0x00, sizeof(mmc3));
	memset(&irqA12, 0x00, sizeof(irqA12));
	memset(&m356, 0x00, sizeof(m356));

	m356.reg[2] = 0x0F;

	m356.mmc3[0] = 0;
	m356.mmc3[1] = 2;
	m356.mmc3[2] = 4;
	m356.mmc3[3] = 5;
	m356.mmc3[4] = 6;
	m356.mmc3[5] = 7;
	m356.mmc3[6] = 0;
	m356.mmc3[7] = 0;

	if (info.format == NES_2_0) {
		if (info.chr.ram.banks_8k_plus > 0) {
			map_chr_ram_extra_init(info.chr.ram.banks_8k_plus * 0x2000);
		}
	} else {
		map_chr_ram_extra_init(0x2000);
	}

	info.mapper.extend_wr = TRUE;

	irqA12.present = TRUE;
	irqA12_delay = 1;
}
void extcl_after_mapper_init_356(void) {
	prg_fix_356(mmc3.bank_to_update);
	chr_fix_356(mmc3.bank_to_update);
	mirroring_fix_356();
}
void extcl_cpu_wr_mem_356(WORD address, BYTE value) {
	if ((address >= 0x6000) && (address <= 0x7FFF)) {
		if (cpu.prg_ram_wr_active && !(m356.reg[3] & 0x40)) {
			m356.reg[m356.index] = value;
			m356.index = (m356.index + 1) & 0x03;
			prg_fix_356(mmc3.bank_to_update);
			chr_fix_356(mmc3.bank_to_update);
			mirroring_fix_356();
		}
		return;
	}
	if (address >= 0x8000) {
		switch (address & 0xE001) {
			case 0x8000:
				if ((value & 0x40) != (mmc3.bank_to_update & 0x40)) {
					prg_fix_356(value);
				}
				if ((value & 0x80) != (mmc3.bank_to_update & 0x80)) {
					chr_fix_356(value);
				}
				mmc3.bank_to_update = value;
				return;
			case 0x8001: {
				WORD cbase = (mmc3.bank_to_update & 0x80) << 5;

				m356.mmc3[mmc3.bank_to_update & 0x07] = value;

				switch (mmc3.bank_to_update & 0x07) {
					case 0:
						chr_swap_356(cbase ^ 0x0000, value & (~1));
						chr_swap_356(cbase ^ 0x0400, value | 1);
						return;
					case 1:
						chr_swap_356(cbase ^ 0x0800, value & (~1));
						chr_swap_356(cbase ^ 0x0C00, value | 1);
						return;
					case 2:
						chr_swap_356(cbase ^ 0x1000, value);
						return;
					case 3:
						chr_swap_356(cbase ^ 0x1400, value);
						return;
					case 4:
						chr_swap_356(cbase ^ 0x1800, value);
						return;
					case 5:
						chr_swap_356(cbase ^ 0x1C00, value);
						return;
					case 6:
						if (mmc3.bank_to_update & 0x40) {
							prg_swap_356(0xC000, value);
						} else {
							prg_swap_356(0x8000, value);
						}
						return;
					case 7:
						prg_swap_356(0xA000, value);
						return;
				}
				return;
			}
			case 0xA000:
				m356.reg[4] = value;
				mirroring_fix_356();
				return;
		}
		extcl_cpu_wr_mem_MMC3(address, value);
	}
}
BYTE extcl_save_mapper_356(BYTE mode, BYTE slot, FILE *fp) {
	save_slot_ele(mode, slot, m356.index);
	save_slot_ele(mode, slot, m356.chr_writable);
	save_slot_ele(mode, slot, m356.reg);
	save_slot_ele(mode, slot, m356.mmc3);
	extcl_save_mapper_MMC3(mode, slot, fp);

	if (mode == SAVE_SLOT_READ) {
		chr_fix_356(mmc3.bank_to_update);
		mirroring_fix_356();
	}

	return (EXIT_OK);
}
void extcl_wr_chr_356(WORD address, BYTE value) {
	if (m356.chr_writable) {
		chr.bank_1k[address >> 10][address & 0x3FF] = value;
	}
}

INLINE static void prg_fix_356(BYTE value) {
	if (value & 0x40) {
		prg_swap_356(0x8000, ~1);
		prg_swap_356(0xC000, m356.mmc3[6]);
	} else {
		prg_swap_356(0x8000, m356.mmc3[6]);
		prg_swap_356(0xC000, ~1);
	}
	prg_swap_356(0xA000, m356.mmc3[7]);
	prg_swap_356(0xE000, ~0);
}
INLINE static void prg_swap_356(WORD address, WORD value) {
	WORD base = m356.reg[1] | ((m356.reg[2] & 0xC0) << 2);
	WORD mask = ~m356.reg[3] & 0x3F;

	value = (base & ~mask) | (value & mask);
	control_bank(info.prg.rom.max.banks_8k)
	map_prg_rom_8k(1, (address >> 13) & 0x03, value);
	map_prg_rom_8k_update();
}
INLINE static void chr_fix_356(BYTE value) {
	WORD cbase = (value & 0x80) << 5;

	chr_swap_356(cbase ^ 0x0000, m356.mmc3[0] & (~1));
	chr_swap_356(cbase ^ 0x0400, m356.mmc3[0] |   1);
	chr_swap_356(cbase ^ 0x0800, m356.mmc3[1] & (~1));
	chr_swap_356(cbase ^ 0x0C00, m356.mmc3[1] |   1);
	chr_swap_356(cbase ^ 0x1000, m356.mmc3[2]);
	chr_swap_356(cbase ^ 0x1400, m356.mmc3[3]);
	chr_swap_356(cbase ^ 0x1800, m356.mmc3[4]);
	chr_swap_356(cbase ^ 0x1C00, m356.mmc3[5]);
}
INLINE static void chr_swap_356(WORD address, WORD value) {
	if (!(m356.reg[2] & 0x20)) {
		m356.chr_writable = TRUE;
		value = address >> 10;
		chr.bank_1k[address >> 10] = &chr.extra.data[value << 10];
	} else {
		WORD base = ((m356.reg[2] & 0xF0) << 4) | m356.reg[0];
		WORD mask = 0xFF >> (~m356.reg[2] & 0x0F);

		m356.chr_writable = FALSE;
		value = (base & ~mask) | (value & mask);
		control_bank(info.chr.rom.max.banks_1k)
		chr.bank_1k[address >> 10] = chr_pnt(value << 10);
	}
}
INLINE static void mirroring_fix_356(void) {
	if (m356.reg[2] & 0x40) {
		mirroring_FSCR();
	} else {
		if (m356.reg[4] & 0x01) {
			mirroring_H();
		} else {
			mirroring_V();
		}
	}
}