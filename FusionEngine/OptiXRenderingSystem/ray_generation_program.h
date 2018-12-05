#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_RAY_GENERATION_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_RAY_GENERATION_PROGRAM_H
/// STL
#include <string>
namespace rt {
	class RayGenerationProgram {
		virtual const std::string ptxPath() = 0;
		virtual const std::string cuPath() = 0;
		virtual const std::string name() = 0;
	};
}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_RAY_GENERATION_PROGRAM_H

