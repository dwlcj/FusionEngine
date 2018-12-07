#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_ROOT_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_ROOT_H
/// OptiX
#include <optixu/optixpp_namespace.h>
/// Fusion
#include <optix_scene_hierarchy/transform_component.h>
#include <optix_scene_hierarchy/geometry_group_component.h>
#include <optix_scene_hierarchy/geometry_instance_component.h>
#include <optix_scene_hierarchy/material_component.h>
#include <optix_scene_hierarchy/geometry_component.h>
/// STL
#include <vector>
#include <memory>
namespace tree {
	class SceneRoot {
	public:
		explicit SceneRoot() { }
		explicit SceneRoot(const TransformComponent& tranComp);
		const GeometryComponent geometryComponent() const;
		const MaterialComponent materialComponent(const std::size_t& idx) const;
		const GeometryInstanceComponent ginstanceComponent() const;
		const GeometryGroupComponent ggroupComponent() const;
		const TransformComponent transformComponent() const;

		GeometryComponent& geometryComponent();
		MaterialComponent& materialComponent(const std::size_t& idx);
		GeometryInstanceComponent& ginstanceComponent();
		GeometryGroupComponent& ggroupComponent();
		TransformComponent& transformComponent();
		void addTransformComp(const TransformComponent& transComp);
		void addGeometryGroupComp(const GeometryGroupComponent& ggroupComp);
		void addGeometryInstanceComponent(const GeometryInstanceComponent& ginstanceComp);
		void addMaterialComponent(const MaterialComponent& matComp);
		void addGeometryComponent(const GeometryComponent& geomComp);
	private:
		TransformComponent mTransformComponent;
	};

	/**
	*	Constructor
	*/
	SceneRoot::SceneRoot(const TransformComponent& transComp) 
		: mTransformComponent(transComp)
	{ }

	/**
	*	adds transform component
	*/
	void SceneRoot::addTransformComp(const TransformComponent& transComp) {
		// TODO: do sthing to check if root already has transform
		mTransformComponent = transComp;
	}

	/**
	*	add geometry group component
	*/
	void SceneRoot::addGeometryGroupComp(const GeometryGroupComponent& ggroupComp) {
		mTransformComponent.addGeometryGroupComponent(ggroupComp);
	}

	/**
	*	add geometry instance component
	*/
	void SceneRoot::addGeometryInstanceComponent(const GeometryInstanceComponent& ginstanceComp) {
		mTransformComponent.ggroupComponent().setGinstanceComponent(ginstanceComp);
	}

	/**
	*	add material component
	*/
	void SceneRoot::addMaterialComponent(const MaterialComponent& matComp) {
		mTransformComponent.ggroupComponent().ginstanceComp().addMaterialComponent(matComp);
	}

	/**
	*	adds geometry component
	*/
	void SceneRoot::addGeometryComponent(const GeometryComponent& geomComp) {
		mTransformComponent.ggroupComponent().ginstanceComp().addGeometryComponent(geomComp);
	}

	/**
	*	returns root's geometry component (const)
	*/
	const GeometryComponent SceneRoot::geometryComponent() const {
		return mTransformComponent.ggroupComponent().ginstanceComp().geometryComponent();
	}

	/**
	*	returns root's indexed material
	*/
	const MaterialComponent SceneRoot::materialComponent(const std::size_t& idx) const {
		return mTransformComponent.ggroupComponent().ginstanceComp().materialComponent(idx);
	}

	/**
	*	returns root's geometry instance component
	*/
	const GeometryInstanceComponent SceneRoot::ginstanceComponent() const {
		return mTransformComponent.ggroupComponent().ginstanceComp();
	}

	/**
	*	returns root's geometry group component
	*/
	const GeometryGroupComponent SceneRoot::ggroupComponent() const {
		return mTransformComponent.ggroupComponent();
	}

	/**
	*	returns root's transform component
	*/
	const TransformComponent SceneRoot::transformComponent() const {
		return mTransformComponent;
	}

	/**
	*	returns root's mutable geometry component
	*/
	GeometryComponent& SceneRoot::geometryComponent() {
		return mTransformComponent.ggroupComponent().ginstanceComp().geometryComponent();
	}

	/**
	*	returns root's mutable material component
	*/
	MaterialComponent& SceneRoot::materialComponent(const std::size_t& idx) {
		return mTransformComponent.ggroupComponent().ginstanceComp().materialComponent(idx);
	}

	/**
	*	returns root's mutable geometry instance component
	*/
	GeometryInstanceComponent& SceneRoot::ginstanceComponent() {
		return mTransformComponent.ggroupComponent().ginstanceComp();
	}

	/**
	*	returns root's mutable geometry group component
	*/
	GeometryGroupComponent& SceneRoot::ggroupComponent() {
		return mTransformComponent.ggroupComponent();
	}

	/**
	*	returns root's mutable transform component
	*/
	TransformComponent& SceneRoot::transformComponent() {
		return mTransformComponent;
	}
}
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_SCENE_ROOT_H

