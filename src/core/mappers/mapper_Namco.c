/*
 *  Copyright (C) 2010-2023 Fabio Cavallo (aka FHorse)
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

#include <stdlib.h>
#include <string.h>
#include "mappers.h"
#include "info.h"
#include "mem_map.h"
#include "cpu.h"
#include "save_slot.h"
#include "emu.h"
#include "tas.h"
#include "gui.h"

INLINE static void prg_fix_Namco_163(void);
INLINE static void chr_fix_Namco_163(void);
INLINE static void chr_swap_Namco_163(BYTE **dst, DBWORD value, BYTE force_chrom, BYTE *bank_writable);
INLINE static void snd_Namco_163_set_volume(void);
INLINE static SWORD snd_Namco_163_wave(int cycles, int channel_offset);

struct _n3425 {
	BYTE bank_to_update;
} n3425;
struct _n3446 {
	BYTE bank_to_update;
	BYTE prg_rom_mode;
} n3446;
_chinaersan2 chinaersan2;
_n163 n163;
struct _namcotmp {
	BYTE ram[0x80];
	BYTE chr_bank_writable[8];
	BYTE nmt_bank_writable[4];
	SWORD volume;
	BYTE hardwired;
	BYTE type;
} namcotmp;

void map_init_Namco(BYTE model) {
	switch (model) {
		case N163:
		case CHINA_ER_SAN2:
			EXTCL_AFTER_MAPPER_INIT(Namco_163);
			EXTCL_MAPPER_QUIT(Namco_163);
			EXTCL_CPU_WR_MEM(Namco_163);
			EXTCL_CPU_RD_MEM(Namco_163);
			EXTCL_SAVE_MAPPER(Namco_163);
			EXTCL_WR_NMT(Namco_163);
			EXTCL_WR_CHR(Namco_163);
			EXTCL_CPU_EVERY_CYCLE(Namco_163);
			EXTCL_BATTERY_IO(Namco_163);
			EXTCL_APU_TICK(Namco_163);
			mapper.internal_struct[0] = (BYTE *)&n163;
			mapper.internal_struct_size[0] = sizeof(n163);

			if (info.prg.ram.banks_8k_plus == 0) {
				info.prg.ram.banks_8k_plus = 1;
			}

			snd_Namco_163_set_volume();

			if (info.reset >= HARD) {
				int i;

				n163.prg[0] = 0xFC;
				n163.prg[1] = 0xFD;
				n163.prg[2] = 0xFE;

				for (i = 0; i < 4; i++) {
					n163.chr[i | 0] = i;
					n163.chr[i | 4] = i | 0x04;
					n163.chr[i | 8] = (i & 0x01) | 0xE0;
				}

				n163.write_protect = 0xFF;
				n163.irq.delay = 0;
				n163.irq.count = 0;

				n163.snd.enabled = 0;
				n163.snd.adr = 0;
				n163.snd.auto_inc = 1;
				n163.snd.tick = 0;
				n163.snd.channel = 0;
				n163.snd.channel_start = 0;
				memset(&n163.snd.output, 0x00, sizeof(n163.snd.output));

				if (info.reset >= CHANGE_ROM) {
					memset(&namcotmp.ram, 0x00, sizeof(namcotmp.ram));
				}
				if (!info.prg.ram.bat.banks) {
					emu_initial_ram(namcotmp.ram, sizeof(namcotmp.ram));
				}
			} else {
				n163.irq.delay = 0;
			}

			info.mapper.extend_wr = TRUE;
			info.mapper.ram_plus_op_controlled_by_mapper = TRUE;

			namcotmp.hardwired = FALSE;

			switch (info.id) {
				case NAMCO_HARD_WIRED_V:
					namcotmp.hardwired = TRUE;
					mirroring_V();
					break;
				case NAMCO_HARD_WIRED_H:
					namcotmp.hardwired = TRUE;
					mirroring_H();
					break;
				case MINDSEEKER:
					info.prg.ram.banks_8k_plus = 1;
					info.prg.ram.bat.banks = 1;
					break;
			}

			if (model == CHINA_ER_SAN2) {
				chinaersan2.enable = TRUE;
			}
			break;
		case N3416:
		case N3425:
		case N3433:
		case N3453:
			EXTCL_CPU_WR_MEM(Namco_3425);
			EXTCL_SAVE_MAPPER(Namco_3425);
			mapper.internal_struct[0] = (BYTE *)&n3425;
			mapper.internal_struct_size[0] = sizeof(n3425);

			if (info.reset >= HARD) {
				memset(&n3425, 0x00, sizeof(n3425));
			}

			if (model == N3453) {
				mirroring_SCR0();
			}
			break;
		case N3446:
			EXTCL_CPU_WR_MEM(Namco_3446);
			EXTCL_SAVE_MAPPER(Namco_3446);
			mapper.internal_struct[0] = (BYTE *)&n3446;
			mapper.internal_struct_size[0] = sizeof(n3446);

			if (info.reset >= HARD) {
				memset(&n3446, 0x00, sizeof(n3446));
			}

			mapper.rom_map_to[2] = info.prg.rom.max.banks_8k - 1;
			break;
	}

	namcotmp.type = model;
}
void map_init_NSF_Namco(BYTE model) {
	memset(&n163, 0x00, sizeof(n163));
	memset(&namcotmp.ram, 0x00, sizeof(namcotmp.ram));

	snd_Namco_163_set_volume();

	n163.snd.enabled = 1;
	n163.snd.auto_inc = 1;

	namcotmp.type = model;
}

void extcl_after_mapper_init_Namco_163(void) {
	prg_fix_Namco_163();
	chr_fix_Namco_163();
}
void extcl_mapper_quit_Namco_163(void) {
	chinaersan2_quit();
}
void extcl_cpu_wr_mem_Namco_163(WORD address, BYTE value) {
	switch (address & 0xF800) {
		case 0x4800:
			namcotmp.ram[n163.snd.adr] = value;
			n163.snd.adr = (n163.snd.adr + n163.snd.auto_inc) & 0x07F;
			return;
		case 0x5000:
			n163.irq.count = (n163.irq.count & 0xFF00) | value;
			irq.high &= ~EXT_IRQ;
			return;
		case 0x5800:
			n163.irq.count = (value << 8) | (n163.irq.count & 0x00FF);
			irq.high &= ~EXT_IRQ;
			return;
		case 0x6000:
		case 0x6800:
		case 0x7000:
		case 0x7800: {
			BYTE slot = (address >> 11) & 0x03;
			BYTE writable = ((n163.write_protect & 0xF0) == 0x40) && !(n163.write_protect & (0x01 << slot));

			if (writable) {
				prg.ram_plus_8k[address & 0x1FFF] = value;
			}
			return;
		}
		case 0x8000:
		case 0x8800:
		case 0x9000:
		case 0x9800:
		case 0xA000:
		case 0xA800:
		case 0xB000:
		case 0xB800:
		case 0xC000:
		case 0xC800:
		case 0xD000:
		case 0xD800:
			n163.chr[(address >> 11) & 0x0F] = value;
			chr_fix_Namco_163();
			return;
		case 0xE000:
			n163.snd.enabled = !(value & 0x40);
			n163.prg[0] = value;
			prg_fix_Namco_163();
			return;
		case 0xE800:
		case 0xF000:
			n163.prg[(address >> 11) & 0x03] = value;
			prg_fix_Namco_163();
			return;
		case 0xF800:
			n163.write_protect = value;
			n163.snd.auto_inc = (value & 0x80) >> 7;
			n163.snd.adr = value & 0x7F;
			return;
	}
}
BYTE extcl_cpu_rd_mem_Namco_163(WORD address, BYTE openbus, UNUSED(BYTE before)) {
	switch (address & 0xF800) {
		case 0x4800:
			openbus = namcotmp.ram[n163.snd.adr];
			n163.snd.adr = (n163.snd.adr + n163.snd.auto_inc) & 0x07F;
			return (openbus);
		case 0x5000:
			return (n163.irq.count & 0xFF);
		case 0x5800:
			return ((n163.irq.count >> 8) & 0xFF);
		case 0x6000:
		case 0x6800:
		case 0x7000:
		case 0x7800:
			return (prg.ram_plus_8k[address & 0x1FFF]);
			//return (openbus);
		default:
			return (openbus);
	}
}
BYTE extcl_save_mapper_Namco_163(BYTE mode, BYTE slot, FILE *fp) {
	if (save_slot.version >= 28) {
		save_slot_ele(mode, slot, n163.prg);
		save_slot_ele(mode, slot, n163.chr);
		save_slot_ele(mode, slot, n163.write_protect);
		save_slot_ele(mode, slot, n163.snd.enabled);
		save_slot_ele(mode, slot, n163.snd.adr);
		save_slot_ele(mode, slot, n163.snd.auto_inc);
		save_slot_ele(mode, slot, n163.snd.tick);
		save_slot_ele(mode, slot, n163.snd.channel);
		save_slot_ele(mode, slot, n163.snd.channel_start);
		save_slot_ele(mode, slot, n163.snd.output);
		save_slot_ele(mode, slot, n163.irq.delay);
		save_slot_ele(mode, slot, n163.irq.count);
		if (mode == SAVE_SLOT_READ) {
			extcl_after_mapper_init_Namco_163();
		}
	} else {
		return (EXIT_ERROR);
	}

	return (EXIT_OK);
}
void extcl_cpu_every_cycle_Namco_163(void) {
	if (n163.irq.delay) {
		n163.irq.delay = FALSE;
		irq.high |= EXT_IRQ;
	}
	if (((n163.irq.count - 0x8000) < 0x7FFF) && (++n163.irq.count == 0xFFFF)) {
		// vale sempre il solito discorso di un ciclo di delay
		n163.irq.delay = TRUE;
	}
}
void extcl_wr_nmt_Namco_163(WORD address, BYTE value) {
	const BYTE slot = (address & 0x0FFF) >> 10;

	if (namcotmp.nmt_bank_writable[slot] | namcotmp.hardwired) {
		ntbl.bank_1k[slot][address & 0x3FF] = value;
	}
}
void extcl_wr_chr_Namco_163(WORD address, BYTE value) {
	const BYTE slot = address >> 10;

	if (namcotmp.chr_bank_writable[slot]) {
		chr.bank_1k[slot][address & 0x3FF] = value;
	}
}
void extcl_apu_tick_Namco_163(void) {
	if (n163.snd.enabled) {
		n163.snd.channel_start = (~namcotmp.ram[0x7F] >> 4) & 0x07;

		if (n163.snd.channel > 7) {
			n163.snd.channel = n163.snd.channel_start;
		}
		n163.snd.output[n163.snd.channel] = (snd_Namco_163_wave(n163.snd.tick ? 0 : 1, n163.snd.channel * 8 + 0x40) * namcotmp.volume) / 40;
		if (++n163.snd.tick >= 15) {
			n163.snd.tick = 0;
			n163.snd.channel++;
		}
	}
}
void extcl_battery_io_Namco_163(BYTE mode, FILE *fp) {
	if (!fp || (tas.type != NOTAS)) {
		return;
	}

	if (mode == WR_BAT) {
		if (info.prg.ram.bat.banks) {
			map_bat_wr_default(fp);
			if (fwrite(namcotmp.ram, sizeof(namcotmp.ram), 1, fp) < 1) {
				log_error(uL("mapper_Namco_163;error on write 128 bytes ram"));
			}
		}
	} else {
		if (info.prg.ram.bat.banks) {
			map_bat_rd_default(fp);
			if (fread(namcotmp.ram, sizeof(namcotmp.ram), 1, fp) < 1) {
				log_error(uL("mapper_Namco_163;error on write 128 bytes ram"));
			}
		}
	}
}

INLINE static void prg_fix_Namco_163(void) {
	WORD value;
	BYTE i;

	for (i = 0; i < 3; i++) {
		value = n163.prg[i] & 0x3F;
		control_bank(info.prg.rom.max.banks_8k)
		map_prg_rom_8k(1, i, value);
	}
	value = 0x3F;
	control_bank(info.prg.rom.max.banks_8k)
	map_prg_rom_8k(1, 3, value);
	map_prg_rom_8k_update();
}
INLINE static void chr_fix_Namco_163(void) {
	BYTE force_chrom;

	force_chrom = (n163.prg[1] & 0x40) ? TRUE : FALSE;
	chr_swap_Namco_163(&chr.bank_1k[0], n163.chr[0], force_chrom, &namcotmp.chr_bank_writable[0]);
	chr_swap_Namco_163(&chr.bank_1k[1], n163.chr[1], force_chrom, &namcotmp.chr_bank_writable[1]);
	chr_swap_Namco_163(&chr.bank_1k[2], n163.chr[2], force_chrom, &namcotmp.chr_bank_writable[2]);
	chr_swap_Namco_163(&chr.bank_1k[3], n163.chr[3], force_chrom, &namcotmp.chr_bank_writable[3]);

	force_chrom = (n163.prg[1] & 0x80) ? TRUE : FALSE;
	chr_swap_Namco_163(&chr.bank_1k[4], n163.chr[4], force_chrom, &namcotmp.chr_bank_writable[4]);
	chr_swap_Namco_163(&chr.bank_1k[5], n163.chr[5], force_chrom, &namcotmp.chr_bank_writable[5]);
	chr_swap_Namco_163(&chr.bank_1k[6], n163.chr[6], force_chrom, &namcotmp.chr_bank_writable[6]);
	chr_swap_Namco_163(&chr.bank_1k[7], n163.chr[7], force_chrom, &namcotmp.chr_bank_writable[7]);

	if (!namcotmp.hardwired) {
		force_chrom = FALSE;
		chr_swap_Namco_163(&ntbl.bank_1k[0], n163.chr[8], force_chrom, &namcotmp.nmt_bank_writable[0]);
		chr_swap_Namco_163(&ntbl.bank_1k[1], n163.chr[9], force_chrom, &namcotmp.nmt_bank_writable[1]);
		chr_swap_Namco_163(&ntbl.bank_1k[2], n163.chr[10], force_chrom, &namcotmp.nmt_bank_writable[2]);
		chr_swap_Namco_163(&ntbl.bank_1k[3], n163.chr[11], force_chrom, &namcotmp.nmt_bank_writable[3]);
	}
}
INLINE static void chr_swap_Namco_163(BYTE **dst, DBWORD value, BYTE force_chrom, BYTE *bank_writable) {
	if ((value < 0xE0) || force_chrom) {
		control_bank(info.chr.rom.max.banks_1k)
		value <<= 10;
		(*dst) = chr_pnt(value);
		(*bank_writable) = mapper.write_vram;
	} else {
		value = (value & ((mapper.mirroring == MIRRORING_FOURSCR) ? 0x03 : 0x01)) << 10;
		(*dst) = &ntbl.data[value];
		(*bank_writable) = TRUE;
	}
}
INLINE static void snd_Namco_163_set_volume(void) {
	SWORD volume[6] = { 0x22, 0x22,  0, 0x22, 0x3B, 0x44 };

	namcotmp.volume = volume[info.mapper.submapper >= 6 ? 0 : info.mapper.submapper];
}
INLINE static SWORD snd_Namco_163_wave(int cycles, int channel_offset) {
	int phase = (namcotmp.ram[channel_offset + 5] << 16) |
		(namcotmp.ram[channel_offset + 3] << 8) | namcotmp.ram[channel_offset + 1];
	int freq = ((namcotmp.ram[channel_offset + 4] & 0x03) << 16) |
		(namcotmp.ram[channel_offset + 2] << 8) | (namcotmp.ram[channel_offset]);
	int length = 256 - (namcotmp.ram[channel_offset + 4] & 0xFC);
	int offset = namcotmp.ram[channel_offset + 6];
	int volume = namcotmp.ram[channel_offset + 7] & 0x0F;
	int sample, output;

	while (cycles--) {
		phase = (phase + freq) % (length << 16);
	}

	sample = ((phase >> 16) + offset) & 0xFF;
	output = (namcotmp.ram[sample >> 1] >> ((sample & 0x01) << 2)) & 0x0F;

	namcotmp.ram[channel_offset + 5] = (phase >> 16) & 0xFF;
	namcotmp.ram[channel_offset + 3] = (phase >> 8) & 0xFF;
	namcotmp.ram[channel_offset + 1] = (phase >> 0) & 0xFF;

	return (SWORD)(((output - 8) * volume));
}

void extcl_cpu_wr_mem_Namco_3425(WORD address, BYTE value) {
	if (namcotmp.type == N3453) {
		if (value & 0x40) {
			mirroring_SCR1();
		} else {
			mirroring_SCR0();
		}
	}
	switch (address & 0xE001) {
		case 0x8000:
			n3425.bank_to_update = value & 0x07;
			return;
		case 0x8001: {
			switch (n3425.bank_to_update) {
				case 0x00:
				case 0x01: {
					const BYTE slot = n3425.bank_to_update << 1;
					DBWORD bank;

					if (namcotmp.type == N3425) {
						// 7  bit  0
						// ---- ----
						// xxxx xRRR
						//       |||
						//       +++- Specify which bank register to update on next write to Bank Data register
						//            0: Select 2 KB CHR bank at PPU $0000-$07FF and nametable at PPU $2000-$27FF
						//            1: Select 2 KB CHR bank at PPU $0800-$0FFF and nametable at PPU $2800-$2FFF
						//            2: Select 1 KB CHR bank at PPU $1000-$13FF
						//            3: Select 1 KB CHR bank at PPU $1400-$17FF
						//            4: Select 1 KB CHR bank at PPU $1800-$1BFF
						//            5: Select 1 KB CHR bank at PPU $1C00-$1FFF
						//            6: Select 8 KB PRG ROM bank at $8000-$9FFF
						//            7: Select 8 KB PRG ROM bank at $A000-$BFFF
						bank = ((value >> 5) & 0x01) << 10;
						ntbl.bank_1k[slot] = &ntbl.data[bank];
						ntbl.bank_1k[slot | 0x01] = &ntbl.data[bank];
					}
					value >>= 1;
					control_bank(info.chr.rom.max.banks_2k)
					bank = value << 11;
					chr.bank_1k[slot] = chr_pnt(bank);
					chr.bank_1k[slot | 0x01] = chr_pnt(bank | 0x400);
					return;
				}
				case 0x02:
				case 0x03:
				case 0x04:
				case 0x05:
					value |= 0x40;
					control_bank(info.chr.rom.max.banks_1k)
					chr.bank_1k[n3425.bank_to_update + 2] = chr_pnt(value << 10);
					return;
				case 0x06:
					control_bank(info.prg.rom.max.banks_8k)
					map_prg_rom_8k(1, 0, value);
					map_prg_rom_8k_update();
					return;
				case 0x07:
					control_bank(info.prg.rom.max.banks_8k)
					map_prg_rom_8k(1, 1, value);
					map_prg_rom_8k_update();
					return;
			}
			return;
		}
	}
}
BYTE extcl_save_mapper_Namco_3425(BYTE mode, BYTE slot, FILE *fp) {
	save_slot_ele(mode, slot, n3425.bank_to_update);

	return (EXIT_OK);
}

void extcl_cpu_wr_mem_Namco_3446(WORD address, BYTE value) {
	switch (address & 0x8001) {
		case 0x8000:
			n3446.bank_to_update = value & 0x7;
			n3446.prg_rom_mode = (value & 0x40) >> 5;
			return;
		case 0x8001: {
			switch (n3446.bank_to_update) {
				case 0x02:
				case 0x03:
				case 0x04:
				case 0x05: {
					const BYTE slot = (n3446.bank_to_update - 2) << 1;
					DBWORD bank;

					control_bank(info.chr.rom.max.banks_2k)
					bank = value << 11;
					chr.bank_1k[slot] = chr_pnt(bank);
					chr.bank_1k[slot | 0x01] = chr_pnt(bank | 0x400);
					return;
				}
				case 0x06:
					control_bank(info.prg.rom.max.banks_8k)
					map_prg_rom_8k(1, n3446.prg_rom_mode, value);
					map_prg_rom_8k_update();
					return;
				case 0x07:
					control_bank(info.prg.rom.max.banks_8k)
					map_prg_rom_8k(1, 1, value);
					map_prg_rom_8k_update();
					return;
			}
			return;
		}
	}
}
BYTE extcl_save_mapper_Namco_3446(BYTE mode, BYTE slot, FILE *fp) {
	save_slot_ele(mode, slot, n3446.bank_to_update);
	save_slot_ele(mode, slot, n3446.prg_rom_mode);

	return (EXIT_OK);
}

// - Thx to guys of Nintendulator for this --------------------------------------------------------------------------------------
#include "ppu.h"
#include "ppu_inline.h"

#define pSan2Font chinaersan2.font.data
#define CPU_BACKUP chinaersan2.ram

#define RENDER_WIDTH 256
#define RENDER_HEIGHT 240
#define MIN_GROUP 0x6
#define MIN_INDEX 0x40
#define MAX_GROUP 0xF
#define MAX_INDEX 0x76

int txt[MAX_GROUP][MAX_INDEX] = {
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x130, 0x131, 0x126, 0x000, 0x000, 0x151, 0x000, 0x000,
		0x39F, 0x132, 0x0C8, 0x133, 0x134, 0x135, 0x3A0, 0x3A1, 0x3A2, 0x152, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x0FF, 0x34D, 0x138, 0x0C5, 0x25B, 0x1C2, 0x3A3, 0x3A4, 0x017, 0x3A5, 0x364, 0x029, 0x120, 0x000, 0x000, 0x3A6,
		0x311, 0x39F, 0x39F, 0x39F, 0x39F, 0x39F, 0x39F, 0x39F, 0x39F, 0x39F, 0x39F, 0x39F, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
		0x001, 0x002, 0x003, 0x004, 0x005, 0x006, 0x007, 0x008, 0x009, 0x00A, 0x00B, 0x00C, 0x00D, 0x00E, 0x00F, 0x010,
		0x011, 0x012, 0x013, 0x014, 0x015, 0x016, 0x017, 0x018, 0x019, 0x01A, 0x01B, 0x01C, 0x01D, 0x01E, 0x01F, 0x020,
		0x021, 0x022, 0x023, 0x024, 0x025, 0x026, 0x027, 0x028, 0x029, 0x02A, 0x02B, 0x02C, 0x02D, 0x02E, 0x02F, 0x030,
		0x1B9, 0x1BA, 0x1BB, 0x1BC, 0x1BD, 0x1BE, 0x1BF, 0x1C0, 0x1C1, 0x1C2, 0x1C3, 0x1C4, 0x1C5, 0x1C6, 0x1C7, 0x1C8,
		0x1C9, 0x1CA, 0x1CB, 0x1CC, 0x1CD, 0x1CE, 0x1CF, 0x1D0, 0x1D1, 0x1D2, 0x1D3, 0x1D4, 0x1D5, 0x1D6, 0x1D7, 0x1D8,
		0x1D9, 0x1DA, 0x1DB, 0x1DC, 0x1DD, 0x1DE, 0x1DF, 0x1E0, 0x1E1, 0x1E2, 0x1E3, 0x1E4, 0x1E5, 0x1E6, 0x1E7, 0x1E8,
		0x1E9, 0x1EA, 0x1EB, 0x1EC, 0x1ED, 0x1EE
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x031,
		0x032, 0x033, 0x034, 0x035, 0x036, 0x037, 0x038, 0x039, 0x03A, 0x03B, 0x03C, 0x03D, 0x03E, 0x03F, 0x040, 0x041,
		0x042, 0x043, 0x044, 0x045, 0x046, 0x047, 0x048, 0x049, 0x04A, 0x04B, 0x04C, 0x04D, 0x04E, 0x04F, 0x050, 0x051,
		0x052, 0x053, 0x054, 0x055, 0x056, 0x057, 0x058, 0x059, 0x05A, 0x05B, 0x05C, 0x05D, 0x05E, 0x05F, 0x060, 0x061,
		0x1EF, 0x1F0, 0x1F1, 0x1F2, 0x1F3, 0x1F4, 0x1F5, 0x1F6, 0x1F7, 0x1F8, 0x1F9, 0x1FA, 0x1FB, 0x1FC, 0x1FD, 0x1FE,
		0x1FF, 0x200, 0x201, 0x202, 0x203, 0x204, 0x205, 0x206, 0x207, 0x208, 0x209, 0x20A, 0x20B, 0x20C, 0x20D, 0x20E,
		0x20F, 0x210, 0x211, 0x212, 0x213, 0x214, 0x215, 0x216, 0x217, 0x218, 0x219, 0x21A, 0x21B, 0x21C, 0x21D, 0x21E,
		0x21F, 0x220, 0x221, 0x222, 0x223, 0x224
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x062,
		0x063, 0x064, 0x065, 0x066, 0x067, 0x068, 0x069, 0x06A, 0x06B, 0x06C, 0x06D, 0x06E, 0x06F, 0x070, 0x071, 0x072,
		0x073, 0x074, 0x075, 0x076, 0x077, 0x078, 0x079, 0x07A, 0x07B, 0x07C, 0x07D, 0x07E, 0x07F, 0x080, 0x081, 0x082,
		0x083, 0x084, 0x085, 0x086, 0x087, 0x088, 0x089, 0x08A, 0x08B, 0x08C, 0x08D, 0x08E, 0x08F, 0x090, 0x091, 0x092,
		0x225, 0x226, 0x227, 0x228, 0x229, 0x22A, 0x22B, 0x22C, 0x22D, 0x22E, 0x22F, 0x230, 0x231, 0x232, 0x233, 0x234,
		0x235, 0x236, 0x237, 0x238, 0x239, 0x23A, 0x23B, 0x23C, 0x23D, 0x23E, 0x23F, 0x240, 0x241, 0x242, 0x243, 0x244,
		0x245, 0x246, 0x247, 0x248, 0x249, 0x24A, 0x24B, 0x24C, 0x24D, 0x24E, 0x24F, 0x250, 0x251, 0x252, 0x253, 0x254,
		0x255, 0x256, 0x257, 0x258, 0x259, 0x25A
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x093,
		0x094, 0x095, 0x096, 0x097, 0x098, 0x099, 0x09A, 0x09B, 0x09C, 0x09D, 0x09E, 0x09F, 0x0A0, 0x0A1, 0x0A2, 0x0A3,
		0x0A4, 0x0A5, 0x0A6, 0x0A7, 0x0A8, 0x0A9, 0x0AA, 0x0AB, 0x0AC, 0x0AD, 0x0AE, 0x0AF, 0x0B0, 0x0B1, 0x0B2, 0x0B3,
		0x0B4, 0x0B5, 0x0B6, 0x0B7, 0x0B8, 0x0B9, 0x0BA, 0x0BB, 0x0BC, 0x0BD, 0x0BE, 0x0BF, 0x0C0, 0x0C1, 0x0C2, 0x0C3,
		0x25B, 0x25C, 0x25D, 0x25E, 0x25F, 0x260, 0x261, 0x262, 0x263, 0x264, 0x265, 0x266, 0x267, 0x268, 0x269, 0x26A,
		0x26B, 0x26C, 0x26D, 0x26E, 0x26F, 0x270, 0x271, 0x272, 0x273, 0x274, 0x275, 0x276, 0x277, 0x278, 0x279, 0x27A,
		0x27B, 0x27C, 0x27D, 0x27E, 0x27F, 0x280, 0x281, 0x282, 0x283, 0x284, 0x285, 0x286, 0x287, 0x288, 0x289, 0x28A,
		0x28B, 0x28C, 0x28D, 0x28E, 0x28F, 0x290
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x0C4,
		0x0C5, 0x0C6, 0x0C7, 0x0C8, 0x0C9, 0x0CA, 0x0CB, 0x0CC, 0x0CD, 0x0CE, 0x0CF, 0x0D0, 0x0D1, 0x0D2, 0x0D3, 0x0D4,
		0x0D5, 0x0D6, 0x0D7, 0x0D8, 0x0D9, 0x0DA, 0x0DB, 0x0DC, 0x0DD, 0x0DE, 0x0DF, 0x0E0, 0x0E1, 0x0E2, 0x0E3, 0x0E4,
		0x0E5, 0x0E6, 0x0E7, 0x0E8, 0x0E9, 0x0EA, 0x0EB, 0x0EC, 0x0ED, 0x0EE, 0x0EF, 0x0F0, 0x0F1, 0x0F2, 0x0F3, 0x0F4,
		0x291, 0x292, 0x293, 0x294, 0x295, 0x296, 0x297, 0x298, 0x299, 0x29A, 0x29B, 0x29C, 0x29D, 0x29E, 0x29F, 0x2A0,
		0x2A1, 0x2A2, 0x2A3, 0x2A4, 0x2A5, 0x2A6, 0x2A7, 0x2A8, 0x2A9, 0x2AA, 0x2AB, 0x2AC, 0x2AD, 0x2AE, 0x2AF, 0x2B0,
		0x2B1, 0x2B2, 0x2B3, 0x2B4, 0x2B5, 0x2B6, 0x2B7, 0x2B8, 0x2B9, 0x2BA, 0x2BB, 0x2BC, 0x2BD, 0x2BE, 0x2BF, 0x2C0,
		0x2C1, 0x2C2, 0x2C3, 0x2C4, 0x2C5, 0x2C6
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x0F5,
		0x0F6, 0x0F7, 0x0F8, 0x0F9, 0x0FA, 0x0FB, 0x0FC, 0x0FD, 0x0FE, 0x0FF, 0x100, 0x101, 0x102, 0x103, 0x104, 0x105,
		0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D, 0x10E, 0x10F, 0x110, 0x111, 0x112, 0x113, 0x114, 0x115,
		0x116, 0x117, 0x118, 0x119, 0x11A, 0x11B, 0x11C, 0x11D, 0x11E, 0x11F, 0x120, 0x121, 0x122, 0x123, 0x124, 0x125,
		0x2C7, 0x2C8, 0x2C9, 0x2CA, 0x2CB, 0x2CC, 0x2CD, 0x2CE, 0x2CF, 0x2D0, 0x2D1, 0x2D2, 0x2D3, 0x2D4, 0x2D5, 0x2D6,
		0x2D7, 0x2D8, 0x2D9, 0x2DA, 0x2DB, 0x2DC, 0x2DD, 0x2DE, 0x2DF, 0x2E0, 0x2E1, 0x2E2, 0x2E3, 0x2E4, 0x2E5, 0x2E6,
		0x2E7, 0x2E8, 0x2E9, 0x2EA, 0x2EB, 0x2EC, 0x2ED, 0x2EE, 0x2EF, 0x2F0, 0x2F1, 0x2F2, 0x2F3, 0x2F4, 0x2F5, 0x2F6,
		0x2F7, 0x2F8, 0x2F9, 0x2FA, 0x2FB, 0x2FC
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x126,
		0x127, 0x128, 0x129, 0x12A, 0x12B, 0x12C, 0x12D, 0x12E, 0x12F, 0x130, 0x131, 0x132, 0x133, 0x134, 0x135, 0x136,
		0x137, 0x138, 0x139, 0x13A, 0x13B, 0x13C, 0x13D, 0x13E, 0x13F, 0x140, 0x141, 0x142, 0x143, 0x144, 0x145, 0x146,
		0x147, 0x148, 0x149, 0x14A, 0x14B, 0x14C, 0x14D, 0x14E, 0x14F, 0x150, 0x151, 0x152, 0x153, 0x154, 0x155, 0x156,
		0x2FD, 0x2FE, 0x2FF, 0x300, 0x301, 0x302, 0x303, 0x304, 0x305, 0x306, 0x307, 0x308, 0x309, 0x30A, 0x30B, 0x30C,
		0x30D, 0x30E, 0x30F, 0x310, 0x311, 0x312, 0x313, 0x314, 0x315, 0x316, 0x317, 0x318, 0x319, 0x31A, 0x31B, 0x31C,
		0x31D, 0x31E, 0x31F, 0x320, 0x321, 0x322, 0x323, 0x324, 0x325, 0x326, 0x327, 0x328, 0x329, 0x32A, 0x32B, 0x32C,
		0x32D, 0x32E, 0x32F, 0x330, 0x331, 0x332
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x157,
		0x158, 0x159, 0x15A, 0x15B, 0x15C, 0x15D, 0x15E, 0x15F, 0x160, 0x161, 0x162, 0x163, 0x164, 0x165, 0x166, 0x167,
		0x168, 0x169, 0x16A, 0x16B, 0x16C, 0x16D, 0x16E, 0x16F, 0x170, 0x171, 0x172, 0x173, 0x174, 0x175, 0x176, 0x177,
		0x178, 0x179, 0x17A, 0x17B, 0x17C, 0x17D, 0x17E, 0x17F, 0x180, 0x181, 0x182, 0x183, 0x184, 0x185, 0x186, 0x187,
		0x333, 0x334, 0x335, 0x336, 0x337, 0x338, 0x339, 0x33A, 0x33B, 0x33C, 0x33D, 0x33E, 0x33F, 0x340, 0x341, 0x342,
		0x343, 0x344, 0x345, 0x346, 0x347, 0x348, 0x349, 0x34A, 0x34B, 0x34C, 0x34D, 0x34E, 0x34F, 0x350, 0x351, 0x352,
		0x353, 0x354, 0x355, 0x356, 0x357, 0x358, 0x359, 0x35A, 0x35B, 0x35C, 0x35D, 0x35E, 0x35F, 0x360, 0x361, 0x362,
		0x363, 0x364, 0x365, 0x366, 0x367, 0x368
	},
	{
		0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x188,
		0x189, 0x18A, 0x18B, 0x18C, 0x18D, 0x18E, 0x18F, 0x190, 0x191, 0x192, 0x193, 0x194, 0x195, 0x196, 0x197, 0x198,
		0x199, 0x19A, 0x19B, 0x19C, 0x19D, 0x19E, 0x19F, 0x1A0, 0x1A1, 0x1A2, 0x1A3, 0x1A4, 0x1A5, 0x1A6, 0x1A7, 0x1A8,
		0x1A9, 0x1AA, 0x1AB, 0x1AC, 0x1AD, 0x1AE, 0x1AF, 0x1B0, 0x1B1, 0x1B2, 0x1B3, 0x1B4, 0x1B5, 0x1B6, 0x1B7, 0x1B8,
		0x369, 0x36A, 0x36B, 0x36C, 0x36D, 0x36E, 0x36F, 0x370, 0x371, 0x372, 0x373, 0x374, 0x375, 0x376, 0x377, 0x378,
		0x379, 0x37A, 0x37B, 0x37C, 0x37D, 0x37E, 0x37F, 0x380, 0x381, 0x382, 0x383, 0x384, 0x385, 0x386, 0x387, 0x388,
		0x389, 0x38A, 0x38B, 0x38C, 0x38D, 0x38E, 0x38F, 0x390, 0x391, 0x392, 0x393, 0x394, 0x395, 0x396, 0x397, 0x398,
		0x399, 0x39A, 0x39B, 0x39C, 0x39D, 0x39E
	}
};

void chinaersan2_apply_font(void) { //书写浮动的文本 Writing floating text
	//访问CPU_BACKUP[]数组可以获得备份的内存(0-ff)内容 Access to CPU_BACKUP[] array to get backed up memory (0-ff) contents
	unsigned char tileid = 0, bktileid = 1, count = 0, offset = 0;
	unsigned int i, j = 0, pos = 0;
	unsigned int gridy = 0, scrolly = 0, scrollyy = 0;
	unsigned int BASE_VRAM = 0x2000, MAX_INDEX_R = MAX_INDEX, F_SPECIAL = 0;
	unsigned int Line_Start = 30, Line_End = 30;

#define LINESKIP(n) ((0x7FFF - ((CPU_BACKUP[0x69+(n<<1)]&0x7F)<<8) - CPU_BACKUP[0x68+(n<<1)])*3)/2728

	switch (CPU_BACKUP[0x60]) {
		case 0x1: //城内武将资料 City generals information
			if (CPU_BACKUP[0xBA] == 0x08) {
				if (CPU_BACKUP[0x6A] == 0x70) {
					Line_Start = 21;       //5 + LINESKIP(0) + LINESKIP(1)+ LINESKIP(2) + LINESKIP(3) + LINESKIP(4);
					{
						scrollyy = (CPU_BACKUP[0x98] & 0x07) + 2;
					}
					gridy = (CPU_BACKUP[0x9B] * 0x100 + CPU_BACKUP[0x9A] + CPU_BACKUP[0x97] * 0x400 - 0x2000) / 0x20 - Line_Start;
					if (CPU_BACKUP[0xBB] != 0x09) {
						MAX_INDEX_R = MIN_INDEX;
					}
				} else {
					Line_Start = 21;
					if (CPU_BACKUP[0x98] <= CPU_BACKUP[0x99]) {
						scrollyy = (CPU_BACKUP[0x98] + 2) & 0x07;
					} else {
						scrollyy = ((CPU_BACKUP[0x98] + 6) & 0x07) + 4;
					}
					gridy = (CPU_BACKUP[0x9B] * 0x100 + CPU_BACKUP[0x9A] - 0x2000) / 0x20 - Line_Start;
					if (CPU_BACKUP[0xBB] != 0x09) {
						MAX_INDEX_R = MIN_INDEX;
					}
				}
			}
			break;
		case 0x3: //全国地图 National map
			if (CPU_BACKUP[0xBA] == 0x08) {
				Line_Start = 0x280 / 0x20;
				gridy = (CPU_BACKUP[0x9B] * 0x100 + CPU_BACKUP[0x9A] - 0x2000) / 0x20 - Line_Start;
				if (CPU_BACKUP[0xBB] != 0x09) {
					MAX_INDEX_R = MIN_INDEX;
				}
			}
			break;
		case 0x5: //大战场，战斗总结 Battlefield, battle summary
			if (CPU_BACKUP[0xBA] == 0x08) {
				if (CPU_BACKUP[0xAF] == 0x08) {
					Line_Start = 0;
				} else {
					Line_Start = 20; //扫描线开始行数 Scanline start line number
					gridy = 32 - Line_Start; //命名表对扫描线的偏移 Named table offset to scan line
				}
				if (CPU_BACKUP[0xBB] != 0x09) {
					MAX_INDEX_R = MIN_INDEX;
				}
			}
			break;
		case 0x6:
			BASE_VRAM = 0x2400;
			if (CPU_BACKUP[0xBA] == 0x08) {
				Line_Start = 0;
				if (CPU_BACKUP[0xBB] != 0x09) {
					MAX_INDEX_R = MIN_INDEX;
				}
			}
			break;
		case 0x7: //用计
			if (CPU_BACKUP[0xBA] == 0x08) {
				Line_Start = 1 + LINESKIP(0) + LINESKIP(1) + LINESKIP(2) + LINESKIP(3);
				gridy = (CPU_BACKUP[0x9B] * 0x100 + CPU_BACKUP[0x9A] + CPU_BACKUP[0x97] * 0x400 - 0x2000) / 0x20 - Line_Start;
				if (CPU_BACKUP[0xBB] != 0x09) {
					MAX_INDEX_R = MIN_INDEX;
				}
			}
			break;
		case 0x8:
			if (CPU_BACKUP[0xBD] == 0xB3) {
				Line_Start = 14;
				F_SPECIAL = 1;
			}
			break;
		case 0xA:
		case 0xB:
			if (CPU_BACKUP[0xBA] == 0x08) {
				Line_Start = 20;
				gridy = (CPU_BACKUP[0x9B] * 0x100 + CPU_BACKUP[0x9A] + CPU_BACKUP[0x97] * 0x400 - 0x2000) / 0x20 - Line_Start;
				if (CPU_BACKUP[0xBB] != 0x09) {
					MAX_INDEX_R = MIN_INDEX;
				}
			}
			break;
	}

	for (j = Line_Start + gridy; j < Line_End + gridy; j++) //scan memory
	{
		if (j == 20 && CPU_BACKUP[0x60] == 0x05 && CPU_BACKUP[0xCB] == 0x09) {
			gridy += 12;
			j += 12;
		}
		count = 0;
		offset = 0;
		bktileid = 1;
		for (i = 0; i < 32; i++) {
			pos = (i & 0x1F) + (j << 5);

			if (pos > 0x800) {
				pos -= 0x400;
				scrolly = scrollyy + 16;
			} else {
				scrolly = scrollyy;
			}
			tileid = ppu_rd_mem(pos + BASE_VRAM);

			//汉字显示 Chinese character display
			if (tileid < MAX_GROUP) {
				bktileid = tileid;
				if (bktileid < 5) {
					offset = 0;
					count = 0;
				} else {
					offset += 8;
				}
			} else if (((tileid < MAX_INDEX_R) && (F_SPECIAL == 0)) || ((F_SPECIAL == 1) && (tileid > 0xC7))) {
				//访问属性表，获得文字颜色 Visit the property sheet to get the text color
				int ataddr;
				ataddr = 0x03C0 + ((j - (j & 3)) << 1) + (i >> 2);
				unsigned char atbyte = ppu_rd_mem(ataddr);
				unsigned char color = 0;

				switch ((i & 3) + ((j & 3) << 2)) {
					case 0:
					case 1:
					case 4:
					case 5:
						//bit 0,1
						color = (((atbyte & 0x03) << 2) | 0);
						break;
					case 2:
					case 3:
					case 6:
					case 7:
						//bit 2,3
						color = ((atbyte & 0x0C) | 0);
						break;
					case 8:
					case 9:
					case 12:
					case 13:
						//bit 4,5
						color = (((atbyte & 0x30) >> 2) | 0);
						break;
					case 10:
					case 11:
					case 14:
					case 15:
						//bit 6,7
						color = (((atbyte & 0xC0) >> 4) | 0);
						break;
				}
				if (F_SPECIAL == 1) {
					tileid -= 0x90;
					color = 7;
				}

				int x = (i << 3) + (count * 4) - offset;
				int y = ((j - gridy) << 3) - scrolly - 2;
				unsigned int index = txt[bktileid][tileid];
				long maxpos = (RENDER_WIDTH * RENDER_HEIGHT - 8);

				for (int ii = 0; ii < 16; ii++) {
					for (int jj = 0; jj < 16; jj++) {
						long pos2 = (y + ii) * RENDER_WIDTH + jj + x;// + 16;

						if (pos2 < maxpos) {
							if (jj < 8 && (pSan2Font[index * 32 + ii * 2] & (0x80 >> jj)))
								ppu_screen.wr->data[pos2] = mmap_palette.color[color];
							if (jj >= 8 && (pSan2Font[index * 32 + ii * 2 + 1] & (0x80 >> (jj - 8))))
								ppu_screen.wr->data[pos2] = mmap_palette.color[color];
						}
					}
				}
				count++;
			} else {
				count = 0;
				offset = 0;
			}
		}
	}
}
BYTE chinaersan2_init(void) {
	size_t size = chinaersan2.font.size;

	chinaersan2_quit();
	if (size) {
		if ((chinaersan2.font.data = malloc(size)) == NULL) {
			return(EXIT_ERROR);
		}
		chinaersan2.font.size = size;
	}
	return(EXIT_OK);
}
void chinaersan2_quit(void) {
	if (chinaersan2.font.data) {
		free(chinaersan2.font.data);
		chinaersan2.font.data = NULL;
		chinaersan2.font.size = 0;
	}
	chinaersan2.enable = FALSE;
}
