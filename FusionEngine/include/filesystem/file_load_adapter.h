#ifndef _INCLUDE_FILESYSTEM_FILE_LOAD_ADAPTER_H_
#define _INCLUDE_FILESYSTEM_FILE_LOAD_ADAPTER_H_

// stl
#include <string>

namespace fs {
	// FileLoadAdapter Interface
	class FileLoadAdapter {
	public:
		explicit FileLoadAdapter() { }
		virtual ~FileLoadAdapter() = 0;
		virtual void loadFile(const std::string& filepath) = 0;
	};
}	// !namespace fs
#endif // !_INCLUDE_FILESYSTEM_FILE_LOAD_ADAPTER_H_

