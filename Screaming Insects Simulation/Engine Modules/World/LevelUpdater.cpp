#include "LevelUpdater.hpp"

#include "../ECS/Entities/EntityManager.hpp"


void LevelUpdater::levelsUpdate() {
	for (uint32_t levelActiveInd = 0; levelActiveInd < levelsActiveVector.size(); levelActiveInd++) {

		LevelPosition levelActivePosition = levelsActiveVector[levelActiveInd];

		BaseLevel* levelActiveCur = LevelGrid<BaseLevel>::levelGet(levelActivePosition);
		


		if (levelActiveCur->isUpdating) {
			for (uint32_t entityIdInd = 0; entityIdInd < levelActiveCur->entities.size(); entityIdInd++) {
				EntityManager::entityUpdate(levelActiveCur->entities[entityIdInd]);
			}

			if (!levelActiveCur->entitiesObservation.empty()) {
				levelActiveCur->entitiesObservedUpdate();

				for (uint32_t entityIdInd = 0; entityIdInd < levelActiveCur->entitiesObserved.size(); entityIdInd++) {
					EntityManager::entityUpdate(levelActiveCur->entitiesObserved[entityIdInd]);
				}
			}
		}
	}
}

