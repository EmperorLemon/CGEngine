#pragma once

int main(int argc, char* argv[]);

namespace CG
{
	class Application
	{
	private:
		friend int ::main(int argc, char* argv[])
	};
}