module;
#include <filesystem>
#include <string>
#include <vector>
export module copium.fs.paths;

export namespace Paths {

    std::filesystem::path
    getDir()
    {
        return std::filesystem::current_path();
        // TODO: Make sure this returns the path of the application executable (this app)
    }

    std::string
    getMyName()
    {
        return std::filesystem::current_path().filename().string();
    }

    std::filesystem::path
    getWorkingDirectory()
    {
        return std::filesystem::current_path();
        // TODO: Make sure this returns the path that the console shell is currently in
    }

    bool
    isFile(const std::filesystem::path& filepath)
    {
        return std::filesystem::is_regular_file(filepath);
    }

    bool
    isDir(const std::filesystem::path& filepath)
    {
        return std::filesystem::is_directory(filepath);
    }

    std::vector<std::filesystem::path>
    listContents(const std::filesystem::path& filepath)
    {
        std::vector<std::filesystem::path> contents;
        for (const auto& entry : std::filesystem::directory_iterator(filepath)) {
            contents.push_back(entry.path());
        }
        return contents;
    }

    std::vector<std::filesystem::path>
    listDirectories(const std::filesystem::path& filepath)
    {
        std::vector<std::filesystem::path> directories;
        for (const auto& entry : std::filesystem::directory_iterator(filepath)) {
            if (std::filesystem::is_directory(entry)) {
                directories.push_back(entry.path());
            }
        }
        return directories;
    }

    std::vector<std::filesystem::path>
    listFiles(const std::filesystem::path& filepath)
    {
        std::vector<std::filesystem::path> files;
        for (const auto& entry : std::filesystem::directory_iterator(filepath)) {
            if (std::filesystem::is_regular_file(entry)) {
                files.push_back(entry.path());
            }
        }
        return files;
    }

    std::filesystem::path
    getRelativeDirectory(const std::filesystem::path& base, const std::filesystem::path& target)
    {
        return std::filesystem::relative(target, base);
    }

    std::filesystem::path
    getAbsoluteDirectory(const std::vector<std::filesystem::path>& paths)
    {
        std::filesystem::path resolvedPath;
        for (const auto& path : paths) {
            resolvedPath /= path;
        }
        return std::filesystem::absolute(resolvedPath);
    }
}
