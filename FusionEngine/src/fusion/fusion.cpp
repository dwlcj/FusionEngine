#define NOMINMAX
#define GLEW_STATIC

/// OptiX
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_aabb_namespace.h>
/// logging
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Log.h>
/// rx
#include <rxcpp/rx.hpp>
/// Argument parsing
#include <cli/CLI11.hpp>
/// imgui
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>
#include <imgui/ImguiWindowsFileIO.hpp>
#include <imgui/ImguiWindowsAboutIGS.hpp>
/// glew & glfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>
/// Fusion
/// Filesystem
#include <filesystem/filesystem.h>
/// Scene Mapper
#include <optix_mapping_system/scene_mapper.h>
/// Scene Tree
#include <optix_scene_hierarchy/scene_tree.h>
/// PTX Compiler
#include <ptx_compiler/ptx_compiler.h>
/// Renderer
#include <optix_rendering_system/optix_rendering_system.h>
#include <optix_rendering_system/solid_color_program.h>
#include <optix_rendering_system/exception_program.h>
#include <optix_rendering_system/simple_miss_program.h>
/// UI
#include <editor_ui/menu_bar.h>
#include <editor_ui/file_menu.h>
#include <editor_ui/edit_menu.h>
#include <editor_ui/ptx_compiler_widget.h>
#include <editor_ui/viewport_widget.h>
/// STL
#include <string>
#include <memory>

/**
*	Globals
*/
static std::size_t AppWidth = 1440;
static std::size_t AppHeight = 720;
static const ImVec4 clearColor(0.2f, 0.2f, 0.2f, 1.0f);
/// Programs Related
optix::float3 gSolidColorProgVal = optix::make_float3(0.2f, 0.2f, 0.2f);
optix::float3 gExceptionProgVal = optix::make_float3(1.0f, 0.0f, 0.0f);
optix::float3 gSimpleMissProgVal = optix::make_float3(0.5f, 0.8f, 0.8f);

// GLFW error callback
static void glfw_error_callback(int error, const char* description) {
	LOG_ERROR << "GLFW Error(" << error << "): " << description;
}

