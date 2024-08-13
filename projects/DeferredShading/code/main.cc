//------------------------------------------------------------------------------
// main.cc
// (C) 2015-2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "DeferredShading.h"
#include "core/meshResource/MeshResource.h"

//------------------------------------------------------------------------------
/**
*/

int
main(int argc, const char** argv)
{
	Example::ExampleApp app;

	
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();	
	
	return app.ExitCode();
	return 0;
}