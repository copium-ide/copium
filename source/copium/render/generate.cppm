module;
#include <bgfx/bgfx.h>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <unistd.h>
export module copium.render.generate;

export namespace ShaderLoader {
    // Get the directory containing the executable
    std::string getExecutableDir()
    {
    #ifdef _WIN32
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        std::string exePath(path);
        size_t lastSlash = exePath.find_last_of('\\');
        if (lastSlash != std::string::npos)
        {
            return exePath.substr(0, lastSlash + 1);
        }
    #elif __APPLE__
        char path[1024];
        uint32_t size = sizeof(path);
        if (_NSGetExecutablePath(path, &size) == 0)
        {
            std::string exePath(path);
            size_t lastSlash = exePath.find_last_of('/');
            if (lastSlash != std::string::npos)
            {
                return exePath.substr(0, lastSlash + 1);
            }
        }
    #else  // Linux
        char path[1024];
        ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
        if (count != -1)
        {
            path[count] = '\0';
            std::string exePath(path);
            size_t lastSlash = exePath.find_last_of('/');
            if (lastSlash != std::string::npos)
            {
                return exePath.substr(0, lastSlash + 1);
            }
        }
    #endif
        return "./";
    }
    
    bgfx::ShaderHandle loadShader(const char* filename)
    {
        std::string fullPath = getExecutableDir() + filename;
        std::ifstream file(fullPath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            return BGFX_INVALID_HANDLE;
        }
        
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        // Allocate bgfx memory
        const bgfx::Memory* mem = bgfx::alloc(uint32_t(size + 1));
        if (file.read((char*)mem->data, size))
        {
            mem->data[mem->size - 1] = '\0';
            return bgfx::createShader(mem);
        }
        
        return BGFX_INVALID_HANDLE;
    }
    
    bgfx::ProgramHandle loadShaderProgram(const char* vsName, const char* fsName)
    {
        bgfx::ShaderHandle vsh = loadShader(vsName);
        if (!bgfx::isValid(vsh))
        {
            return BGFX_INVALID_HANDLE;
        }
        
        bgfx::ShaderHandle fsh = loadShader(fsName);
        if (!bgfx::isValid(fsh))
        {
            bgfx::destroy(vsh);
            return BGFX_INVALID_HANDLE;
        }
        
        // createProgram takes ownership of shaders and will destroy them
        return bgfx::createProgram(vsh, fsh, true);
    }
    
    // Check if shader file exists
    bool shaderExists(const char* filename)
    {
        std::string fullPath = getExecutableDir() + filename;
        std::ifstream file(fullPath);
        return file.good();
    }
}