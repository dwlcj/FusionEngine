#ifndef INCLUDE_OPTIX_SCENE_GRAPH_GEOMETRY_INSTANCE_NODE_H
#define INCLUDE_OPTIX_SCENE_GRAPH_GEOMETRY_INSTANCE_NODE_H
///OptiX
#include <optixu/optixpp_namespace.h>
/// Fusion
#include <optix_scene_graph/node_type.h>
#include <optix_scene_graph/geometry_node.h>
namespace graph {
	/**
	*	Geometry Instance Node
	*/
	class GeometryInstanceNode : public Node<optix::GeometryInstance> {
	public:
		GeometryInstanceNode() { }
		void setVertexBuffer(const optix::Buffer& buffer);
		void setIndexBuffer(const optix::Buffer& buffer);
		void setNormalsBuffer(const optix::Buffer& buffer);
		void setTexcoordBuffer(const optix::Buffer& buffer);
		void setMaterialBuffer(const optix::Buffer& buffer);
		void setClosestHitProgram(const optix::Program& program);
		void setAnyHitProgram(const optix::Program& program);
		void setTextureSampler(const optix::TextureSampler& sampler);;
	};

}	//	!namespace graph
#endif // !INCLUDE_OPTIX_SCENE_GRAPH_GEOMETRY_INSTANCE_NODE_H

