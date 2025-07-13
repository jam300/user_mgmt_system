/*#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

void init_logger()
{
    try
    {
        std::cout << "en init_logger.\n";
        fs::create_directory("logs");
        auto log_path = "logs/app.log";
        std::cout << "en antes del file_sink.\n";
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path, true);
        std::cout << "despues del filesink.\n";
        auto logger = std::make_shared<spdlog::logger>("file_logger", file_sink);
        if (!logger) {
            // Manejar el error: el logger no se creó correctamente
            std::cerr << "Error: No se pudo crear el logger." << std::endl;
            return; // O lanzar una excepción, etc.
        }
        std::cout << "jam1.\n";
        spdlog::set_default_logger(logger);
        std::cout << "jam2.\n";
        spdlog::set_level(spdlog::level::info);
        std::cout << "jam3.\n";
        spdlog::flush_on(spdlog::level::info);
        std::cout << "final\n";
        spdlog::info("✅ Logger initialized, writing to {}", log_path);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
    }
}*/
#include <iostream>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem> // Para std::filesystem

void init_logger() {
    try {
        std::cout << "en init_logger().\n";

        // 1. Asegúrate de que el directorio 'logs' exista
        std::filesystem::path log_dir = "logs";
        if (!std::filesystem::exists(log_dir)) {
            if (!std::filesystem::create_directories(log_dir)) {
                std::cerr << "Error: No se pudo crear el directorio 'logs'.\n";
                return; // Salir si no se puede crear el directorio
            }
        }

        std::string log_path = "logs/app.log";
        std::cout << "auto log_path = \"" << log_path << "\"\n";

        std::cout << "en antes del file_sink.\n";
        // 2. Crear el file_sink
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path, true);
        std::cout << "despues del filesink.\n";

        // 3. Crear el logger
        auto logger = std::make_shared<spdlog::logger>("file_logger", file_sink);
        std::cout << "auto logger = spdlog::make_shared<spdlog::logger>(\"file_logger\", file_sink);\n";

        // 4. Verificar si el logger se creó correctamente
        if (!logger) {
            std::cerr << "Error: El logger no se pudo inicializar. Posible problema con el sink o permisos.\n";
            return; // Salir si el logger es nulo
        }

        std::cout << "jam1.\\n";
        // 5. Establecer el logger por defecto
        //spdlog::set_default_logger(logger); // Esta es la línea con el problema
        spdlog::register_logger(logger);
        std::cout << "jam2.\\n";
        spdlog::set_level(spdlog::level::info);
        spdlog::flush_on(spdlog::level::info);
        std::cout << "final\\n";

        spdlog::info("Logger initialized, writing to {}", log_path);

    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
    }
}