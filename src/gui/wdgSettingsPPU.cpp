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

#include "wdgSettingsPPU.moc"
#include "mainWindow.hpp"
#include "emu_thread.h"
#include "tas.h"
#include "ppu.h"
#include "conf.h"
#include "gui.h"
#include "settings.h"

wdgSettingsPPU::wdgSettingsPPU(QWidget *parent) : QWidget(parent) {
	setupUi(this);

	setFocusProxy(checkBox_Hide_Sprites);

	spinBox_VB_Slines->setRange(0, 1000);
	spinBox_Postrender_Slines->setRange(0, 1000);

	connect(checkBox_Hide_Sprites, SIGNAL(clicked(bool)), this, SLOT(s_hide_sprites(bool)));
	connect(checkBox_Hide_Background, SIGNAL(clicked(bool)), this, SLOT(s_hide_background(bool)));
	connect(checkBox_Unlimited_Sprites, SIGNAL(clicked(bool)), this, SLOT(s_unlimited_sprites(bool)));

	connect(checkBox_PPU_Overclock, SIGNAL(clicked(bool)), this, SLOT(s_ppu_overclock(bool)));
	connect(checkBox_Disable_DMC_Control, SIGNAL(clicked(bool)), this, SLOT(s_disable_dmc_control(bool)));

	connect(spinBox_VB_Slines, SIGNAL(valueChanged(int)), this, SLOT(s_overclock_vb_slines(int)));
	connect(spinBox_Postrender_Slines, SIGNAL(valueChanged(int)), this, SLOT(s_overclock_pr_slines(int)));

	connect(pushButton_Reset_Lag_Counter, SIGNAL(clicked(bool)), this, SLOT(s_lag_counter_reset(bool)));

	installEventFilter(this);
}
wdgSettingsPPU::~wdgSettingsPPU() {}

void wdgSettingsPPU::changeEvent(QEvent *event) {
	if (event->type() == QEvent::LanguageChange) {
		Ui::wdgSettingsPPU::retranslateUi(this);
	} else {
		QWidget::changeEvent(event);
	}
}
void wdgSettingsPPU::showEvent(QShowEvent *event) {
	int dim = fontMetrics().height();

	icon_Sprites_and_background->setPixmap(QIcon(":/icon/icons/background.svg").pixmap(dim, dim));
	icon_PPU_Overclock->setPixmap(QIcon(":/icon/icons/speedometer.svg").pixmap(dim, dim));

	QWidget::showEvent(event);
}

void wdgSettingsPPU::update_widget(void) {
	checkBox_Hide_Sprites->setChecked(cfg->hide_sprites);
	checkBox_Hide_Background->setChecked(cfg->hide_background);
	checkBox_Unlimited_Sprites->setChecked(cfg->unlimited_sprites);
	checkBox_PPU_Overclock->setChecked(cfg->ppu_overclock);

	checkBox_Disable_DMC_Control->setChecked(cfg->ppu_overclock_dmc_control_disabled);
	checkBox_Disable_DMC_Control->setEnabled(cfg->ppu_overclock);

	label_VB_Slines->setEnabled(cfg->ppu_overclock);

	spinBox_VB_Slines->setEnabled(cfg->ppu_overclock);
	qtHelper::spinbox_set_value(spinBox_VB_Slines, cfg->extra_vb_scanlines);

	label_Postrender_Slines->setEnabled(cfg->ppu_overclock);

	spinBox_Postrender_Slines->setEnabled(cfg->ppu_overclock);
	qtHelper::spinbox_set_value(spinBox_Postrender_Slines, cfg->extra_pr_scanlines);

	lag_counter_update();
}
void wdgSettingsPPU::lag_counter_update(void) {
	lineEdit_Lag_Counter->setText(QString("%1").arg(tas.total_lag_frames));
}

void wdgSettingsPPU::s_hide_sprites(UNUSED(bool checked)) {
	emu_thread_pause();
	cfg->hide_sprites = !cfg->hide_sprites;
	emu_thread_continue();
	update_widget();
}
void wdgSettingsPPU::s_hide_background(UNUSED(bool checked)) {
	emu_thread_pause();
	cfg->hide_background = !cfg->hide_background;
	emu_thread_continue();
	update_widget();
}
void wdgSettingsPPU::s_unlimited_sprites(UNUSED(bool checked)) {
	emu_thread_pause();
	cfg->unlimited_sprites = !cfg->unlimited_sprites;
	emu_thread_continue();
	update_widget();
}
void wdgSettingsPPU::s_ppu_overclock(UNUSED(bool checked)) {
	emu_thread_pause();
	cfg->ppu_overclock = !cfg->ppu_overclock;
	ppu_overclock(TRUE);
	emu_thread_continue();
	settings_pgs_save();
	update_widget();
}
void wdgSettingsPPU::s_disable_dmc_control(UNUSED(bool checked)) {
	emu_thread_pause();
	cfg->ppu_overclock_dmc_control_disabled = !cfg->ppu_overclock_dmc_control_disabled;
	emu_thread_continue();
	settings_pgs_save();
	update_widget();
}
void wdgSettingsPPU::s_overclock_vb_slines(int i) {
	emu_thread_pause();
	cfg->extra_vb_scanlines = i;
	ppu_overclock(FALSE);
	emu_thread_continue();
	settings_pgs_save();
	update_widget();
}
void wdgSettingsPPU::s_overclock_pr_slines(int i) {
	emu_thread_pause();
	cfg->extra_pr_scanlines = i;
	ppu_overclock(FALSE);
	emu_thread_continue();
	settings_pgs_save();
	update_widget();
}
void wdgSettingsPPU::s_lag_counter_reset(UNUSED(bool checked)) {
	emu_thread_pause();
	tas.total_lag_frames = 0;
	emu_thread_continue();
	lag_counter_update();
	update_widget();
}
