#ifndef __PANEL_H__
#define __PANEL_H__

#include "SFML/Graphics.hpp"

typedef sf::View PanelView;
typedef sf::RenderTexture PanelScreenTexture;
typedef sf::FloatRect PanelRect;

enum PanelTypes : uint16_t;

typedef PanelTypes PanelName;

struct Panel {

	Panel() = default;
	Panel(PanelRect _screenRect, PanelRect _viewRect, sf::Color _clearColor);
	virtual ~Panel() {};

	void viewCreate();

	// DEPRECATED: have the panel draw directly to the texture instead
	template <typename T>
	inline void objectDraw(const T& drawableObject) {
		texture.draw(drawableObject);
	}
	// DEPRECATED: have the panel draw directly to the texture instead
	template <typename T>
	inline void objectDraw(const T& drawableObject, sf::StencilMode stencilMode) {
		texture.draw(drawableObject, stencilMode);
	}
	// DEPRECATED: have the panel draw directly to the texture instead
	inline void objectDraw(const sf::Drawable& drawableObject, sf::Shader& shader) {
		texture.draw(drawableObject, &shader);
	}
	void vertexArrayDraw(const sf::VertexArray& vertexArray, const sf::RenderStates& states);

	void viewZoomScale(float zoomFactor);
	void viewZoomSet(float zoomValue);
	float viewZoomGet() const;
	void viewMove(float moveX, float moveY);
	void viewMove(sf::Vector2f moveVec);

	void viewSetRotation(float angle);
	void viewRotate(float angle);

	void panelDrawObjects();
	void panelRender(sf::RenderWindow& renderWindowMain);
	// clears the panel with a transparent color
	void panelClear();

	float viewAspectRatioGet();

	// gets the mouse position relative to the panel's window position
	sf::Vector2f panelMousePositionGet();
	// gets the mouse position relative to the panel's view position
	sf::Vector2f viewMousePositionGet();

	const PanelView& viewGet();

	void viewUpdate();

	// the Panel's position and dimensions on the screen
	PanelRect screenRect{ sf::Vector2f(0, 0), sf::Vector2f(0, 0) };

	// the Panel's position and dimensions in it's drawn environment.
	// as an example, if an object is drawn at 250x250, and the viewRect is 0x0x500x500,
	// then the object will be drawn in the center of the screenRect
	PanelRect viewRect{ sf::Vector2f(0, 0), sf::Vector2f(0, 0) };
	float viewRotation = 0.f;

	// color used when clearing panel
	sf::Color clearColor;
protected:
	// the panel's texture, this is what is actual drawn on, and it's rendered to the main game window
	PanelScreenTexture texture;
	// the panel's view,
	PanelView view;

	sf::Vector2f viewRectSizeOriginal;

	float viewZoom = 1.f;
private:

	virtual void panelUpdate() = 0;

	static uint16_t allocatedPanelIdsTotal;
};

typedef std::unique_ptr<Panel> PanelPtr;

#endif