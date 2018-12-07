#ifndef INCLUDE_OPTIX_SCENE_GRAPH_GEOMETRY_NODE_H
#define INCLUDE_OPTIX_SCENE_GRAPH_GEOMETRY_NODE_H
/// OptiX
#include <optixu/optixpp_namespace.h>
/// Logging
#include <plog/Log.h>
/// Fusion
#include <optix_scene_graph/node_type.h>
namespace graph {
	/**
	*	Geometry Node
	*/
	class GeometryNode : public Node<optix::Geometry> {
	public:
		explicit GeometryNode() { }
		const optix::Program intersectionProgram() const;
		const optix::Program boundingBoxProgram() const;
		optix::Program& intersectionProgram();
		optix::Program& boundingBoxProgram();
		void setIntersectionProgram(const optix::Program& interProg);
		void setBoundingBoxProgram(const optix::Program& bboxProg);
	};

	/**
	*	
	*/
	const optix::Program GeometryNode::intersectionProgram() const {
		return this->get()->getIntersectionProgram();
	}

	/**
	*
	*/
	const optix::Program GeometryNode::boundingBoxProgram() const {
		return this->get()->getBoundingBoxProgram();
	}

	/**
	*
	*/
	optix::Program& GeometryNode::intersectionProgram() {
		return this->get()->getIntersectionProgram();
	}

	/**
	*	
	*/
	optix::Program& GeometryNode::boundingBoxProgram() {
		return this->get()->getBoundingBoxProgram();
	}

	/**
	*
	*/
	void GeometryNode::setIntersectionProgram(const optix::Program& program) {
		this->get()->setIntersectionProgram(program);
	}

	/**
	*
	*/
	void GeometryNode::setBoundingBoxProgram(const optix::Program& program) {
		this->get()->setBoundingBoxProgram(program);
	}

#endif // !INCLUDE_OPTIX_SCENE_GRAPH_GEOMETRY_NODE_H

