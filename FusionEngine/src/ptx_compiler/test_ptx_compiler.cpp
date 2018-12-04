#define NOMINMAX
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Log.h>

#include <ptx_compiler/ptx_compiler.h>

#include <memory>
#include <string>

int main(int argc, char* argv[]) {
	//-----------------
	// Logging Related
	//-----------------
	plog::ColorConsoleAppender<plog::TxtFormatter> app;
	plog::init(plog::debug, &app);

	std::shared_ptr<ptx::PTXCompiler> compiler =
		std::make_shared<ptx::PTXCompiler>();
#ifdef CERTH
	compiler->setCudaArch(30);
	compiler->setHostPlatform("x64");
	compiler->setCompileConfiguration("Debug");
	compiler->setRdc(true);
	compiler->setUseFastMath(true);
	compiler->addIncludeDir("D:\\_dev\\_Projects\\_Visual_Studio\\360Fuzio\\360_fuzion\\dependencies\\NVIDIA\\Optix51\\include");
	compiler->addIncludeDir("D:\\_dev\\_Projects\\_Visual_Studio\\360Fuzio\\360_fuzion\\dependencies\\NVIDIA\\Optix51\\\\optixu");
	compiler->addIncludeDir("D:\\_dev\\_Projects\\_Visual_Studio\\360Fuzio\\360_fuzion\\dependencies\\NVIDIA\\Optix51\\mdl-sdk\\include");
	compiler->addIncludeDir("D:\\_dev\\_Projects\\_Visual_Studio\\360Fuzio\\360_fuzion\\dependencies\\ptx");
	compiler->addIncludeDir("D:\\_dev\\_Libraries\\NVIDIA\\OptiX51\\include");
	compiler->addIncludeDir("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.2/include");
	compiler->addIncludeDir("..\\src\\optix_mapping_system\\CUDA");
	//std::string ptx = compiler->compileStr("D:\\_dev\\_Libraries\\NVIDIA\\OptiX511\\SDK\\cuda\\phong.cu");
	compiler->compileFile("D:\\_dev\\_Libraries\\NVIDIA\\OptiX511\\SDK\\cuda\\triangle_mesh.cu", "..\\src\\optix_mapping_system\\CUDA\\triangle_mesh.ptx");
#else
	compiler->setCudaArch(30);
	compiler->setHostPlatform("x64");
	compiler->setCompileConfiguration("Debug");
	compiler->setRdc(true);
	compiler->setUseFastMath(true);
	compiler->addIncludeDir("E:\\_dev\\_Libraries\\NVIDIA\\OptiX51\\SDK\\cuda");
	compiler->addOptiXIncludeDir("E:\\_dev\\_Libraries\\NVIDIA\\OptiX51\\include");
	compiler->addIncludeDir("C:\\Program Files\\NVIDIA GPU Computing Toolkit\\CUDA\\v8.0\\include");
	std::string ptx = compiler->compileStr("E:\\_dev\\_Libraries\\NVIDIA\\OptiX51\\SDK\\cuda\\phong.cu");
#endif // CERTH

	//LOG_DEBUG << ptx;
	return std::getchar();
}