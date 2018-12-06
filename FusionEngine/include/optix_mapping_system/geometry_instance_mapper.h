#ifndef INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_INSTANCE_MAPPER_H
#define INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_INSTANCE_MAPPER_H

// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_aabb_namespace.h>

// scene
#include <optix_mapping_system/mapper.h>
#include <scene/material.h>

namespace map {
	class GinstanceMapper : public Mapper<optix::GeometryInstance, optix::Geometry, std::vector<scene::Material>> {
	public:
		explicit GinstanceMapper(optix::Context& ctx);
		explicit GinstanceMapper(
			optix::Context& ctx,
			const std::string& closestHitProgPath,
			const std::string& closestHitProgName,
			const std::string& anyHitProgPath,
			const std::string& anyHitProgName);

		const std::string closestHitProgPath() const { return mClosestHitProgPath; }
		const std::string anyHitProgPath() const { return mAnyHitProgPath; }
		const std::string closestHitProgName() const { return mClosestHitProgName; }
		const std::string anyHitProgName() const { return mAnyHitProgName; }

		void setClosestHitProgPath(const std::string& path);
		void setClosestHitProgName(const std::string& name);
		void setClosestHitProgram(const std::string& filepath, const std::string& name);
		void setAnyHitProgPath(const std::string& path);
		void setAnyHitProgName(const std::string& name);
		void setAnyHitProgram(const std::string& filepath, const std::string& name);

		const optix::GeometryInstance map(const optix::Geometry& geometry, const std::vector<scene::Material>& mesh) override;
	protected:
		struct GinstancePrograms {
			optix::Program closestHitProg;
			optix::Program anyHitProg;
		};
		optix::TextureSampler generateTexSampler(const scene::Material& mat);
		void addVarsToPrograms(GinstancePrograms& programs, const scene::Material& mat);
		GinstancePrograms generatePrograms();
	private:
		optix::Context mContext;
		std::string mClosestHitProgPath;
		std::string mAnyHitProgPath;
		std::string mClosestHitProgName;
		std::string mAnyHitProgName;
	};

	/**
	*	Constructor
	*/
	GinstanceMapper::GinstanceMapper(optix::Context& ctx)
		: mContext(ctx)
	{ 
		mClosestHitProgPath = "res\\ptx\\phong.ptx";
		mClosestHitProgName = "closest_hit_radiance";
		mAnyHitProgPath = "res\\ptx\\phong.ptx";
		mAnyHitProgName = "any_hit_shadow";
	}

	/**
	*	Constructor
	*/
	GinstanceMapper::GinstanceMapper(
		optix::Context& ctx,
		const std::string& closestHitProgPath,
		const std::string& closestHitProgName,
		const std::string& anyHitProgPath,
		const std::string& anyHitProgName)
		: mContext(ctx), 
		mClosestHitProgPath(closestHitProgPath),
		mClosestHitProgName(closestHitProgName),
		mAnyHitProgPath(anyHitProgPath),
		mAnyHitProgName(anyHitProgName)
	{ }

	/**
	*	sets closest hit program's path
	*/
	void GinstanceMapper::setClosestHitProgPath(const std::string& path) {
		mClosestHitProgPath = path;
	}

	/**
	*	sets closest hit program's name
	*/
	void GinstanceMapper::setClosestHitProgName(const std::string& name) {
		mClosestHitProgName = name;
	}

	/**
	*	sets closest hit program's attributes (path & name)
	*/
	void GinstanceMapper::setClosestHitProgram(const std::string& filepath, const std::string& name) {
		mClosestHitProgPath = filepath;
		mClosestHitProgName = name;
	}

	/**
	*	sets any hit programs filepath
	*/
	void GinstanceMapper::setAnyHitProgPath(const std::string& path) {
		mAnyHitProgPath = path;
	}

	/**
	*	sets lany hit program's name
	*/
	void GinstanceMapper::setAnyHitProgName(const std::string& name) {
		mAnyHitProgName = name;
	}

	/**
	*	sets any hit program's attributes (filepath & name)
	*/
	void GinstanceMapper::setAnyHitProgram(const std::string& filepath, const std::string& name) {
		mAnyHitProgPath = filepath;
		mAnyHitProgName = name;
	}

