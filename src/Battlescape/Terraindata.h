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
#ifndef OPENXCOM_TERRAINDATA_H
#define OPENXCOM_TERRAINDATA_H

#include <vector>
#include <string>

class TerrainObject;

class Terraindata
{
private:
	std::vector<TerrainObject*> _terrainObjects;
	std::string _name;
public:
	/// create a set of MCD records under a certain name
	Terraindata(const std::string &name);
	~Terraindata();
	/// get pointer to the set of MCD records
	std::vector<TerrainObject*> *getTerrainObjects();
	/// load the set of MCD records from an MCD file
	void load(const std::string &filename);
};

#endif