int main(int argc, char* argv[]) {
	/*******************
	*	Logging Related
	********************/
	plog::ColorConsoleAppender<plog::TxtFormatter> logger;
	plog::init(plog::debug, &logger);

	/********************
	*	Argument Parsing
	*********************/
	//CLI::App cli{ "360Fusion: 4D - 360 video Fusion" };
	
	/*******************************
	*	Graphics: GL-Imgui Related
	********************************/
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		LOG_ERROR << "Cannot initialize Graphics.";
		return -1;
	}
	GLFWwindow* window = glfwCreateWindow(AppWidth, AppHeight, "360Fusion", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glewInit();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& imguiIO = ImGui::GetIO();
	(void)imguiIO;
	imguiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
	auto& imguiStyle = ImGui::GetStyle();
	imguiStyle.FrameRounding = 5.5f;
	imguiStyle.ChildRounding = 5.0f;
	imguiStyle.ColumnsMinSpacing = 0.0f;
	imguiStyle.FrameBorderSize = 1.0f;
	imguiStyle.ScrollbarRounding = 2.0f;
	imguiStyle.WindowBorderSize = 0.5f;
	imguiStyle.WindowRounding = 0.0f;
	imguiStyle.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	ImFont* font = imguiIO.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Calibri.ttf", 11.0f, NULL, NULL);
	IM_ASSERT(font != NULL);

	/**
	*	Initialize GL state
	*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// TODO: Modify so that Viewport is in the middle of the application
	glViewport(0, 0, AppWidth, AppHeight);

	/********************
	*	Fusion Related
	********************/
	/// OptiX context
	optix::Context ctx = optix::Context::create();
	/// Filesystem
	std::shared_ptr<fs::Filesystem> filesystem =
		std::make_shared<fs::Filesystem>();
	/// PTX Compiler
	// TODO: Maybe do not use hard coded paths (except for cuda)
	std::shared_ptr<ptx::PTXCompiler> ptxCompiler =
		std::make_shared<ptx::PTXCompiler>();
	ptxCompiler->setCudaArch(30);
	ptxCompiler->setHostPlatform("x64");
	ptxCompiler->setCompileConfiguration("Debug");
	ptxCompiler->setRdc(true);
	ptxCompiler->setUseFastMath(true);
	ptxCompiler->addOptiXIncludeDir("..\\..\\src\\ptx_mapping_system\\CUDA");
	ptxCompiler->addIncludeDir("..\\src\\CUDA");
#ifdef CERTH
	ptxCompiler->addIncludeDir("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.2/include");
	ptxCompiler->addIncludeDir("D:\\_dev\\_Libraries\\NVIDIA\\OptiX51\\include");
#else
	ptxCompiler->addIncludeDir("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v8.0/include");
	ptxCompiler->addIncludeDir("E:\\_dev\\_Libraries\\NVIDIA\\OptiX51\\include");
#endif // CERTH
	/// OptiX Scene Mapper
	std::shared_ptr<map::SceneMapper> sceneMapper =
		std::make_shared<map::SceneMapper>(ctx);
	/// OPTIX Renderer
	std::shared_ptr<rt::OptiXRenderer> optixRenderer =
		std::make_shared<rt::OptiXRenderer>(ctx, AppWidth, AppHeight, ptxCompiler);
	/// Scene Tree
	std::shared_ptr<tree::SceneTree> sceneTree =
		std::make_shared<tree::SceneTree>(ctx, ptxCompiler, AppWidth, AppHeight);
	/// UI Related
	std::shared_ptr<ui::MenuBar> menuBar =
		std::make_shared<ui::MenuBar>();
	std::shared_ptr<ui::FileMenu> fileMenu =
		std::make_shared<ui::FileMenu>();
	std::shared_ptr<ui::EditMenu> editMenu =
		std::make_shared<ui::EditMenu>();
	menuBar->addItem(fileMenu);
	menuBar->addItem(editMenu);
	/// Widgets
	std::shared_ptr<ui::widget::PTXCompilerWidget> ptxCompilerWidget =
		std::make_shared<ui::widget::PTXCompilerWidget>();
	std::shared_ptr<ui::widget::ViewportWidget> viewportWidget =
		std::make_shared<ui::widget::ViewportWidget>();

	/*****************
	*	Subscriptions
	******************/
	/// PTX Compiler Widget -> PTX Compiler
	ptxCompilerWidget->messageFlowOut().subscribe(ptxCompiler->messageFlowIn());
	/// File Menu -> Filesystem
	fileMenu->messageFlowOut().subscribe(filesystem->messageFlowIn());
	/// Filesystem -> scene Mapper
	//filesystem->sceneFlowOut().subscribe(sceneMapper->sceneFlowIn());
	/// FileSystem -> SceneTree
	filesystem->sceneFlowOut().subscribe(sceneTree->sceneFlowIn());
	/// Scene Mapper -> OptiXRenderer
	sceneMapper->cameraMessageFlowOut().subscribe(optixRenderer->pinholeCameraMessageFlowIn());
	/// Scen Mapper -> OptiX Renderer
	//sceneMapper->topObjectFlowOUt().subscribe(optixRenderer->topObjectFlowIn());
	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar;
	windowFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize;
	windowFlags |= ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar;
	

	/**************
	*	Game Loop
	***************/
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		ImGuiIO& io = ImGui::GetIO();
		int displayWidth, displayHeight;
		glfwGetFramebufferSize(window, &displayWidth, &displayHeight);
		glViewport(0, 0, displayWidth, displayHeight);
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/********************
		*	Do whatevah here
		*********************/
		try {
			ctx->validate();
			optixRenderer->render();
		}
		catch (optix::Exception& ex) {
			LOG_ERROR << ex.getErrorCode() << ": " << ex.getErrorString();
		}
		menuBar->render();
		/// Render Widgets
		//ptxCompilerWidget->render();
		//viewportWidget->render();

		/**********************
		*	End of Whatevah
		**********************/
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}
	/************
	*	Cleanup
	************/
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();

	ctx->destroy();
}
