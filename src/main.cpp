#include <iostream>
#include <iomanip>
#include <array>
#include <filesystem>

#include "ArgumentParser.h"

using namespace std;

namespace Args {
	namespace Flags {
		static constexpr const char* Rename = "rename";
		static constexpr const char* Convert = "convert";
		static constexpr const char* Resize = "resize";
		static constexpr const char* Scale = "scale";
	}
	namespace Options {
		static constexpr const char* Folder = "folder";
		static constexpr const char* Filter = "filter";
		static constexpr const char* Width = "width";
		static constexpr const char* Height = "height";
		static constexpr const char* Amount = "amount";
		static constexpr const char* Prefix = "prefix";
		static constexpr const char* StartNumber = "startnumber";
		static constexpr const char* From = "from";
		static constexpr const char* To = "to";
	}
}

static const string& GetInvalidChars() {
	static const string invalidCharacters = "\\/*?\"<>|:";
	return invalidCharacters;
}

static bool HasInvalidChars(const string& str) {
	return str.find_first_of(GetInvalidChars()) != string::npos;
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

	// Validar a pasta passada como a opção folder
	const auto folder = argParser.GetOptionAs<string>(Args::Options::Folder);
	if (folder.empty()) throw invalid_argument("A pasta não pode estar em branco");

	if (!filesystem::exists(folder)) throw invalid_argument("A pasta não existe");

	// Validar se o filtro é uma string válida

	const auto filter = argParser.GetOptionAs<string>(Args::Options::Filter);
	if (!filter.empty() && HasInvalidChars(filter)) throw invalid_argument("O filtro não pode conter " + GetInvalidChars());

	// Validar o modo Resize
	if (bResizeMode) {

		int width = 0;
		int height = 0;

		try {
			width = argParser.GetOptionAs<int>(Args::Options::Width);
			height = argParser.GetOptionAs<int>(Args::Options::Height);
		}
		catch (const invalid_argument&) {
			throw invalid_argument("O valor informado para Width ou Height não são números válidos");
		}

		// No modo resize o comprimento e a altura devem ser maior que zero
		if (width <= 0 || height <= 0) throw invalid_argument("Width e Height devem ser maiores que zero");

		if (filter.empty()) throw invalid_argument("Filter não pode estar em branco no modo resize");
	}

	// Validar o modo scale
	if (bScaleMode) {
		float amount = 0.0f;

		try {
			amount = argParser.GetOptionAs<float>(Args::Options::Amount);
		}
		catch (const invalid_argument&) {
			throw invalid_argument("O valor informado para Amount não é um número válido");
		}


		// No modo escala o amount deve ser maior do que zero
		if (amount <= 0.0f) throw invalid_argument("Amount deve ser maior do que zero");
		if (filter.empty()) throw invalid_argument("Filter não pode estar em branco no modo scale");

	}

	// Validar o modo Rename
	if (bRenameMode) {
		int startNumber = -1;

		try {
			startNumber = argParser.GetOptionAs<int>(Args::Options::StartNumber);
		}
		catch (const invalid_argument&) {
			throw invalid_argument("O valor informado para StartNumber não é um número válido");
		}

		auto prefix = argParser.GetOptionAs<string>(Args::Options::Prefix);

		// No modo Rename o StartNumber deve ser maior que zero
		if (startNumber < 0) throw invalid_argument("StartNumber deve ser maior ou igual a zero");
		if (prefix.empty() || HasInvalidChars(prefix)) throw invalid_argument("Prefix não pode estar em branco e não pode contar os caracters: " + GetInvalidChars());
	}

	// Validar o modo Convert
	if (bConvertMode) {
		const auto from = argParser.GetOptionAs<string>(Args::Options::From);
		const auto to = argParser.GetOptionAs<string>(Args::Options::To);
		const array<string, 2> convertOptions = { "jpg", "png" };

		const auto bIsFromValid = find(begin(convertOptions), end(convertOptions), from) != end(convertOptions);
		const auto bIsToValid = find(begin(convertOptions), end(convertOptions), to) != end(convertOptions);

		if (from.empty() || to.empty()) throw invalid_argument("É necessário especificar o From e To");

		if (!bIsFromValid || !bIsToValid) throw invalid_argument("From e To devem ser jpg ou png");

		if (from == to) throw invalid_argument("From e To devem ser diferentes");
	}
}

int main(int argc, char* argv[]) {

	setlocale(LC_ALL, "pt_BR");
	setlocale(LC_NUMERIC, "en_US");

	ArgumentParser argParser;
	// Registra as flags do programa
	argParser.RegisterFlag(Args::Flags::Rename);
	argParser.RegisterFlag(Args::Flags::Convert);
	argParser.RegisterFlag(Args::Flags::Resize);
	argParser.RegisterFlag(Args::Flags::Scale);

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
		ValidateArguments(argParser);
	}
	catch (const exception& exception) {
		cerr << exception.what() << endl;
	}

	return 0;
}