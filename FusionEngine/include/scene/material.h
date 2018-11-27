#ifndef _INCLUDE_SCENE_MATERIAL_H_
#define _INCLUDE_SCENE_MATERIAL_H_

#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_aabb_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_matrix_namespace.h>
//
#include <vector>
#include <string>

namespace scene {
	/*! Material simple representation
	*/
	class Material {
	public:
		// Empty Constructor
		explicit Material() { }
		explicit Material(const std::string& name, 
			const optix::float3& diff, 
			const optix::float3& spec, 
			const optix::float3& rad, 
			const optix::float3& amb, 
			const optix::float1& shin);
		const std::string name() const { return mName; }
		const optix::float3 diffusion() const { return mDiffuse; }
		const optix::float3 specular() const { return mSpecular; }
		const optix::float3 radiance() const { return mRadiance; }
		const optix::float3 ambient() const { return mAmbient; }
		const optix::float1 shininess() const { return mShininess; }
		const Material generateDefault();
	private:
		std::string mName;
		optix::float3 mDiffuse;
		optix::float3 mSpecular;
		optix::float3 mRadiance;
		optix::float3 mAmbient;
		optix::float1 mShininess;
	};

	// constructor
	Material::Material(const std::string& name,
		const optix::float3& diff,
		const optix::float3& spec,
		const optix::float3& rad,
		const optix::float3& amb,
		const optix::float1& shin)
		: mDiffuse(diff), 
		mSpecular(spec), 
		mRadiance(rad), 
		mAmbient(amb), 
		mShininess(shin)
	{ }

	/*! Returns a default material
	*/
	const Material Material::generateDefault() {
		optix::float3 def = optix::make_float3(0.5f, 0.5f, 0.5f);
		return Material(std::string("default"), def, def, def, def, optix::make_float1(0.0f));
	}
}	//	!namespace scene
#endif // !_INCLUDE_SCENE_MATERIAL_H_


