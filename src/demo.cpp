#include <chat.h>
#include <chrono>
#include <iostream>
#include <thread>

static chat::Args parse_args(const std::vector<std::string> &argv)
{
    chat::Args args;

    for (size_t i = 1; i < argv.size(); i++)
    {
        const std::string &arg = argv.at(i);

        if (arg == "--model_path")
        {
            args.model_path = argv.at(++i);
        }
        else if (arg == "--prompt")
        {
            args.prompt = argv.at(++i);
        }
    }

    return args;
}

static chat::Args parse_args(int argc, char **argv)
{
    std::vector<std::string> argv_vec;
    argv_vec.reserve(argc);

#ifdef _WIN32
    LPWSTR *wargs = CommandLineToArgvW(GetCommandLineW(), &argc);
    chatGLM_CHECK(wargs) << "failed to retrieve command line arguments";

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    for (int i = 0; i < argc; i++)
    {
        argv_vec.emplace_back(converter.to_bytes(wargs[i]));
    }

    LocalFree(wargs);
#else
    for (int i = 0; i < argc; i++)
    {
        argv_vec.emplace_back(argv[i]);
    }
#endif

    return parse_args(argv_vec);
}

class LocalEmitter: public chat::Emitter
{
public:
    void emit(std::string type, std::string msg)
    {
        std::cout << type << msg << std::endl;
    };
};

int main(int argc, char **argv)
{
    chat::Args args = parse_args(argc, argv);
    LocalEmitter emitter;
    chat::chat(emitter, args);
    return 0;
}