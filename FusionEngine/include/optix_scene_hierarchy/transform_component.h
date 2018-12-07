#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_TRANSFORM_COMPONENT_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_TRANSFORM_COMPONENT_H
/// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
/// Fusion
#include <optix_scene_hierarchy/component.h>
#include <optix_scene_hierarchy/geometry_group_component.h>
namespace tree {
	/**
	*	Transform Component
	*/
	class TransformComponent : public Component<optix::Transform> {
	public:
		explicit TransformComponent() { }
		explicit TransformComponent(const optix::Transform& transform);
		explicit TransformComponent(const optix::Transform& transform, const GeometryGroupComponent& ggroupComp);
		
		const optix::Matrix4x4 getTransMatrix() { return mTransMat; }
		const GeometryGroupComponent ggroupComponent() const { return mGGroupComponent; }
		
		void addGeometryGroupComponent(const GeometryGroupComponent& ggroupComp);
		GeometryGroupComponent& ggroupComponent() { return mGGroupComponent; }
		void translate(const optix::float3& tranVector);
		void rotate(const optix::float3& rotVector);
		void scale(const optix::float3& scaleVector);
		void setTransMatrix(const optix::Matrix4x4& mat);
	private:
		optix::Matrix4x4 mTransMat;
		GeometryGroupComponent mGGroupComponent;
	};

	/**
	*	Constructor
	*/
	TransformComponent::TransformComponent(const optix::Transform & transform) 
		: Component(transform), mTransMat(optix::Matrix4x4::identity())
	{ }

	/**
	*	Constructor	
	*/
	TransformComponent::TransformComponent(const optix::Transform& transform, const GeometryGroupComponent& ggroupComp) 
		: Component(transform), mGGroupComponent(ggroupComp)
	{ }

	/**
	*	sets tranformation matrix
	*/
	void TransformComponent::setTransMatrix(const optix::Matrix4x4& mat) {
		this->get()->setMatrix(false, (float*)mat.getData(), NULL);
		mGGroupComponent.get()->getAcceleration()->markDirty();
	}

	/**
	*	sets geometry group componenet
	*/
	void TransformComponent::addGeometryGroupComponent(const GeometryGroupComponent& ggroupComp) {
		mGGroupComponent = ggroupComp;
	}
}
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_TRANSFORM_COMPONENT_H