	/**
	* creates and sets Texture Sampler
	*/
	optix::TextureSampler GinstanceMapper::generateTexSampler(const scene::Material& mat) {
		optix::TextureSampler texSampler = mContext->createTextureSampler();
		texSampler->setWrapMode(0, RT_WRAP_REPEAT);
		texSampler->setWrapMode(1, RT_WRAP_REPEAT);
		texSampler->setWrapMode(2, RT_WRAP_REPEAT);
		texSampler->setIndexingMode(RT_TEXTURE_INDEX_NORMALIZED_COORDINATES);
		texSampler->setReadMode(RT_TEXTURE_READ_NORMALIZED_FLOAT);
		texSampler->setMaxAnisotropy(1.0f);
		texSampler->setMipLevelCount(1u);
		texSampler->setArraySize(1u);

		// buffer with single texel
		// TODO: load textures :)
		optix::Buffer texBuffer = mContext->createBuffer(RT_BUFFER_INPUT, RT_FORMAT_UNSIGNED_BYTE4, 1u, 1u);
		unsigned char* bufferData = static_cast<unsigned char*>(texBuffer->map());
		bufferData[0] = static_cast<unsigned char>(optix::clamp((int)(mat.diffusion().x * 255.0f), 0, 255));
		bufferData[1] = static_cast<unsigned char>(optix::clamp((int)(mat.diffusion().y * 255.0f), 0, 255));
		bufferData[2] = static_cast<unsigned char>(optix::clamp((int)(mat.diffusion().z * 255.0f), 0, 255));
		bufferData[3] = 255;
		texBuffer->unmap();
		texSampler->setBuffer(0u, 0u, texBuffer);
		texSampler->setFilteringModes(RT_FILTER_LINEAR, RT_FILTER_LINEAR, RT_FILTER_NONE);
		return texSampler;
	}

	GinstanceMapper::GinstancePrograms GinstanceMapper::generatePrograms() {
		GinstancePrograms programs;
		programs.closestHitProg = mContext->createProgramFromPTXFile(mClosestHitProgPath, mClosestHitProgName);
		programs.anyHitProg = mContext->createProgramFromPTXFile(mAnyHitProgPath, mAnyHitProgName);
		return programs;
	}

	void GinstanceMapper::addVarsToPrograms(GinstanceMapper::GinstancePrograms& programs, const scene::Material& mat) {
		// any hit program does not make any use of the material variables
		optix::TextureSampler texSampler = generateTexSampler(mat);
		programs.closestHitProg["Kd_map"]->setTextureSampler(texSampler);
		// TODO: modify this when able to load textures
		programs.closestHitProg["Kd_mapped"]->setInt(0);
		programs.closestHitProg["Kd"]->setFloat(mat.diffusion());
		programs.closestHitProg["Ks"]->setFloat(mat.specular());
		programs.closestHitProg["Kr"]->setFloat(mat.radiance());
		programs.closestHitProg["Ka"]->setFloat(mat.ambient());
		programs.closestHitProg["phong_exp"]->setFloat(mat.shininess());
	}

	const optix::GeometryInstance GinstanceMapper::map(const optix::Geometry& geom, const std::vector<scene::Material>& mat) {
		// generate programs
		std::vector<optix::Material> materials;
		for (std::size_t m = 0; m < mat.size(); ++m) {
			optix::Material material = mContext->createMaterial();
			GinstancePrograms programs = generatePrograms();
			addVarsToPrograms(programs, mat[m]);
			material->setClosestHitProgram(0u, programs.closestHitProg);
			material->setAnyHitProgram(0u, programs.anyHitProg);
			materials.emplace_back(material);
		}
		optix::GeometryInstance ginstance = mContext->createGeometryInstance(geom, materials.begin(), materials.end());
		ginstance->setMaterialCount(mat.size());
		return ginstance;
	}
}	//	!namespace map
#endif // !INCLUDE_OPTIX_MAPPING_SYSTEM_GEOMETRY_INSTANCE_MAPPER_H
