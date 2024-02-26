#pragma once

#include "ResizeMode.h"

class ScaleMode final : public ResizeMode {
public:
	ScaleMode(const std::string& filter,
		const std::string& folder,
		float scaleFactor);
	const std::string& GetModeName() const override;
protected:
	void RunImpl() override;
private:
	float m_ScaleFactor;
};