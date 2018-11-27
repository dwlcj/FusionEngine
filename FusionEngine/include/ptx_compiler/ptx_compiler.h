#ifndef _INCLUDE_PTX_COMPILER_PTX_COMPILER_H_
#define _INCLUDE_PTX_COMPILER_PTX_COMPILER_H_

#define CERTH
// CUDA
#include <nvrtc.h>

//
#include <plog/Log.h>

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace ptx {
	/*! Host Platforms
	*/
	enum class Platform : std::size_t {
		None = 0,
		x86,
		x64
	};

	/*! Compile Configurations
	*/
	enum class CompileConf : std::size_t {
		None = 0,
		Debug,
		Release
	};

	/*! PTX compiler
		provides an interface to compile .cu files to ptx
		as well as many utlities regarding .cu and .ptx files
	*/
	class PTXCompiler {
	public:
		explicit PTXCompiler() { }
		explicit PTXCompiler(const std::uint8_t& arch, 
			const bool& useFastMath, 
			const bool& rdc, 
			const std::vector<std::string>& includeDirs, 
			const std::string& conf);
		const std::uint8_t cudaArch() const { return mCudaArch; }
		const bool useFastMath() const { return mUseFastMath; }
		const bool rdc() const { return mRdc; }
		const std::vector<std::string> includeDirs() const;
		const std::string configuration() const;
		void addIncludeDir(const std::string& dir);
		void setCudaArch(const std::uint8_t& arch);
		void setHostPlatform(const std::string& platform);
		void setUseFastMath(const bool& use);
		void setRdc(const bool& use);
		const std::string compileStr(const std::string& filepath);
		void compileFile(const std::string& filepath);
	protected:
		const bool readSrcFile(const std::experimental::filesystem::path& filepath, std::string& srcStr);
		const std::string cuStrFromFile(const std::experimental::filesystem::path& filepath);
		const std::vector<std::string> includeDirsToStr();
	private:
		std::uint8_t mCudaArch;
		Platform mPlatform;
		CompileConf mConfiguration;
		bool mUseFastMath;
		bool mRdc;
		std::vector<std::experimental::filesystem::path> mIncludeDirs;
	};

	/*! returns compiler include directories as vector of strings
	*/
	const std::vector<std::string> PTXCompiler::includeDirs() const {
		std::vector<std::string> dirs;
		for (std::vector<std::experimental::filesystem::path>::const_iterator it = mIncludeDirs.begin();
			it != mIncludeDirs.end(); ++it) {
			dirs.emplace_back(it->generic_string());
		}
		return dirs;
	}

	/*! Return Compiler Confguration as string
	*/
	const std::string PTXCompiler::configuration() const {
		switch (mConfiguration)
		{
		case ptx::CompileConf::None:
			return std::string("None");
		case ptx::CompileConf::Debug:
			return std::string("Debug");
		case ptx::CompileConf::Release:
			return std::string("Release");
		default:
			return std::string("None");
		}
	}

	/* adds include directory to compiler options
	*/
	void PTXCompiler::addIncludeDir(const std::string& dir) {
		mIncludeDirs.emplace_back(std::experimental::filesystem::path(dir));
	}

	/*! sets Cuda Compute Capabitlity
	*/
	void PTXCompiler::setCudaArch(const std::uint8_t& arch) {
		mCudaArch = arch;
	}

	/*! sets host platform arch x86 or x64
	*/
	void PTXCompiler::setHostPlatform(const std::string& platform) {
		if (platform == "x86") {
			mPlatform = Platform::x86;
		}
		else if (platform == "x64") {
			mPlatform = Platform::x64;
		}
		else {
			mPlatform = Platform::None;
		}
	}

	/*! modifies use fast math option
	*/
	void PTXCompiler::setUseFastMath(const bool& use) {
		mUseFastMath = use;
	}

	/*! modifies rdc option
	*/
	void PTXCompiler::setRdc(const bool& use) {
		mRdc = use;
	}

	/*! reads a .txt file and returns its content as string 
	*/
	const bool PTXCompiler::readSrcFile(const std::experimental::filesystem::path& filepath, std::string& srcStr) {
		std::ifstream file(filepath);
		if (file.good()) {
			// source file exists
			std::stringstream fileBuffer;
			fileBuffer << file.rdbuf();
			srcStr = fileBuffer.str();
			return true;
		}
		// bad path or file
		return false;
	}

	/*! reads .cu file and returns content as string
	*/
	const std::string PTXCompiler::cuStrFromFile(const std::experimental::filesystem::path& filepath) {
		std::string cuStr;
		if (readSrcFile(filepath, cuStr)) {
			return cuStr;
		}
		LOG_ERROR << "Bad filepath or file: " << filepath;
		return "";
	}

	/*! compiles .cu file returns compiled (ptx) content as string
	*/
	const std::string PTXCompiler::compileStr(const std::string& filepath) {
		std::experimental::filesystem::path cuFilepath(filepath);
		std::string cuSrc = cuStrFromFile(cuFilepath);
		// create nvrtc program
		nvrtcProgram prog = 0;
		const nvrtcResult prog_creation_res = nvrtcCreateProgram(&prog, cuSrc.c_str(), cuFilepath.c_str(), 0, NULL, NULL);
		if (prog_creation_res != NVRTC_SUCCESS) {
			LOG_ERROR << "PTX compiling failed (Cannot create NVRTC Program). file: " << filepath;
			throw std::exception();
		}
		std::vector<const char*> nvrtc_options;												// nvrtc compilation options
		std::vector<std::string> nvrtc_include_dirs = get_nvrtc_includes();					// get nvrtc include dirs
		const std::string incl_base = "-I" + cu_filepath.parent_path().generic_string();	// add cu file's base dir to nvrtc inclide paths
		nvrtc_include_dirs.emplace_back(incl_base.c_str());

		// ptx code as string, ptx compilation log
		std::string ptx, compilation_log;

		// fill nvrtc compilation options
		for (int i = 0; i < nvrtc_compiler_options.size(); i++) {
			nvrtc_options.emplace_back(nvrtc_compiler_options[i].c_str());
		}
		for (int i = 0; i < nvrtc_include_dirs.size(); i++) {
			nvrtc_options.emplace_back(nvrtc_include_dirs[i].c_str());
		}

		// compile cu to ptx
		const nvrtcResult compile_res = nvrtcCompileProgram(prog, (int)nvrtc_options.size(), nvrtc_options.data());

		// retrieve compilation log
		std::size_t log_size = 0;
		const nvrtcResult log_sz_res = nvrtcGetProgramLogSize(prog, &log_size);
		compilation_log.resize(log_size);
		if (log_size > 1) {
			const nvrtcResult log_res = nvrtcGetProgramLog(prog, &compilation_log[0]);
		}
		if (compile_res != NVRTC_SUCCESS) {
			LOG_FATAL << "PTX compilation failed.";
			LOG_DEBUG << "\tCompilation Log: {";
			LOG_DEBUG << "\t" + compilation_log;
			LOG_DEBUG << "\t}";
			throw std::exception();
		}
		// retrieve ptx code
		std::size_t ptx_size = 0;
		const nvrtcResult nvrtc_ptx_size_res = nvrtcGetPTXSize(prog, &ptx_size);
		ptx.resize(ptx_size);
		const nvrtcResult nvrtc_ptx_res = nvrtcGetPTX(prog, &ptx[0]);

		return ptx;
	}



	//------------------------------------
	//		NVRTC Compiler Options
	//------------------------------------
	// TODO: MUST CHANGE these are hard coded paths. Must find a generic way
	static std::vector<std::string> nvrtc_compiler_options = {
		"-arch=compute_30 ",
		"-use_fast_math ",
		"-default-device ",
		"-rdc=true ",
		"-D__x86_64 ",
		"-IC:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.2/include",
		"-ID:\\_dev\\_Projects\\_Visual_Studio\\360Fuzio\\360_fuzion\\src\\core\\CUDA",
		"-IC:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v8.0\\include"
	};

	//-----------------------------------------------------
	//	searches for CUDA directories to 
	//	add to nvrtc compilation include dirs
	//	Returns a string with cmd nvrtc compilation options
	//------------------------------------------------------
	// TODO: MUST CHANGE the paths are hard coded and must find generic solution
	static const std::string get_nvrtc_options() {
		return std::string("-arch=compute_52 -use_fast_math -default-device -rdc=true -D__x86_64 0 ");
	}

	//------------------------------------
	//	Returns nvrtc include paths
	//------------------------------------
	// TODO: MUST CHANGE the paths are hard coded and must find generic solution
	static const std::vector<std::string> get_nvrtc_includes() {
		std::vector<std::string> include_dirs;

#ifdef CERTH
		include_dirs.emplace_back("-ID:\\_dev\\_Projects\\_Visual_Studio\\360Fuzio\\360_fuzion\\dependencies\\NVIDIA\\Optix51\\include");
		include_dirs.emplace_back("-ID:\\_dev\\_Projects\\_Visual_Studio\\360Fuzio\\360_fuzion\\dependencies\\NVIDIA\\Optix51\\\\optixu");
		include_dirs.emplace_back("-ID:\\_dev\\_Projects\\_Visual_Studio\\360Fuzio\\360_fuzion\\dependencies\\NVIDIA\\Optix51\\mdl-sdk\\include");
		include_dirs.emplace_back("-ID:\\_dev\\_Projects\\_Visual_Studio\\360Fuzio\\360_fuzion\\dependencies\\ptx");
		include_dirs.emplace_back("-ID:\\_dev\\_Libraries\\NVIDIA\\OptiX51\\include");
		include_dirs.emplace_back("-IC:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.2/include");
#else
		include_dirs.emplace_back("-IE:\\_dev\\_Projects\\360Fuzion\\360_fuzion\\dependencies\\NVIDIA\\Optix51\\include");
		include_dirs.emplace_back("-IE:\\_dev\\_Projects\\360Fuzion\\360_fuzion\\dependencies\\NVIDIA\\Optix51\\include\\optixu");
		include_dirs.emplace_back("-IE:\\_dev\\_Libraries\\NVIDIA\\OptiX51\\SDK\\cuda");
		include_dirs.emplace_back("-IE:\\_dev\\_Projects\\360Fuzion\\360_fuzion\\dependencies\\ptx");
		include_dirs.emplace_back("-IE:\\_dev\\_Libraries\\NVIDIA\\OptiX51\\include");
		include_dirs.emplace_back("-IC:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0/include");
		//include_dirs.emplace_back("-IE:\\_dev\\_Libraries\\NVIDIA\\OptiX51\\include");
#endif

		return include_dirs;
	}

	//---------------------------------------------------
	// Reads a source file and puts the src in a string
	// return bool if successful
	//---------------------------------------------------
	static bool read_src_file(std::string& src, const filesystem::path& src_filepath) {
		std::ifstream file(src_filepath);
		if (file.good()) {
			// source file exists
			std::stringstream src_buffer;
			src_buffer << file.rdbuf();
			src = src_buffer.str();
			return true;
		}
		// bad path or file
		return false;
	}


	//---------------------------------------------
	// Reads a source file
	// Returns the source file's text as a string
	//----------------------------------------------
	static std::string cu_str_from_file(const  filesystem::path& cu_filepath) {
		std::string cu_src_str;
		if (read_src_file(cu_src_str, cu_filepath)) {
			// source file read successfully
			return cu_src_str;
		}
		// bad path or file
		LOG_FATAL << "File { " << cu_filepath.generic_string() << " } doesn't exist or is broken.";
		return "";
	}

	//-------------------------------------------
	// reads cu source file
	// compiles it to ptx
	// returns ptx code as string
	//--------------------------------------------
	static std::string ptx_from_cu(const std::string& cu_filename, const std::string& location) {
		if (cu_filename.empty()) {
			LOG_FATAL << "Given filepath is empty.";
			throw std::exception();
		}
		std::experimental::filesystem::path fs_location(location);
		//std::experimental::filesystem::path fs_location 
		std::experimental::filesystem::path cu_filepath = fs_location.append(cu_filename);

		// read source file
		std::string cu_src = cu_str_from_file(cu_filepath);

		// create nvrtc program
		nvrtcProgram prog = 0;
		const nvrtcResult prog_creation_res = nvrtcCreateProgram(&prog, cu_src.c_str(), location.c_str(), 0, NULL, NULL);
		if (prog_creation_res != NVRTC_SUCCESS) {
			LOG_FATAL << "PTX program creation failed.";
			throw std::exception();
		}

		std::vector<const char*> nvrtc_options;												// nvrtc compilation options
		std::vector<std::string> nvrtc_include_dirs = get_nvrtc_includes();					// get nvrtc include dirs
		const std::string incl_base = "-I" + cu_filepath.parent_path().generic_string();	// add cu file's base dir to nvrtc inclide paths
		nvrtc_include_dirs.emplace_back(incl_base.c_str());

		// ptx code as string, ptx compilation log
		std::string ptx, compilation_log;

		// fill nvrtc compilation options
		for (int i = 0; i < nvrtc_compiler_options.size(); i++) {
			nvrtc_options.emplace_back(nvrtc_compiler_options[i].c_str());
		}
		for (int i = 0; i < nvrtc_include_dirs.size(); i++) {
			nvrtc_options.emplace_back(nvrtc_include_dirs[i].c_str());
		}

		// compile cu to ptx
		const nvrtcResult compile_res = nvrtcCompileProgram(prog, (int)nvrtc_options.size(), nvrtc_options.data());

		// retrieve compilation log
		std::size_t log_size = 0;
		const nvrtcResult log_sz_res = nvrtcGetProgramLogSize(prog, &log_size);
		compilation_log.resize(log_size);
		if (log_size > 1) {
			const nvrtcResult log_res = nvrtcGetProgramLog(prog, &compilation_log[0]);
		}
		if (compile_res != NVRTC_SUCCESS) {
			LOG_FATAL << "PTX compilation failed.";
			LOG_DEBUG << "\tCompilation Log: {";
			LOG_DEBUG << "\t" + compilation_log;
			LOG_DEBUG << "\t}";
			throw std::exception();
		}
		// retrieve ptx code
		std::size_t ptx_size = 0;
		const nvrtcResult nvrtc_ptx_size_res = nvrtcGetPTXSize(prog, &ptx_size);
		ptx.resize(ptx_size);
		const nvrtcResult nvrtc_ptx_res = nvrtcGetPTX(prog, &ptx[0]);

		return ptx;
	}


}
#endif // !_INCLUDE_PTX_COMPILER_PTX_COMPILER_H_

