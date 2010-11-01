/*
 * Copyright 2010 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "MonthlyCostsState.h"
#include <sstream>
#include "../Engine/Game.h"
#include "../Resource/ResourcePack.h"
#include "../Engine/Language.h"
#include "../Resource/LangString.h"
#include "../Engine/Font.h"
#include "../Engine/Palette.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Base.h"
#include "../Savegame/SavedGame.h"
#include "../Ruleset/Ruleset.h"
#include "../Ruleset/RuleCraft.h"

/**
 * Initializes all the elements in the Monthly Costs screen.
 * @param game Pointer to the core game.
 * @param base Pointer to the base to get info from.
 */
MonthlyCostsState::MonthlyCostsState(Game *game, Base *base) : State(game), _base(base)
{
	// Create objects
	_window = new Window(this, 320, 200, 0, 0);
	_btnOk = new TextButton(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 300, 20, 10, 170);
	_txtTitle = new Text(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 310, 16, 5, 12);
	_txtCost = new Text(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 80, 9, 115, 32);
	_txtQuantity = new Text(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 55, 9, 195, 32);
	_txtTotal = new Text(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 60, 9, 250, 32);
	_txtRental = new Text(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 80, 9, 10, 48);
	_txtSalaries = new Text(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 80, 9, 10, 80);
	_txtIncome = new Text(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 100, 9, 10, 136);
	_lstCrafts = new TextList(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 300, 20, 10, 56);
	_lstSalaries = new TextList(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 300, 30, 10, 88);
	_lstMaintenance = new TextList(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 300, 9, 10, 120);
	_lstTotal = new TextList(_game->getResourcePack()->getFont("BIGLETS.DAT"), _game->getResourcePack()->getFont("SMALLSET.DAT"), 100, 9, 205, 136);
	
	// Set palette
	_game->setPalette(_game->getResourcePack()->getPalette("BACKPALS.DAT")->getColors(Palette::blockOffset(6)), Palette::backPos, 16);

	add(_window);
	add(_btnOk);
	add(_txtTitle);
	add(_txtCost);
	add(_txtQuantity);
	add(_txtTotal);
	add(_txtRental);
	add(_txtSalaries);
	add(_txtIncome);
	add(_lstCrafts);
	add(_lstSalaries);
	add(_lstMaintenance);
	add(_lstTotal);

	// Set up objects
	_window->setColor(Palette::blockOffset(15)+4);
	_window->setBackground(_game->getResourcePack()->getSurface("BACK13.SCR"));

	_btnOk->setColor(Palette::blockOffset(15)+4);
	_btnOk->setText(_game->getResourcePack()->getLanguage()->getString(STR_OK));
	_btnOk->onMouseClick((ActionHandler)&MonthlyCostsState::btnOkClick);

	_txtTitle->setColor(Palette::blockOffset(15)+1);
	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setText(_game->getResourcePack()->getLanguage()->getString(STR_MONTHLY_COSTS));
	
	_txtCost->setColor(Palette::blockOffset(15)+1);
	_txtCost->setText(_game->getResourcePack()->getLanguage()->getString(STR_COST_PER_UNIT));

	_txtQuantity->setColor(Palette::blockOffset(15)+1);
	_txtQuantity->setText(_game->getResourcePack()->getLanguage()->getString(STR_QUANTITY));

	_txtTotal->setColor(Palette::blockOffset(15)+1);
	_txtTotal->setText(_game->getResourcePack()->getLanguage()->getString(STR_TOTAL));

	_txtRental->setColor(Palette::blockOffset(15)+1);
	_txtRental->setText(_game->getResourcePack()->getLanguage()->getString(STR_CRAFT_RENTAL));

	_txtSalaries->setColor(Palette::blockOffset(15)+1);
	_txtSalaries->setText(_game->getResourcePack()->getLanguage()->getString(STR_SALARIES));

	_txtIncome->setColor(Palette::blockOffset(13)+10);
	std::stringstream ss;
	ss << _game->getResourcePack()->getLanguage()->getString(STR_INCOME) << "=" << Text::formatFunding(_game->getSavedGame()->getCountryFunding());
	_txtIncome->setText(ss.str());

	_lstCrafts->setColor(Palette::blockOffset(13)+10);
	_lstCrafts->setColumns(4, 125, 70, 45, 60);
	_lstCrafts->setDot(true);

	std::stringstream ss2;
	ss2 << _base->getCraftCount(STR_SKYRANGER);
	_lstCrafts->addRow(4, _game->getResourcePack()->getLanguage()->getString(STR_SKYRANGER).c_str(), Text::formatFunding(_game->getRuleset()->getCraft(STR_SKYRANGER)->getMonthlyFee()).c_str(), ss2.str().c_str(), Text::formatFunding(_base->getCraftCount(STR_SKYRANGER) * _game->getRuleset()->getCraft(STR_SKYRANGER)->getMonthlyFee()).c_str());
	std::stringstream ss3;
	ss3 << _base->getCraftCount(STR_INTERCEPTOR);
	_lstCrafts->addRow(4, _game->getResourcePack()->getLanguage()->getString(STR_INTERCEPTOR).c_str(), Text::formatFunding(_game->getRuleset()->getCraft(STR_INTERCEPTOR)->getMonthlyFee()).c_str(), ss3.str().c_str(), Text::formatFunding(_base->getCraftCount(STR_INTERCEPTOR) * _game->getRuleset()->getCraft(STR_INTERCEPTOR)->getMonthlyFee()).c_str());

	_lstSalaries->setColor(Palette::blockOffset(13)+10);
	_lstSalaries->setColumns(4, 125, 70, 45, 60);
	_lstSalaries->setDot(true);

	std::stringstream ss4;
	ss4 << _base->getSoldiers()->size();
	_lstSalaries->addRow(4, _game->getResourcePack()->getLanguage()->getString(STR_SOLDIERS).c_str(), Text::formatFunding(20000).c_str(), ss4.str().c_str(), Text::formatFunding(_base->getSoldiers()->size() * 20000).c_str());
	std::stringstream ss5;
	ss5 << _base->getTotalEngineers();
	_lstSalaries->addRow(4, _game->getResourcePack()->getLanguage()->getString(STR_ENGINEERS).c_str(), Text::formatFunding(25000).c_str(), ss5.str().c_str(), Text::formatFunding(_base->getTotalEngineers() * 25000).c_str());
	std::stringstream ss6;
	ss6 << _base->getTotalScientists();
	_lstSalaries->addRow(4, _game->getResourcePack()->getLanguage()->getString(STR_SCIENTISTS).c_str(), Text::formatFunding(30000).c_str(), ss6.str().c_str(), Text::formatFunding(_base->getTotalScientists() * 30000).c_str());

	_lstMaintenance->setColor(Palette::blockOffset(13)+10);
	_lstMaintenance->setColumns(2, 240, 60);
	_lstMaintenance->setDot(true);
	_lstMaintenance->addRow(2, _game->getResourcePack()->getLanguage()->getString(STR_BASE_MAINTENANCE).c_str(), Text::formatFunding(_base->getFacilityMaintenance()).c_str());
	_lstMaintenance->getCell(0, 0)->setColor(Palette::blockOffset(15)+1);
	_lstMaintenance->draw();

	_lstTotal->setColor(Palette::blockOffset(13));
	_lstTotal->setColumns(2, 45, 55);
	_lstTotal->setDot(true);
	_lstTotal->addRow(2, _game->getResourcePack()->getLanguage()->getString(STR_TOTAL).c_str(), Text::formatFunding(_base->getMonthlyMaintenace()).c_str());
}

/**
 *
 */
MonthlyCostsState::~MonthlyCostsState()
{
	
}

/**
 * Returns to the previous screen.
 * @param action Pointer to an action.
 */
void MonthlyCostsState::btnOkClick(Action *action)
{
	_game->popState();
}
