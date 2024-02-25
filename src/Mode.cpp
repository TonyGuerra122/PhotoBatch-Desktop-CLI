#include "Mode.h"
#include "ArgumentParser.h"
#include "RenameMode.h"
#include "ConvertMode.h"
#include "StringUtils.h"

#include <iostream>
#include <array>
#include <filesystem>
#include <chrono>

using namespace std;

static constexpr const char* VERSION = "1.0";


Mode::Mode(const std::string& filter, const std::string& folder)
	: m_Filter{ filter }
	, m_Folder{ folder }
{}

const std::string& Mode::GetFilter() const {
	return m_Filter;
}

const std::string& Mode::GetFolder() const{
	return m_Folder;
}

void Mode::Run() {

	using ClockT = chrono::high_resolution_clock;

	// Ler o tempo de execução neste ponto
	ClockT::time_point startTime = ClockT::now();
	RunImpl();
	ClockT::time_point endTime = ClockT::now();

	ClockT::duration elapsedTime = endTime - startTime;
	auto elapsedTimeMs = chrono::duration_cast<chrono::milliseconds>(elapsedTime);

	cout << GetModeName() << "Operação finalizada em " << elapsedTimeMs.count() << "ms" << endl;
}

vector<filesystem::path> Mode::GetFiles(const filesystem::path& extension) const {
	std::vector<std::filesystem::path> files;
	int numSkippedFiles = 0;

	// Coletar todos os arquvos que correspondem ao filtro especificado
	for (const auto& entry : std::filesystem::directory_iterator(GetFolder())) {

		const auto bIsFile = std::filesystem::is_regular_file(entry.path());
		const auto bMatchFilter = GetFilter().empty() || (StringUtils::ToLower(entry.path().string()).find(GetFilter()) != std::string::npos);
		const auto bMatchExtension = extension.empty() || (entry.path().extension() == extension);

		if (bIsFile && bMatchFilter) files.push_back(entry.path());
		else numSkippedFiles++;
	}

	std::cout << GetModeName() << "Número de arquivos encontrados: " << files.size() << std::endl;
	std::cout << GetModeName() << "Número de arquivos ignorados: " << numSkippedFiles << std::endl;

	return files;
}

static void PrintHelpMode() {
	system("cls");

	const auto printSpcmArg = [](const string arg1, const string arg2) -> void {
		cout << "	" << flush;
		cout << arg1 << "      " << arg2 << endl;
		};

	cout << "Comandos: " << endl;
	printSpcmArg("PhotoBatch --help", "Mostra todos os comandos do PhotoBatch");
	printSpcmArg("PhotoBatch --rename", "Renomeia o arquivo");
	printSpcmArg("PhotoBatch --convert", "Convete o arquivo para as extensões possíveis (png, jpg), utilize --From= e --To=");
	printSpcmArg("PhotoBatch --resize", "Redimensiona o tamanho da imagem, utilize --Width= e --Height=");
	printSpcmArg("PhotoBatch --scale", "Aumenta ou diminui a escala da imagem, utilize --Amount=");
}

static void PrintVersionMode() {
	system("cls");

	cout << "PhotoBatch Version: " << VERSION << endl;
}

static const string& GetInvalidChars() {
	static const string invalidCharacters = "\\/*?\"<>|:";
	return invalidCharacters;
}

static bool HasInvalidChars(const string& str) {
	return str.find_first_of(GetInvalidChars()) != string::npos;
}

std::unique_ptr<Mode> CreateMode(const ArgumentParser& argParser) {

	// Ler as flags que o ArgumentParser identificou

	const auto bRenameMode = argParser.GetFlag(Args::Flags::Rename);
	const auto bConvertMode = argParser.GetFlag(Args::Flags::Convert);
	const auto bResizeMode = argParser.GetFlag(Args::Flags::Resize);
	const auto bScaleMode = argParser.GetFlag(Args::Flags::Scale);
	const auto bHelpMode = argParser.GetFlag(Args::Flags::Help);
	const auto bVersionMode = argParser.GetFlag(Args::Flags::Version);

	const array<bool, 6> modes = { bRenameMode, bConvertMode, bResizeMode, bScaleMode, bHelpMode, bVersionMode };
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

		return std::make_unique<RenameMode>(filter, folder, prefix, startNumber);
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

		const map<string, ConvertMode::Format> convertOptionsMap = {
			{ "jpg", ConvertMode::Format::JPG },
			{ "png", ConvertMode::Format::PNG }
		};

		return make_unique<ConvertMode>(filter, folder, convertOptionsMap.at(from), convertOptionsMap.at(to));
	}


	// Criação do modo Help
	if (bHelpMode) PrintHelpMode();

	// Criação do modo Version
	if (bVersionMode) PrintVersionMode();


	return nullptr;
}