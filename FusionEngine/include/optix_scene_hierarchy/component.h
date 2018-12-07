#ifndef INCLUDE_OPTIX_SCENE_HIERARCHY_COMPONENT_H
#define INCLUDE_OPTIX_SCENE_HIERARCHY_COMPONENT_H
namespace tree {
	template <typename CompType>
	class Component {
	public:
		explicit Component() { }
		explicit Component(const CompType& type);
		const CompType get() const { return mInstance; }
		CompType& get() { return mInstance; }
		void set(const CompType& type) { mInstance = type; }
	private:
		CompType mInstance;
	};

	/**
	*	Constructor
	*/
	template <typename CompType>
	Component<CompType>::Component(const CompType& type) {
		mInstance = type;
	}
}
#endif // !INCLUDE_OPTIX_SCENE_HIERARCHY_COMPONENT_H

