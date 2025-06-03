/**
 * @file fileIO.h
 * @brief Main file handling class for ediotr
 * 
 * Contains file loading, saving and creating systems
 * 
 * @author Harman Bhogal
 * @date 2025-5-25
 */

#pragma once
#include <string>
#include <fstream>
#include <vector>

/**
 * @class fileIO
 * @brief Handles all file operations for the editor.
 * 
 * Supports:
 * - Loading existing files into memory.
 * - Saving in-memory content to disk.
 * - Creating new empty files.
 * - Checking file existence.
 */
class fileIO{
public:

    /**
     * @brief Loads file content into a vector of strings.
     * @param filepath Path to the target file.
     * @param content Reference to a vector storing loaded lines.
     * @return `true` if successful, `false` on failure (e.g., file not found).
     */
    static bool load(const std::string& filepath, std::vector<std::string>& content);

    /**
     * @brief Saves vector content to a file (each string as a new line).
     * @param filepath Destination file path.
     * @param content Vector of strings to write.
     * @return `true` if saved successfully, `false` on error.
     */
    static bool save(const std::string& filepath, std::vector<std::string>& content);

    /**
     * @brief Creates a new empty file.
     * @param filepath Path for the new file.
     * @return `true` if the file was created, `false` if the operation failed.
     */
    static bool file_create(const std::string& filepath);

    /**
     * @brief Checks if a file exists using `stat()`.
     * @param filepath Path to check.
     * @return `true` if the file exists, `false` otherwise.
     */
    static bool file_exists(const std::string& filepath);
};


