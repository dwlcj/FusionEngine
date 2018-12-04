#ifndef INCLUDE_COMM_SYSTEM_PTX_COMPILER_MESSAGE_H
#define INCLUDE_COMM_SYSTEM_PTX_COMPILER_MESSAGE_H
/// Logging
#include <plog/Log.h>
///Fusion
#include <ptx_compiler/ptx_compiler_flags.h>
/// STL
#include <string>
namespace comm {
	class PTXCompilerMessage {
	public:
		PTXCompilerMessage(
			const std::string& platform,
			const std::string& compConf,
			const std::string& cudaArch,
			const bool& rdc,
			const bool& fastMath);
		const std::string platform() const { return mPlatform; }
		const std::string compilerConfiguration() const { return mCompilerConfiguration; }
		const std::uint8_t cudaArch() const { return mCudaArch; }
		const bool rdc() const { return mRdc; }
		const bool fastMath() const { return mFastMath; }
	private:
		std::string mPlatform;
		std::string mCompilerConfiguration;
		std::uint8_t mCudaArch;
		bool mRdc;
		bool mFastMath;
	};

	PTXCompilerMessage::PTXCompilerMessage(
		const std::string& platform,
		const std::string& compConf,
		const std::string& cudaArch,
		const bool& rdc,
		const bool& fastMath)
	{
		// check Platform
		if (platform != "x64" && platform != "x86") {
			LOG_ERROR << "Platform not recognized.";
			mPlatform = "x64";
		}
		else if (platform == "x86") {
			LOG_WARNING << "x86 Platform not Available.";
			mPlatform = "x64";
		}
		else {
			mPlatform = "x64";
		}

		// check Compiler Configuration
		if (compConf != "Debug" && compConf != "Release") {
			LOG_ERROR << "Unknown Compiler Configuration.";
			mCompilerConfiguration = "Debug";
		}
		else {
			mCompilerConfiguration = compConf;
		}

		// check Cuda Arch
		if (cudaArch == "30") {
			mCudaArch = 30;
		}
		else if (cudaArch == "51") {
			mCudaArch = 51;
		}
		else if (cudaArch == "52") {
			mCudaArch = 52;
		}
		else {
			LOG_ERROR << "Unknown Cuda Architecture.";
			mCudaArch = 30;
		}
		// set rdc
		mRdc = rdc;
		// set fast math
		mFastMath = fastMath;
	}
}
#endif // !INCLUDE_COMM_SYSTEM_PTX_COMPILER_MESSAGE_H

