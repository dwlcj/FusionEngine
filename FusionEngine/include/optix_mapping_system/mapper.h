#ifndef _INCLUDE_OPTIX_MAPPING_SYSTEM_MAPPER_H_
#define _INCLUDE_OPTIX_MAPPING_SYSTEM_MAPPER_H_

namespace map {
	
	template <typename MapType>
	class Mapper {
	public:
		virtual void map(const MapType& objectToMap) = 0;
	};
}	//	!namespace map
#endif // !_INCLUDE_OPTIX_MAPPING_SYSTEM_MAPPER_H_

