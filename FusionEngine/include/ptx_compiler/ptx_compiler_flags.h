#ifndef INCLUDE_PTX_COMPILER_PTX_COMPILER_FLAGS_H
#define INCLUDE_PTX_COMPILER_PTX_COMPILER_FLAGS_H

#include <vector>
namespace ptx {
	/**
	*	Available Host Platforms
	*/
	enum class Platform : std::size_t {
		None = 0,
		x86,
		x64
	};

	/**
	*	Compile Configurations
	*/
	enum class CompileConf : std::size_t {
		None = 0,
		Debug,
		Release
	};
}	//	!namespace ptx
#endif // !INCLUDE_PTX_COMPILER_PTX_COMPILER_FLAGS_H

