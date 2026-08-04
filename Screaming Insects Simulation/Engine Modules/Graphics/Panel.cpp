#include "Panel.hpp"
#include "../Input/InputInterface.hpp"

Panel::Panel(PanelRect _screenRect, PanelRect _viewRect, sf::Color _clearColor) :
	screenRect(_screenRect), viewRect(_viewRect), clearColor(_clearColor), viewRectSizeOriginal(_viewRect.size), texture(sf::Vector2u(screenRect.size))
{
	viewCreate();
}

void Panel::viewCreate() {
	view = PanelView(viewRect);
}
inline void Panel::vertexArrayDraw(const sf::VertexArray& vertexArray, const sf::RenderStates& states) {
	texture.draw(vertexArray, states);
}
void Panel::panelDrawObjects() {
	panelUpdate();
}
void Panel::panelRender(sf::RenderWindow& renderWindowMain) {
	viewUpdate();
	texture.display();

	sf::Sprite sprite(texture.getTexture());
	sprite.setPosition(screenRect.position);

	renderWindowMain.draw(sprite);
}
void Panel::panelClear() {
	texture.clear(clearColor);
}

void Panel::viewZoomScale(float zoomFactor) {
	viewZoom *= zoomFactor;

	viewRect.position += (viewRect.size - (viewRect.size * zoomFactor)) / 2.f;
}
void Panel::viewZoomSet(float zoomValue) {
	viewZoomScale(zoomValue / viewZoom);
}
float Panel::viewZoomGet() const {
	return viewZoom;
}

void Panel::viewMove(sf::Vector2f moveVec) {
	viewRect.position += moveVec;
}
void Panel::viewMove(float moveX, float moveY) {
	viewMove(sf::Vector2f(moveX, moveY));
}
void Panel::viewSetRotation(float angle) {
	viewRotation = angle;
}
void Panel::viewRotate(float angle) {
	viewRotation += angle;
}

void Panel::viewUpdate() {

	sf::Vector2f newSize = viewRectSizeOriginal * viewZoom;

	viewRect.size = newSize;

	view.setCenter(viewRect.getCenter());
	view.setSize(viewRect.size);
	view.setRotation(sf::radians(viewRotation));
	texture.setView(view);
}

const PanelView& Panel::viewGet() {
	return view;
}

float Panel::viewAspectRatioGet() {
	return view.getSize().x / view.getSize().y;
}

sf::Vector2f Panel::panelMousePositionGet() {
	sf::Vector2f mousePos = sf::Vector2f(InputInterface::windowMousePositionGet());

	return mousePos - screenRect.position;
}

sf::Vector2f Panel::viewMousePositionGet() {
	sf::Vector2f mousePos = panelMousePositionGet();

	sf::Vector2f ratio = viewRect.size.componentWiseDiv(screenRect.size);
	mousePos += viewRect.position;

	sf::Vector2f axis = mousePos - viewRect.position;

	mousePos = viewRect.position + axis.componentWiseMul(ratio);

	return mousePos;
}

uint16_t Panel::allocatedPanelIdsTotal = 0;

