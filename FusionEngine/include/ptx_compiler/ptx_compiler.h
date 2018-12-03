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

	/**
	*	PTX compiler
	*	provides an interface to compile .cu files to ptx
	*	as well as many utlities regarding .cu and .ptx files
	*/
	class PTXCompiler {
	public:
		explicit PTXCompiler() { }
		explicit PTXCompiler(const std::uint8_t& arch, 
			const bool& useFastMath, 
			const bool& rdc, 
			const std::vector<std::string>& includeDirs,
			const std::vector<std::string>& OptixIncludeDirs,
			const std::string& conf,
			const std::string& platform);
		const std::uint8_t cudaArch() const { return mCudaArch; }
		const bool useFastMath() const { return mUseFastMath; }
		const bool rdc() const { return mRdc; }
		const std::vector<std::string> includeDirs() const;
		const std::string configuration() const;
		void addIncludeDir(const std::string& dir);
		void addOptiXIncludeDir(const std::string& dir);
		void setCudaArch(const std::uint8_t& arch);
		void setHostPlatform(const std::string& platform);
		void setUseFastMath(const bool& use);
		void setRdc(const bool& use);
		void setCompileConfiguration(const std::string& conf);
		const std::string compileStr(const std::string& filepath);
		void compileFile(const std::string& cuFilepath, const std::string& outputFilepath);
	protected:
		const bool readSrcFile(const std::experimental::filesystem::path& filepath, std::string& srcStr);
		const std::string cuStrFromFile(const std::experimental::filesystem::path& filepath);
		const std::string archOption() const;
		const std::string useFastMathOption() const;
		const std::string rdcOption() const;
		const std::string configPlatformOption() const;
	private:
		std::uint8_t mCudaArch;
		Platform mPlatform;
		CompileConf mConfiguration;
		bool mUseFastMath;
		bool mRdc;
		std::vector<std::experimental::filesystem::path> mIncludeDirs;
		std::vector<std::experimental::filesystem::path> mOptiXIncludeDirs;
		std::vector<std::string> mInclude;
		
	};

	/**
	*	Initializes member fields
	*/
	PTXCompiler::PTXCompiler(
		const std::uint8_t& arch, 
		const bool& useFastMath, 
		const bool& rdc, 
		const std::vector<std::string>& includeDirs,
		const std::vector<std::string>& optixIncludeDirs,
		const std::string& conf,
		const std::string& platform) 
		: mCudaArch(arch), mUseFastMath(useFastMath), mRdc(rdc)
	{ 
		for (std::vector<std::string>::const_iterator it = includeDirs.begin();
			it != includeDirs.end(); ++it) {
			mIncludeDirs.emplace_back(std::experimental::filesystem::path(*it));
		}
		for (std::vector<std::string>::const_iterator it = optixIncludeDirs.begin();
			it != optixIncludeDirs.end(); ++it) {
			mOptiXIncludeDirs.emplace_back(std::experimental::filesystem::path(*it));
		}
		setHostPlatform(platform);
		setCompileConfiguration(conf);
	}

	/**
	*	returns compiler include directories as vector of strings
	*/
	const std::vector<std::string> PTXCompiler::includeDirs() const {
		std::vector<std::string> dirs;
		for (std::vector<std::experimental::filesystem::path>::const_iterator it = mIncludeDirs.begin();
			it != mIncludeDirs.end(); ++it) {
			dirs.emplace_back("-I" + it->generic_string());
		}
		for (std::vector<std::experimental::filesystem::path>::const_iterator it = mOptiXIncludeDirs.begin();
			it != mOptiXIncludeDirs.end(); ++it) {
			dirs.emplace_back("-I" + it->generic_string());
		}
		return dirs;
	}

	/**
	*	Return Compiler Confguration as string
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

	/**
	*	sets compiling configuration
	*/
	void PTXCompiler::setCompileConfiguration(const std::string& conf) {
		if (conf == "Debug") {
			mConfiguration = CompileConf::Debug;
		}
		else if (conf == "Release") {
			mConfiguration == CompileConf::Release;
		}
		else {
			mConfiguration = CompileConf::None;
		}
	}

	/**
	*	adds include directory to compiler options
	*/
	void PTXCompiler::addIncludeDir(const std::string& dir) {
		mIncludeDirs.emplace_back(std::experimental::filesystem::path(dir));
	}
	void PTXCompiler::addOptiXIncludeDir(const std::string& dir) {
		mOptiXIncludeDirs.emplace_back(std::experimental::filesystem::path(dir));
	}

	/**
	*	sets Cuda Compute Capabitlity
	*/
	void PTXCompiler::setCudaArch(const std::uint8_t& arch) {
		mCudaArch = arch;
	}

	/**
	*	sets host platform arch x86 or x64
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

	/**
	*	modifies use fast math option
	*/
	void PTXCompiler::setUseFastMath(const bool& use) {
		mUseFastMath = use;
	}

	/**
	*	modifies rdc option
	*/
	void PTXCompiler::setRdc(const bool& use) {
		mRdc = use;
	}

	/**
	*	reads a .txt file and returns its content as string 
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

	/**
	*	reads .cu file and returns content as string
	*/
	const std::string PTXCompiler::cuStrFromFile(const std::experimental::filesystem::path& filepath) {
		std::string cuStr;
		if (readSrcFile(filepath, cuStr)) {
			return cuStr;
		}
		LOG_ERROR << "Bad filepath or file: " << filepath.generic_string();
		return "";
	}



	/**
	*	Returns cuda architecture (cli) option
	*/
	const std::string PTXCompiler::archOption() const {
		return std::string("--gpu-architecture=compute_" + std::to_string(mCudaArch));
		//return std::string("");
	}

	/**
	*	Returns use fast math (cli) option
	*/
	const std::string PTXCompiler::useFastMathOption() const {
		if (mUseFastMath) {
			return std::string("--use_fast_math");
		}
		return std::string("");
	}

	/**
	*	return rdc option as string
	*/
	const std::string PTXCompiler::rdcOption() const {
		if (mRdc) {
			return std::string("--relocatable-device-code=true");
		}
		return std::string("--relocatable-device-code=false");
	}

	/**
	*	Returns Compiler Configuration and Platform cli option
	*/
	const std::string PTXCompiler::configPlatformOption() const {
		if (mPlatform == Platform::x64 && mConfiguration == CompileConf::Debug) {
			return std::string("-D__x86_64");
		}
		return std::string("");
	}

	/*! compiles .cu file returns compiled (ptx) content as string
	*/
	const std::string PTXCompiler::compileStr(const std::string& filepath) {
		std::experimental::filesystem::path cuFilepath(filepath);
		std::string cuSrc = cuStrFromFile(cuFilepath);
		LOG_DEBUG << cuSrc;
		// create nvrtc program
		nvrtcProgram prog = 0;
		const nvrtcResult prog_creation_res = nvrtcCreateProgram(&prog, cuSrc.c_str(), filepath.c_str(), 0, NULL, NULL);
		if (prog_creation_res != NVRTC_SUCCESS) {
			LOG_ERROR << "PTX compiling failed (Cannot create NVRTC Program). file: " << filepath;
		}
		// Unlikable workaround to use compolier options
		std::vector<const char*> options;
		mInclude = includeDirs();
		const std::string arch = archOption();
		const std::string fastMath = useFastMathOption();
		const std::string devDef("--device-as-default-execution-space");
		const std::string confPlat = configPlatformOption();
		const std::string rdc = rdcOption();
		options.emplace_back(arch.c_str());
		options.emplace_back(fastMath.c_str());
		options.emplace_back(devDef.c_str());
		options.emplace_back(confPlat.c_str());
		options.emplace_back(rdc.c_str());
		for (std::vector<std::string>::const_iterator it = mInclude.begin();
			it != mInclude.end(); ++it) {
			options.emplace_back(it->c_str());
		}
		// ptx code as string, ptx compilation log
		std::string ptx, compilation_log;
		const nvrtcResult compile_res = nvrtcCompileProgram(prog, (int)options.size(), options.data());
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
			LOG_DEBUG << "}";
		}
		// retrieve ptx code
		std::size_t ptx_size = 0;
		const nvrtcResult nvrtc_ptx_size_res = nvrtcGetPTXSize(prog, &ptx_size);
		ptx.resize(ptx_size);
		const nvrtcResult nvrtc_ptx_res = nvrtcGetPTX(prog, &ptx[0]);
		nvrtcDestroyProgram(&prog);
		return ptx;
	}

	/**
	*	Compiles ptx and saves ptx file
	*/
	void PTXCompiler::compileFile(const std::string& cuFilepath, const std::string& outputFilepath) {
		const std::string ptxStr = compileStr(cuFilepath);
		std::ofstream fileHandle(outputFilepath, std::ofstream::out);
		fileHandle << ptxStr;
		fileHandle.close();
	}
}
#endif // !_INCLUDE_PTX_COMPILER_PTX_COMPILER_H_

