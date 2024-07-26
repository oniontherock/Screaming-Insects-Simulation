#ifndef __PANELS_H__
#define __PANELS_H__

#include "Graphics/Panel.hpp"

enum PanelTypes : uint16_t {
	GameView,
};

struct PanelGameView : public Panel {
	using Panel::Panel;

	enum Modes {
		Normal,
		Screams,
		Hearing,
	};

	Modes mode = Normal;

	void panelUpdate() final;
	
private:
	void checkModeChange();
	void drawInsects();
	void drawScreams();
	void drawHearing();
};


#endif