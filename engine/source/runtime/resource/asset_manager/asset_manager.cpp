#include "runtime/resource/asset_manager/asset_manager.h"

#include "runtime/function/global/global_context.h"

#include <filesystem>

namespace Polaris
{
    std::filesystem::path AssetManager::getFullPath(const std::string& relative_path) const
    {
        return  std::filesystem::absolute(relative_path);
    }
} // namespace Polaris