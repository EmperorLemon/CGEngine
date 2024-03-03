#include <CGEngine.h>

int main(const int argc, char* argv[])
{
	CGEngine::Logger::Init();

	CGEngine::ApplicationCreateInfo createInfo;
	{
		createInfo.cmdArgs = { argc, argv };
	}

	auto app = CGEngine::Application(createInfo);
	app.Run();
	app.Quit();
}