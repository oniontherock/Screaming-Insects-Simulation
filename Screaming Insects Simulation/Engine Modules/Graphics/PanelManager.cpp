#include "PanelManager.hpp"

std::unordered_map<PanelName, PanelPtr> PanelManager::panels{};

void PanelManager::panelAdd(PanelName panelName, PanelPtr panelInstance) {
	panels.insert(std::move(std::pair<PanelName, PanelPtr>(panelName, std::move(panelInstance))));
}

Panel& PanelManager::panelGet(PanelName panelName) {
	return *panels[panelName].get();
}

bool PanelManager::panelExists(PanelName panelName) {
	return panels.count(panelName);
}

void PanelManager::panelDelete(PanelName panelName) {
	panels.erase(panelName);
}

void PanelManager::panelManagerTerminate() {
	panels.clear();
}
