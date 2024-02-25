#include <iostream>
#include <iomanip>
#include <array>
#include "ArgumentParser.h"

using namespace std;

namespace Args {
	namespace Flags {
		static constexpr const char* Rename = "rename";
		static constexpr const char* Convert = "convert";
		static constexpr const char* Resize = "resize";
		static constexpr const char* Scale = "scale";
	}
}

static void ValidateArguments(const ArgumentParser& argParser) {

	// Ler as flags que o ArgumentParser identificou

	const bool bRenameMode = argParser.GetFlag(Args::Flags::Rename);
	const bool bConvertMode = argParser.GetFlag(Args::Flags::Convert);
	const bool bResizeMode = argParser.GetFlag(Args::Flags::Resize);
	const bool bScaleMode = argParser.GetFlag(Args::Flags::Scale);

	const array<bool, 4> modes = { bRenameMode, bConvertMode, bResizeMode, bScaleMode };
	const ptrdiff_t numActiveModes = count(begin(modes), end(modes), true);

	// Verificar se somente um modo do PhotoBatch está ativo
	if (numActiveModes != 1) {

		// Se houver mais de um modo ativo, lança uma exceção

		throw invalid_argument("Somento um modo pode estar ativo");
	}
}

int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "pt_BR");
	setlocale(LC_NUMERIC, "en_US");

	ArgumentParser argParser;
	argParser.RegisterFlag(Args::Flags::Rename);
	argParser.RegisterFlag(Args::Flags::Convert);
	argParser.RegisterFlag(Args::Flags::Resize);
	argParser.RegisterFlag(Args::Flags::Scale);

	argParser.Parse(argc, argv);

	try {
		ValidateArguments(argParser);
	}
	catch (const exception& exception) {
		cerr << exception.what() << endl;
	}

	return 0;
}