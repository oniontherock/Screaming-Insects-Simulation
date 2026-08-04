#ifndef __PANEL_MANAGER_H__
#define __PANEL_MANAGER_H__

#include <unordered_map>
#include "Panel.hpp"

namespace PanelManager {
	extern std::unordered_map<PanelName, PanelPtr> panels;

	void panelAdd(PanelName panelName, PanelPtr panelInstance);
	Panel& panelGet(PanelName panelName);
	bool panelExists(PanelName panelName);
	void panelDelete(PanelName panelName);
	void panelManagerTerminate();
}

#endif