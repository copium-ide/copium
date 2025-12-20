module;
#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
export module copium.logging;

import copium.fs.paths;

export namespace Logging {

uint8_t printLevel = 3;

// Print function with level filtering
void print(std::string tag, uint8_t level, std::string content) {
    if (level < printLevel) {
        std::cout << "[" << tag << "](" << static_cast<int>(level) << ") " << content << std::endl;
    }
}

/**
    * @brief logs text to a file
    * 
    * @param tag appears as [tag]...
    * @param level debug level, only appears in the log if the level is less than printLevel
    * @param content shows as [...] content
    * @param filepath path 
    */
void log(std::string tag, uint8_t level, std::string content, std::string filepath)
{
    if (Paths::isFile(filepath))
    {
        if (level < printLevel)
        {
            std::ofstream logFile(filepath, std::ios_base::app);
            if (logFile.is_open())
            {
                logFile << "[" << tag << "](" << static_cast<int>(level) << ") " << content << std::endl;
                logFile.close();
            }
        }
    }
}
}