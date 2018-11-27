#define NOMINMAX

// logging
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Log.h>

#include <filesystem/filesystem.h>

#include <memory>

int main(int argc, char* argv[]) {
	//-----------------
	// Logging Related
	//-----------------
	plog::ColorConsoleAppender<plog::TxtFormatter> app;
	plog::init(plog::debug, &app);

	std::shared_ptr<fs::Filesystem> fsystem =
		std::make_shared<fs::Filesystem>();
	fsystem->load3DFile(std::string("D:\\_Media\\3Dmodels\\nbbeg1gg19-Iron_Man_update1\\Iron_Man_update1\\Iron_Man.obj"));
	std::getchar();
	return 0;
}