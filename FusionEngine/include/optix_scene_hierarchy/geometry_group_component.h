#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_GEOMETRY_GROUP_COMPONENT_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_GEOMETRY_GROUP_COMPONENT_H
/// OptiX
#include <optixu/optixpp_namespace.h>
/// Fusion
#include <optix_scene_hierarchy/component.h>
#include <optix_scene_hierarchy/geometry_instance_component.h>
///
namespace tree {
	class GeometryGroupComponent : public Component<optix::GeometryGroup> {
	public:
		explicit GeometryGroupComponent() { }
		explicit GeometryGroupComponent(const optix::GeometryGroup& ggroup);
		explicit GeometryGroupComponent(const optix::GeometryGroup& ggroup, 
			const GeometryInstanceComponent& ginstanceComp);
		const GeometryInstanceComponent ginstanceComp() const { return mGInstanceComponent; }
		GeometryInstanceComponent& ginstanceComp() { return mGInstanceComponent; }
		void setGinstanceComponent(const GeometryInstanceComponent& gistanceComp);
		void setAcceleration(const optix::Acceleration& accel);
	private:
		optix::Acceleration mAcceleration;
		GeometryInstanceComponent mGInstanceComponent;
	};

	/**
	*	Constructor
	*/
	GeometryGroupComponent::GeometryGroupComponent(const optix::GeometryGroup& ggroup)
		: Component(ggroup)
	{ 
		this->get()->setChildCount(1u);
	}

	/**
	*	Constructor
	*/
	GeometryGroupComponent::GeometryGroupComponent(const optix::GeometryGroup& ggroup, const GeometryInstanceComponent& gistanceComp) 
		: Component(ggroup), mGInstanceComponent(gistanceComp)
	{ }

	/**
	*	sets Geometry Instance Component
	*/
	void tree::GeometryGroupComponent::setGinstanceComponent(const GeometryInstanceComponent & gistanceComp) {
		mGInstanceComponent = gistanceComp;
		this->get()->addChild(mGInstanceComponent.get());
	}
	/**
	*	sets Acceleration Structure
	*/
	void GeometryGroupComponent::setAcceleration(const optix::Acceleration & accel) {
		mAcceleration = accel;
		this->get()->setAcceleration(mAcceleration);
	}
}
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_GEOMETRY_GROUP_COMPONENT_H
