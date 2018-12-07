#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_MATERIAL_COMPONENT_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_MATERIAL_COMPONENT_H
/// Optix
#include <optixu/optixpp_namespace.h>
/// Fusion
#include <optix_scene_hierarchy/component.h>
/// STL
namespace tree {
	/**
	*	Material Component
	*/
	class MaterialComponent : public Component<optix::Material> {
	public:
		explicit MaterialComponent() { }
		explicit MaterialComponent(const optix::Material& mat);
		explicit MaterialComponent(
			const optix::Material& mat,
			const optix::float3& diff,
			const optix::float3& rad,
			const optix::float3& spec,
			const optix::float3& amb,
			const float& shin);
		const optix::float3 diffusionColor() const { return mDiffusion; }
		const optix::float3 specularColor() const { return mSpecular; }
		const optix::float3 radianceColor() const { return mRadiance; }
		const optix::float3 ambientColor() const { return mAmbient; }
		const optix::Program closestHitProgram() const { return mClosestHitProgram; }
		const optix::Program anyHitProgram() const { return mAnyHitProgram; }
		const float shininess() const { return mShininess; }

		void setDiffusionColor(const optix::float3& color);
		void setRandianceColor(const optix::float3& color);
		void setAmbientColor(const optix::float3& color);
		void setSpecularColor(const optix::float3& color);
		void setShininess(const float& val);
		void setTextureSampler(const optix::TextureSampler& sampler);
		void setClosestHitProgram(const optix::Program& program);
		void setAnyHitProgram(const optix::Program& program);
	private:
		optix::float3 mDiffusion;
		optix::float3 mSpecular;
		optix::float3 mRadiance;
		optix::float3 mAmbient;
		float mShininess;
		optix::Program mClosestHitProgram;
		optix::Program mAnyHitProgram;
		optix::TextureSampler mTextureSampler;
	};

	/**
	*	Constructor
	*/
	MaterialComponent::MaterialComponent(const optix::Material& mat)
		: Component(mat)
	{ }

	/**
	*	Constructor
	*/
	MaterialComponent::MaterialComponent(
		const optix::Material& mat,
		const optix::float3& diff,
		const optix::float3& rad,
		const optix::float3& spec,
		const optix::float3& amb,
		const float& shin)
		: Component(mat),
		mDiffusion(diff),
		mSpecular(spec),
		mRadiance(rad),
		mAmbient(amb),
		mShininess(shin)
	{ }

	/**
	*	Sets diffusion color
	*/
	void MaterialComponent::setDiffusionColor(const optix::float3& color) {
		mDiffusion = color;
		mClosestHitProgram["Kd"]->setFloat(color);
	}

	/**
	*	Sets specular color
	*/
	void MaterialComponent::setSpecularColor(const optix::float3& color) {
		mSpecular = color;
		mClosestHitProgram["Ks"]->setFloat(color);
	}

	/**
	*	Sets radiance color
	*/
	void MaterialComponent::setRandianceColor(const optix::float3& color) {
		mRadiance = color;
		mClosestHitProgram["Kr"]->setFloat(color);
	}

	/**
	*	Sets ambient color
	*/
	void MaterialComponent::setAmbientColor(const optix::float3& color) {
		mAmbient = color;
		mClosestHitProgram["Ka"]->setFloat(color);
	}

	/**
	*	sets shininess
	*/
	void MaterialComponent::setShininess(const float& val) {
		mShininess = val;
		mClosestHitProgram["phong_exp"]->setFloat(val);
	}

	/**
	*	sets texture sampler
	*/
	void MaterialComponent::setTextureSampler(const optix::TextureSampler& sampler) {
		mTextureSampler = sampler;
		mClosestHitProgram["Kd_map"]->setTextureSampler(sampler);
	}

	/**
	*	Sets closest hit program
	*/
	void MaterialComponent::setClosestHitProgram(const optix::Program& program) {
		mClosestHitProgram = program;
	}

	/**
	*	Sets anyhit program
	*/
	void MaterialComponent::setAnyHitProgram(const optix::Program& program) {
		mAnyHitProgram = program;
	}

}	//	!namespace tree
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_MATERIAL_COMPONENT_H

