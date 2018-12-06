#ifndef INCLUDE_OPTIX_SCENE_GRAPH_NODE_TYPE_H
#define INCLUDE_OPTIX_SCENE_GRAPH_NODE_TYPE_H
/// Logging
#include <plog/Log.h>
/// STL
#include <vector>
namespace graph {
	/**
	*	Empty Node Element
	*/
	template <typename NodeType>
	class Node {
	public:
		explicit Node() { }
		explicit Node(const NodeType& instance)
			: mInstance(instance)
		{ }
		const NodeType get() const { return mInstance; }
		bool hasChildren() { return !mChildren.empty(); }
		const std::vector<Node> children() const { return mChildren; }
		const Node getChild(const std::size_t& idx);
		NodeType& get() { return mInstance; }
		void addChild(const Node& child);
		void setInstance(const NodeType& node) { mInstance = node; }
	private:
		NodeType mInstance;
		std::vector<Node> mChildren;
	};

	/**
	*	returns child w.r.t the index
	*/
	template <typename NodeType>
	const Node Node::getChild(const std::size_t& idx) {
		if (idx < 0 || idx >= mChildren.size()) {
			LOG_ERROR << "Index out of range.";
			return NULL;
		}
		return mChildren[idx];
	}

	/**
	*	add child to node
	*/
	template <typename NodeType>
	void Node::addChild(const Node& child) {
		mChilder.emplace_back(child);
	}
}	//	!namespace graph
#endif // !INCLUDE_OPTIX_SCENE_GRAPH_NODE_TYPE_H

