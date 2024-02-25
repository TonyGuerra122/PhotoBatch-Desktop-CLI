#include "RenameMode.h"
#include "StringUtils.h"

#include <iostream>
#include <filesystem>
#include <vector>

using namespace StringUtils;

RenameMode::RenameMode(const std::string& filter, const std::string& folder, const std::string& prefix, int startNumber)
	: Mode{ filter, folder }
	, m_Prefix{ prefix }
	, m_StartNumber{ startNumber }
{
}

const std::string& RenameMode::GetModeName() const {
	static const std::string renameModeName = "[Renomear]: ";
	return renameModeName;
}

void RenameMode::RunImpl() {
	std::cout << GetModeName() << "Modo : Renomear" << std::endl;
	std::cout << GetModeName() << "Pasta : " << GetFolder() << std::endl;
	std::cout << GetModeName() << "Filtro : " << GetFilter() << std::endl;
	std::cout << GetModeName() << "Prefixo : " << m_Prefix << std::endl;
	std::cout << GetModeName() << "Número inicial : " << m_StartNumber << std::endl;

	

	// Iniciar o processo de renomear os arquivos

	//"yo.jpg" -> "PrefixoN.jpg"
	int currentNumber = m_StartNumber;
	bool success = false;
	for (const auto& filepath : GetFiles()) {
		const auto extension = filepath.extension();
		const auto newFileName = m_Prefix + std::to_string(currentNumber) + extension.string();

		std::filesystem::path newFilepath = filepath;
		newFilepath.replace_filename(newFileName);

		try {
			std::filesystem::rename(filepath, newFilepath);
			success = true;
		}
		catch (const std::exception& exception) {
			std::cerr << GetModeName() << "Erro ao renomear \"" << filepath << "\":" << exception.what() << std::endl;
			success = false;
		}
		if(success) std::cout << GetModeName() << filepath << " -> " << newFilepath << std::endl;

		currentNumber++;
	}
}