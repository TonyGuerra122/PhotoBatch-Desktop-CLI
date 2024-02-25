#pragma once

#include <string>
#include <memory>

class Mode {
public:

	Mode(const std::string& filter, const std::string& folder);

	const std::string& GetFilter() const;
	const std::string& GetFolder() const;

	virtual const std::string& GetModeName() const = 0;

	void Run();

protected:
	virtual void RunImpl() = 0;

private:
	std::string m_Folder;
	std::string m_Filter;
};

namespace Args {
	namespace Flags {
		static constexpr const char* Rename = "rename";
		static constexpr const char* Convert = "convert";
		static constexpr const char* Resize = "resize";
		static constexpr const char* Scale = "scale";
		static constexpr const char* Help = "help";
		static constexpr const char* Version = "version";
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

class ArgumentParser;

std::unique_ptr<Mode> CreateMode(const ArgumentParser& argParser);