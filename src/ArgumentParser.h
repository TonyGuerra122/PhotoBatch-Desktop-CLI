#pragma once

#include <string>
#include <map>
class ArgumentParser {
public:
	// Registra uma flag usada durante o parse
	void RegisterFlag(const std::string& flag);

	// Registra uma opção usada durante o parse
	void RegisterOption(const std::string& option);

	// Obtem o valor de uma flag obtida durante o parse
	bool GetFlag(const std::string& flag) const;

	// Obtem o valor de uma opção convertida como um tipo especificado
	template<typename T>
	T GetOptionAs(const std::string& option) const;

	template<>
	std::string GetOptionAs(const std::string& option) const { return GetOption(option); }

	template<>
	float GetOptionAs(const std::string& option) const { return GetOptionAsFloat(option); }

	template<>
	int GetOptionAs(const std::string& option) const { return GetOptionAsInt(option); }

	// Faz o parse da lista de argumentos em argv
	void Parse(int argc, char* argv[]);
private:
	std::map<std::string, bool> m_Flags;
	std::map<std::string, std::string> m_Options;

	// Obter o valor de uma opção lida durante o parse
	const std::string& GetOption(const std::string& option) const;

	// Obter o valor de uma opção convertida como float
	float GetOptionAsFloat(const std::string& option) const;

	// Obter o valor de uma opção convertida como int
	int GetOptionAsInt(const std::string& option) const;
};