#ifndef INCLUE_OPTIX_SCENE_HIERARCHY_GEOMETRY_COMPONENT_H
#define INCLUE_OPTIX_SCENE_HIERARCHY_GEOMETRY_COMPONENT_H
/// OptiX
#include <optixu/optixpp_namespace.h>
/// Fusion
#include <optix_scene_hierarchy/component.h>
/// STL
namespace tree {
	/**
	*	Geometry Component
	*/
	class GeometryComponent : public Component<optix::Geometry> {
	public:
		explicit GeometryComponent() { }
		explicit GeometryComponent(const optix::Geometry& geometry);
		explicit GeometryComponent(
			const optix::Geometry& geometry,
			const optix::Buffer& vertexBuffer,
			const optix::Buffer& indexBuffer,
			const optix::Buffer& normalsBuffer,
			const optix::Buffer& texcoordsBuffer,
			const optix::Buffer& materialsBuffer,
			const optix::Program& interProg,
			const optix::Program& bboxProg);
		void setVertexBuffer(const optix::Buffer& buffer);
		void setIndexBuffer(const optix::Buffer& buffer);
		void setNormalsBuffer(const optix::Buffer& buffer);
		void setTexcoordsBuffer(const optix::Buffer& buffer);
		void setMaterialsBuffer(const optix::Buffer& buffer);
		void setBoundingBoxProgram(const optix::Program& program);
		void setIntersectionProgram(const optix::Program& program);
	private:
		optix::Program mIntersectionProgram;
		optix::Program mBoundingBoxProgram;
		optix::Buffer mVertexBuffer;
		optix::Buffer mIndexBuffer;
		optix::Buffer mNormalsBuffer;
		optix::Buffer mTexcoordsBuffer;
		optix::Buffer mMaterialsBuffer;
	};

	/**
	*	Constructor
	*/
	GeometryComponent::GeometryComponent(const optix::Geometry& geometry)
		: Component(geometry)
	{ }

	/**
	*	Constructor
	*/
	GeometryComponent::GeometryComponent(
		const optix::Geometry& geometry, 
		const optix::Buffer& vertexBuffer,
		const optix::Buffer& indexBuffer,
		const optix::Buffer& normalsBuffer,
		const optix::Buffer& texcoordsBuffer,
		const optix::Buffer& materialsBuffer,
		const optix::Program& interProg,
		const optix::Program& bboxProg)
		: Component(geometry),
		mVertexBuffer(vertexBuffer),
		mIndexBuffer(indexBuffer),
		mNormalsBuffer(normalsBuffer),
		mTexcoordsBuffer(texcoordsBuffer),
		mMaterialsBuffer(materialsBuffer),
		mIntersectionProgram(interProg),
		mBoundingBoxProgram(bboxProg)
	{ }

	/**
	*	sets vertex buffer
	*/
	void GeometryComponent::setVertexBuffer(const optix::Buffer& buffer) {
		mVertexBuffer = buffer;
		mIntersectionProgram["vertex_buffer"]->setBuffer(buffer);
		mBoundingBoxProgram["vertex_buffer"]->setBuffer(buffer);
	}

	/**
	*	sets index buffer
	*/
	void GeometryComponent::setIndexBuffer(const optix::Buffer& buffer) {
		mIndexBuffer = buffer;
		mIntersectionProgram["index_buffer"]->setBuffer(buffer);
	}

	/**
	*	sets normals buffer
	*/
	void GeometryComponent::setNormalsBuffer(const optix::Buffer& buffer) {
		mNormalsBuffer = buffer;
		mIntersectionProgram["normal_buffer"]->setBuffer(buffer);
	}

	/**
	*	sets texcoord buffer
	*/
	void GeometryComponent::setTexcoordsBuffer(const optix::Buffer& buffer) {
		mTexcoordsBuffer = buffer;
		mIntersectionProgram["texcoord_buffer"]->setBuffer(buffer);
	}

	/**
	*	sets materials buffer
	*/
	void GeometryComponent::setMaterialsBuffer(const optix::Buffer& buffer) {
		mMaterialsBuffer = buffer;
		mIntersectionProgram["materials_buffer"]->setBuffer(buffer);
	}

	/**
	*	sets intersection program
	*/
	void GeometryComponent::setIntersectionProgram(const optix::Program& program) {
		mIntersectionProgram = program;
	}

	/**
	*	sets Bounding Box program
	*/
	void GeometryComponent::setBoundingBoxProgram(const optix::Program& program) {
		mBoundingBoxProgram = program;
	}
}	//	!namespace tree
#endif // !INCLUE_OPTIX_SCENE_HIERARCHY_GEOMETRY_COMPONENT_H

