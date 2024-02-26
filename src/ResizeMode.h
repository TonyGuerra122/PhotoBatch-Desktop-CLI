#pragma once

#include "Mode.h"

class ResizeMode : public Mode {
public:
	ResizeMode(const std::string& filter,
		const std::string& folder,
		int width = -1,
		int height = -1);
	const std::string& GetModeName() const override;

protected:
	void RunImpl() override;

	void ResizeImage(const std::filesystem::path& filepath, int newWidth, int newHeight) const;
private:
	int m_Width;
	int m_Height;
};