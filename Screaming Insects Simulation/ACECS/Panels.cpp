#include "Panels.hpp"

#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "Graphics/PanelManager.hpp"
#include "Input.hpp"
#include "../Include/Common/TimeHandler.hpp"


void PanelGameView::panelUpdate() {

	checkModeChange();

	float zoomAmount = 1.f - (InputInterface::mouseScrollAmountGet() / 32.f);

	auto& gameViewPanel = PanelManager::panelGet(PanelTypes::GameView);

	gameViewPanel.viewZoomScale(zoomAmount);

	float panelMoveAmountX = float(InputInterface::inputGetActive("Move Panel Right") - InputInterface::inputGetActive("Move Panel Left"));
	float panelMoveAmountY = float(InputInterface::inputGetActive("Move Panel Down") - InputInterface::inputGetActive("Move Panel Up"));

	float panelMoveSpeed = float((240.f * gameViewPanel.viewZoomGet()) * TimeHandler::deltaSimulatedGet());

	panelMoveAmountX *= panelMoveSpeed;
	panelMoveAmountY *= panelMoveSpeed;

	gameViewPanel.viewMove(panelMoveAmountX, panelMoveAmountY);


	if (mode == Normal) {
		drawTargets();
		drawInsects();
	}
	if (mode == Screams) {
		drawScreams();
	}
	if (mode == Hearing) {
		drawHearing();
	}
}

void PanelGameView::checkModeChange() {

	if (InputInterface::inputGetActive("Toggle Scream View")) {
		if (mode == Screams) {
			mode = Normal;
		}
		else {
			mode = Screams;
		}
	}
	else if (InputInterface::inputGetActive("Toggle Hearing View")) {
		if (mode == Hearing) {
			mode = Normal;
		}
		else {
			mode = Hearing;
		}
	}
}
void PanelGameView::drawTargets() {
	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);

	for (uint32_t i = 0; i < gameLevel->targets.size(); i++) {
		Entity& entity = EntityManager::entitiesVector[gameLevel->targets[i]];

		auto* entityPositionComponent = entity.entityComponentGet<EntityComponents::ComponentPosition>();
		if (entityPositionComponent && entity.entityComponentHas<EntityComponents::ComponentObjectGridCellPopulator>()) {
			sf::CircleShape circle(entity.entityComponentGet<EntityComponents::ComponentObjectGridCellPopulator>()->popRadius / 2);

			circle.setOrigin(circle.getRadius(), circle.getRadius());
			circle.setPosition(entityPositionComponent->x, entityPositionComponent->y);
			circle.setFillColor(sf::Color::Blue);

			objectDraw(circle);
		}
	}
}

void PanelGameView::drawInsects() {
	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);

	sf::VertexArray vertexArray(sf::Points, gameLevel->insects.size());

	for (uint32_t i = 0; i < gameLevel->insects.size(); i++) {
		Entity& entity = EntityManager::entitiesVector[gameLevel->insects[i]];

		auto* entityPositionComponent = entity.entityComponentGet<EntityComponents::ComponentPosition>();

		if (entityPositionComponent) {
			vertexArray[i] = sf::Vector2f(entityPositionComponent->x, entityPositionComponent->y);
		}
	}

	objectDraw(vertexArray);
}
void PanelGameView::drawScreams() {
	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);

	sf::VertexArray vertexArray(sf::Lines, gameLevel->screamConnections.size() * 2);

	for (uint32_t i = 0; i < gameLevel->screamConnections.size(); i++) {

		ScreamConnection connectionCur = gameLevel->screamConnections[i];

		uint32_t lineInd = i * 2;

		vertexArray[lineInd + 0] = std::get<0>(connectionCur);
		vertexArray[lineInd + 1] = std::get<1>(connectionCur);

		sf::Color homeColor = sf::Color(255, 255, 0, 255 / 8);
		sf::Color foodColor = sf::Color(0, 255, 255, 255 / 8);

		sf::Color screamColor = std::get<2>(connectionCur) == TargetType::Home ? homeColor : foodColor;

		vertexArray[lineInd + 0].color = screamColor;
		vertexArray[lineInd + 1].color = screamColor;
	}

	objectDraw(vertexArray);
}
void PanelGameView::drawHearing() {
	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);

	sf::VertexArray vertexArray(sf::Lines, gameLevel->acceptedScreamConnections.size() * 2);

	for (uint32_t i = 0; i < gameLevel->acceptedScreamConnections.size(); i++) {

		ScreamConnection connectionCur = gameLevel->acceptedScreamConnections[i];

		uint32_t lineInd = i * 2;

		vertexArray[lineInd + 0] = std::get<0>(connectionCur);
		vertexArray[lineInd + 1] = std::get<1>(connectionCur);

		sf::Color homeColor = sf::Color(255, 255, 0, 255 / 4);
		sf::Color foodColor = sf::Color(0, 255, 255, 255 / 4);

		sf::Color screamColor = std::get<2>(connectionCur) == TargetType::Home ? homeColor : foodColor;

		vertexArray[lineInd + 0].color = screamColor;
		vertexArray[lineInd + 1].color = screamColor;
	}

	objectDraw(vertexArray);
}

