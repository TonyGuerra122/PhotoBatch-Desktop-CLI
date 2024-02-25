#include "ArgumentParser.h"
#include "StringUtils.h"

using namespace StringUtils;

void ArgumentParser::RegisterFlag(const std::string& flag) {
	if (!flag.empty()) ArgumentParser::m_Flags[flag] = false;
}
void ArgumentParser::RegisterOption(const std::string& option) {
	if (!option.empty()) {
		m_Options[option] = "";
	}
}
bool ArgumentParser::GetFlag(const std::string& flag) const{
	if (!flag.empty()) {
		auto flagIt = m_Flags.find(flag);
		if (flagIt != std::end(m_Flags)) return flagIt->second;
	}
	
	return false;
}
const std::string& ArgumentParser::GetOption(const std::string& option) const {
	if (!option.empty()) {
		auto optionIt = m_Options.find(option);
		if (optionIt != std::end(m_Options)) {
			return optionIt->second;
		}
	}

	static auto emptyOption = "";
	return emptyOption;
}

float ArgumentParser::GetOptionAsFloat(const std::string& option) const {
	const auto& optionValue = GetOption(option);
	if (!optionValue.empty()) return std::stof(optionValue);

	return -1;

}

int ArgumentParser::GetOptionAsInt(const std::string& option) const {
	const auto& optionValue = GetOption(option);
	if (!optionValue.empty()) return std::stoi(optionValue);

	return -1;
}


void ArgumentParser::Parse(int argc, char* argv[]) {
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			std::string arg = ToLower(argv[i]);

			if (arg.length() >= 3) {
				// Validando a prefixo da flag
				if (arg[0] == '-' && arg[1] == '-') {
					arg = arg.substr(2);
					
					if (arg.find_first_of('=') != std::string::npos) {
						// Opção
						const size_t equalSingPos = arg.find('=');
						if (equalSingPos != std::string::npos) {
							// Dividia a opção em chave e valor
							auto optionName = arg.substr(0, equalSingPos);
							auto optionValue = arg.substr(equalSingPos + 1);

							auto optionIt = m_Options.find(optionName);
							if (optionIt != std::end(m_Options)) {
								optionIt->second = optionValue;
							}
						}

					}
					else {
						// Flag
						auto flagIt = m_Flags.find(arg);
						if (flagIt != std::end(m_Flags)) {
							// Achou uma flag registrada
							flagIt->second = true;
						}
					}
				}
			}
		}
	}
}