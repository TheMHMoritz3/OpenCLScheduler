/*
Embedded Systems Project 2019
Copyright (C) 2019  Moritz Herzog, Philip Lersch

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CanManager.h"
#include <exception>
#include "RandomNumberGenerator.h"

using namespace std;
using namespace CAN;

CanManager::CanManager() {
	SimulationTrigger = false;
	Count = 100;
	//SimCar=new ValueGen::SimCar();
}

void CanManager::create(CAN::CanID id, int count) {
	Count = count;
	CanAccessor* accessor = new CanAccessor(id, count);
	accessor->startCollectingData();
	CanThreadMap.insert(pair<int, CanAccessor*>((int)id, accessor));
}

void CanManager::create(std::vector<CAN::CanID> ids, int count) {
	Count = count;
	for (CanID id : ids) {
		CanAccessor* accessor = new CanAccessor(id, count);
		accessor->startCollectingData();
		CanThreadMap.insert(pair<int, CanAccessor*>((int)id, accessor));
	}
}

double CanManager::getSamplingRate(CAN::CanID id) {
	switch (id) {
	case CanID::WheelFrontRight:
	case CanID::WheelFrontLeft:
	case CanID::WheelRearLeft:
	case CanID::WheelRearRight:
	case CanID::AccelerationLateral:
	case CanID::AccelerationLongitudinal:
	case CanID::Temperature:
		return 0.050;
	case CanID::BatteryVoltage:
		return 0.2;
	default:
		return 0.0;
	}
}

vector<uint32_t*> CanManager::getData(CAN::CanID id) {
	return CanThreadMap.at((int)id)->getData();
}

std::vector<void*> CanManager::getValuesFromSimulation(CAN::CanID id, int count)
{
	switch (id) {
	case WheelFrontRight: 
	case WheelFrontLeft:
	case WheelRearLeft: 
	case WheelRearRight:
		return RandomNumberGenerator::generateRandomNumbers(count,500,2000);
	case BatteryVoltage:
		return RandomNumberGenerator::generateRandomNumbers(count, 300, 600);
	case AccelerationLongitudinal:
	case AccelerationLateral:
		return RandomNumberGenerator::generateRandomNumbers(count, 2000, 2100);
	case Temperature:
		return RandomNumberGenerator::generateRandomNumbers(count, 0, 400);
	default:;
	}
}
