#pragma once

#include "Panel.h"
#include <filesystem>
class ContentBrowserPanel : public Panel
{
public:
	ContentBrowserPanel(std::string path);
	~ContentBrowserPanel();

	virtual void Draw() override;

private:
	std::filesystem::path m_assetspath;
	std::filesystem::path m_currentDir;
};
