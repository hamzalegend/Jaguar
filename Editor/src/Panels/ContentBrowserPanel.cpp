#include "ContentBrowserPanel.h"

#include "imguiLayer/ImguiLayer.h"

#include <filesystem>

ContentBrowserPanel::ContentBrowserPanel(std::string path)
	:m_assetspath(path), m_currentDir(path)
{
}

ContentBrowserPanel::~ContentBrowserPanel()
{
}

void ContentBrowserPanel::Draw()
{
	ImGui::Begin("Content Browser");

	if (m_assetspath != m_currentDir)
	{
		if (ImGui::Button("<-"))
			m_currentDir = m_currentDir.parent_path();
	}
	ImGui::Text(m_currentDir.string().c_str());

	for (auto& p : std::filesystem::directory_iterator(m_currentDir))
	{
		const auto& path = p.path();
		auto relativePath = std::filesystem::relative(path, m_assetspath);
		auto filenameString = relativePath.filename().string();


		if (p.is_directory())
		{
			if (ImGui::Button(filenameString.c_str()))
			{
				m_currentDir /= path.filename();
			}
		}
		else
		{
			if (ImGui::Button(filenameString.c_str()))
			{
			}
		}

		if (ImGui::BeginDragDropSource())
		{ 
			const wchar_t* itemPath = relativePath.c_str();
			int size = (wcslen(itemPath) + 1) * sizeof(wchar_t);
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, size);
			ImGui::EndDragDropSource();
		}
	}

	ImGui::End();
}
