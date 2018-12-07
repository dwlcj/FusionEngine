#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_GEOMETRY_INSTANCE_COMPONENT_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_GEOMETRY_INSTANCE_COMPONENT_H
/// OptiX
#include <optixu/optixpp_namespace.h>
/// Logging
#include <plog/Log.h>
/// Fusion
#include <optix_scene_hierarchy/component.h>
#include <optix_scene_hierarchy/material_component.h>
#include <optix_scene_hierarchy/geometry_component.h>
/// STL
#include <vector>
namespace tree {
	class GeometryInstanceComponent : public Component<optix::GeometryInstance> {
	public:
		explicit GeometryInstanceComponent() { }
		explicit GeometryInstanceComponent(const optix::GeometryInstance& ginstance);
		const MaterialComponent materialComponent(const std::size_t& idx) const;
		const GeometryComponent geometryComponent() const;
		MaterialComponent& materialComponent(const std::size_t& idx);
		GeometryComponent& geometryComponent();

		void addMaterialComponent(const MaterialComponent& matComp);
		void addGeometryComponent(const GeometryComponent& geomComp);
	private:
		std::vector<MaterialComponent> mMaterialComponents;
		GeometryComponent mGeometryComponent;
	};

	/**
	*	Constructor
	*/
	GeometryInstanceComponent::GeometryInstanceComponent(const optix::GeometryInstance& ginstance) 
		: Component(ginstance)
	{ }

	/**
	*	returns indexed material
	*/
	const MaterialComponent GeometryInstanceComponent::materialComponent(const std::size_t& idx) const {
		if (mMaterialComponents.empty()) {
			LOG_WARNING << "Geometry Instance is empty.";
			return MaterialComponent();
		}
		else if (idx < 0 || idx >= mMaterialComponents.size()) {
			LOG_ERROR << "Index out of range.";
			return MaterialComponent();
		}
		return mMaterialComponents[idx];
	}

	/**
	*	returns geometry instance component's geometry component
	*/
	const GeometryComponent GeometryInstanceComponent::geometryComponent() const {
		return mGeometryComponent;
	}

	/**
	*	returns ginstance component's material (const)
	*/
	MaterialComponent& GeometryInstanceComponent::materialComponent(const std::size_t& idx) {
		if (mMaterialComponents.empty()) {
			LOG_WARNING << "Geometry Instance is empty.";
			return MaterialComponent();
		}
		else if (idx < 0 || idx >= mMaterialComponents.size()) {
			LOG_ERROR << "Index out of range.";
			return MaterialComponent();
		}
		return mMaterialComponents[idx];
	}

	/**
	*	returns geometry instance component's geometry component
	*/
	GeometryComponent& GeometryInstanceComponent::geometryComponent() {
		return mGeometryComponent;
	}

	/**
	*	Adds material component
	*/
	void GeometryInstanceComponent::addMaterialComponent(const MaterialComponent& matComp) {
		mMaterialComponents.emplace_back(matComp);
		this->get()->addMaterial(mMaterialComponents.back().get());
	}

	/**
	*	sets geometry component
	*/
	void GeometryInstanceComponent::addGeometryComponent(const GeometryComponent& geomComp) {
		mGeometryComponent = geomComp;
		this->get()->setGeometry(mGeometryComponent.get());
	}
}	//	!namespace tree
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_GEOMETRY_INSTANCE_COMPONENT_H

