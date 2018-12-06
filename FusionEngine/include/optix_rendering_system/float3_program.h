#ifndef INCLUDE_OPTIX_RENDERING_SYSTEM_FLOAT3_PROGRAM_H
#define INCLUDE_OPTIX_RENDERING_SYSTEM_FLOAT3_PROGRAM_H
/// Fusion
#include <optix_rendering_system/rt_program.h>
#include <comm_system/float3_program_message.h>
#include <ptx_compiler/ptx_compiler.h>
/// STL
#include <string>
#include <functional>
#include <memory>
namespace rt {
	/**
	*	The simple (one bg color) Programs (Exception, SimpleMiss, SolidColor) will
	*	inherit from Float3Program class. So there will be 2 levels of
	*	inheritance (which I don't like) but this design automates a lot of
	*	stuff for these kind of Programs.
	*
	*	Represents OptiX Programs with only one optix::float3 parameter
	*/
	class Float3Program : public RTProgram {
	public:
		explicit Float3Program() { }
		explicit Float3Program(
			optix::Context& ctx,
			const std::string&  name,
			const std::string& ptxPath,
			const std::string& cuPath,
			const optix::float3& val,
			const std::shared_ptr<ptx::PTXCompiler> compiler);
		const optix::float3 val() const { return mVal; }
	protected:
		void setValue(const optix::float3& val);
	private:
		optix::float3 mVal;
	};

	/**
	*	Constructor
	*/
	Float3Program::Float3Program(optix::Context& ctx,
		const std::string&  name,
		const std::string& ptxPath,
		const std::string& cuPath,
		const optix::float3& val,
		std::shared_ptr<ptx::PTXCompiler> compiler)
		: RTProgram(ctx, name, ptxPath, cuPath, compiler), mVal(val)
	{ }

	/**
	*	Sets value
	*/
	void Float3Program::setValue(const optix::float3& val) {
		mVal = val;
	}
}	//	!namespace rt
#endif // !INCLUDE_OPTIX_RENDERING_SYSTEM_FLOAT3_PROGRAM_H

