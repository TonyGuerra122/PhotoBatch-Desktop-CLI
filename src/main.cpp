#include <iostream>

#include "ArgumentParser.h"
#include "Mode.h"


int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "pt_BR");
	setlocale(LC_NUMERIC, "en_US");

	ArgumentParser argParser;
	// Registra as flags do programa
	argParser.RegisterFlag(Args::Flags::Rename);
	argParser.RegisterFlag(Args::Flags::Convert);
	argParser.RegisterFlag(Args::Flags::Resize);
	argParser.RegisterFlag(Args::Flags::Scale);
	argParser.RegisterFlag(Args::Flags::Help);
	argParser.RegisterFlag(Args::Flags::Version);

	// Registra as opções do programa
	argParser.RegisterOption(Args::Options::Folder);
	argParser.RegisterOption(Args::Options::Filter);
	argParser.RegisterOption(Args::Options::Width);
	argParser.RegisterOption(Args::Options::Height);
	argParser.RegisterOption(Args::Options::Amount);
	argParser.RegisterOption(Args::Options::Prefix);
	argParser.RegisterOption(Args::Options::StartNumber);
	argParser.RegisterOption(Args::Options::From);
	argParser.RegisterOption(Args::Options::To);

	argParser.Parse(argc, argv);

	try {
		if (std::unique_ptr<Mode> photoBatchMode = CreateMode(argParser)) {
			photoBatchMode->Run();
		}
	}
	catch (const std::exception& exception) {
		std::cerr << exception.what() << std::endl;
	}


	return 0;
}