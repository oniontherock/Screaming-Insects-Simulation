#include "ComponentTemplateManager.hpp"
#include "../../Auxiliary/ConsoleHandler.hpp"


std::map<ComponentTemplateName, ComponentTemplate> ComponentTemplateManager::componentTemplates{};

bool ComponentTemplateManager::componentTemplateExists(ComponentTemplateName templateName) {
	return componentTemplates.find(templateName) != componentTemplates.end();
}

bool ComponentTemplateManager::componentTemplatesErrorIfNameTaken(ComponentTemplateName templateName) {
	if (componentTemplateExists(templateName)) {
		ConsoleHandler::consolePrintErr("Template name taken: \"" + std::string(templateName) + std::string("\""));
		return true;
	}
	return false;
}

void ComponentTemplateManager::componentTemplateAdd(ComponentTemplateName templateName, ComponentTemplate templateComponentsVector) {
	if (templateComponentsVector.size() < EntityComponents::totalComponents) {
		ConsoleHandler::consolePrintErr("Attempted to add template with fewer components than the total amount of components: \"" + std::string(templateName) + std::string("\""));
		return;
	}
	else if (componentTemplatesErrorIfNameTaken(templateName)) {
		return;
	}

	componentTemplates.insert({ templateName, std::move(templateComponentsVector) });
}

void ComponentTemplateManager::componentTemplateAdd(ComponentTemplateName templateName, ComponentTemplatePairVector templateComponentsMap) {

	if (componentTemplatesErrorIfNameTaken(templateName)) {
		return;
	}

	ComponentTemplate templateComponentsVector(EntityComponents::totalComponents);

	for (uint16_t i = 0; i < templateComponentsMap.size(); i++) {
		templateComponentsVector[templateComponentsMap[i].first] =
			Duplicatable::duplicateAndConvertToType<EntityComponents::Component>(templateComponentsMap[i].second.get());
	}

	componentTemplates.insert({ templateName, std::move(templateComponentsVector) });
}

void ComponentTemplateManager::componentTemplateAdd(ComponentTemplateName templateName, std::vector<ComponentTemplateName> chunkNames, ComponentTemplatePairVector templateComponentsMap) {
	if (componentTemplatesErrorIfNameTaken(templateName)) {
		return;
	}

	ComponentTemplate templateComponentsVector(EntityComponents::totalComponents);

	for (uint16_t i = 0; i < chunkNames.size(); i++) {

		auto& chunkName = chunkNames[i];

		if (!componentTemplateExists(chunkName)) {
			ConsoleHandler::consolePrintErr("Template chunk name \"" + std::string(chunkName) + "\"" + " does not exist!");
		}
		else {
			componentTemplateApplyToComponentVector(chunkName, templateComponentsVector, TemplateApplicationType::Merge);
		}
	}

	for (uint16_t i = 0; i < templateComponentsMap.size(); i++) {
		templateComponentsVector[templateComponentsMap[i].first] =
			Duplicatable::duplicateAndConvertToType<EntityComponents::Component>(templateComponentsMap[i].second.get());
	}

	componentTemplates.insert({ templateName, std::move(templateComponentsVector) });
}

void ComponentTemplateManager::componentTemplateApplyToComponentVector(ComponentTemplateName templateName, std::vector<ComponentUniquePtr>& componentsVector, TemplateApplicationType applicationType) {

	if (!componentTemplateExists(templateName)) {
		ConsoleHandler::consolePrintErr("Template name \"" + std::string(templateName) + std::string("\"") + " does not exist!");
		return;
	}

	if (applicationType == Overwrite) {
		for (EntityComponents::ComponentTypeID i = 0; i < EntityComponents::totalComponents; i++) {
			componentsVector[i].reset();
		}
	}

	for (EntityComponents::ComponentTypeID i = 0; i < componentTemplates[templateName].size(); i++) {

		// go to next iteration if the current component is a nullptr
		if (!static_cast<bool>(componentTemplates[templateName][i])) continue;

		// we use no overwrite regardless of the application type because if the application type IS overwrite,
		// then all the components were terminated, and so adding a component with no overwrite is the same as with overwrite
		componentsVector[i] = ComponentUniquePtr(static_cast<EntityComponents::Component*>(Duplicatable::duplicateAndGetRaw(componentTemplates[templateName][i].get())));
	}
}

void ComponentTemplateManager::componentTemplateApplyToEntity(ComponentTemplateName templateName, Entity& entity, TemplateApplicationType applicationType) {
	componentTemplateApplyToComponentVector(templateName, entity.componentsVector, applicationType);
}
void ComponentTemplateManager::componentTemplateTerminate(ComponentTemplateName templateName) {
	componentTemplates[templateName].clear();
}
void ComponentTemplateManager::componentTemplatesAllTerminate() {
	componentTemplates.clear();
}

