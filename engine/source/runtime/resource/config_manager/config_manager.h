#pragma once

#include <filesystem>

namespace Polaris
{
    struct EngineInitParams;

    class ConfigManager
    {
    public:
        void initialize(const std::filesystem::path& config_file_path);

        const std::filesystem::path& getRootFolder() const;

        const std::string& getDefaultWorldUrl() const;

    private:
        std::filesystem::path m_root_folder;

        std::string m_default_world_url;
    };
} // namespace Polaris