// FileCopier.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <chrono>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>

using namespace std::chrono_literals;

static auto const epoch = std::chrono::steady_clock::time_point{ 0s };

static void waitForFile(std::filesystem::path const& filename)
{
    auto const filename_str = filename.string();

    while (true)
    {
        auto const handle = CreateFileA(
            filename_str.c_str(),
            GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (handle != INVALID_HANDLE_VALUE)
        {
            (void)CloseHandle(handle);
            return;
        }

        auto const error = GetLastError();
        if (error != ERROR_SHARING_VIOLATION)
        {
            throw std::runtime_error{ "Failed to open file. Error " + std::to_string(error) };
        }

        std::this_thread::sleep_for(500ms);
    }
}

int run(int, char* []);

int main(int argc, char* argv[])
{
    try
    {
        return run(argc, argv);
    }
    catch (std::exception const& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}

int run(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: FileCopier.exe <Output directory> <File to copy>" << std::endl;
        return 1;
    }

    auto const output_directory = std::filesystem::path{ argv[1] };
    auto const filename = std::filesystem::path{ argv[2] };

    std::filesystem::remove_all(output_directory);
    std::filesystem::create_directory(output_directory);

    while (true)
    {
        std::cout << "Press c to copy or q to quit..." << std::endl;
        auto command = std::string{};
        std::cin >> command;

        if (command.empty())
        {
            continue;
        }

        auto const commandChar = command[0];
        if (commandChar == 'c' || commandChar == 'c')
        {
            std::cout << "Waiting for file...";
            std::cout.flush();

            waitForFile(filename);
            std::cout << " Done" << std::endl;

            auto diff = std::chrono::steady_clock::now() - epoch;
            auto output_file = output_directory / std::to_string(diff.count());
            std::filesystem::copy(filename, output_file, std::filesystem::copy_options::overwrite_existing);
            auto const file_size = std::filesystem::file_size(output_file);
            std::cout << "Copied to file \"" << output_file << "\" with size " << file_size << std::endl;
        }
        else if (commandChar == 'q' || commandChar == 'Q')
        {
            break;
        }
    }

    return 0;
}

