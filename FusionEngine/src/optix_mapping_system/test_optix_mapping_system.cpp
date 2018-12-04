#define NOMINMAX

#include <optix_mapping_system/geometry_mapper.h>
/// logging
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Log.h>

#include <memory>

int main(int argc, char* argv[]) {
	plog::ColorConsoleAppender<plog::TxtFormatter> app;
	plog::init(plog::debug, &app);
	
	optix::Context ctx;

	std::shared_ptr<map::GeometryMapper> geomMapper =
		std::make_shared<map::GeometryMapper>(ctx);

	
	return std::getchar();
}