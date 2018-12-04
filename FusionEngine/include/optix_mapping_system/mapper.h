#ifndef _INCLUDE_OPTIX_MAPPING_SYSTEM_MAPPER_H_
#define _INCLUDE_OPTIX_MAPPING_SYSTEM_MAPPER_H_

namespace map {

	/** Mapper interface
	*/
	template <typename OutType, typename... InTypes>
	class Mapper {
	public:
		virtual const OutType map(const InTypes& ...inputsToMap) = 0;
	};
}
#endif // !_INCLUDE_OPTIX_MAPPING_SYSTEM_MAPPER_H_

