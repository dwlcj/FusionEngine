#ifndef INCLUDE_OPTIX_MAPPING_SYSTEM_MAPPER_H
#define INCLUDE_OPTIX_MAPPING_SYSTEM_MAPPER_H

namespace map {

	/** Mapper interface
	*/
	template <typename OutType, typename... InTypes>
	class Mapper {
	public:
		virtual const OutType map(const InTypes& ...inputsToMap) = 0;
	};
}
#endif // !INCLUDE_OPTIX_MAPPING_SYSTEM_MAPPER_H

