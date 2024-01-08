#include <napi.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <chatglm.h>
#include "chatglm.cpp"

enum InferenceMode
{
    INFERENCE_MODE_CHAT,
    INFERENCE_MODE_GENERATE,
};

struct Args
{
    std::string model_path = "chatglm-ggml.bin";
    InferenceMode mode = INFERENCE_MODE_CHAT;
    std::string prompt = "";
    std::string system = "";
    int max_length = 2048;
    int max_new_tokens = -1;
    int max_context_length = 512;
    int top_k = 0;
    float top_p = 0.7;
    float temp = 0.95;
    float repeat_penalty = 1.0;
    int num_threads = 0;
};

// trim from start (in place)
void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

class EmitterStreamer : public chatglm::BaseStreamer
{
public:
    EmitterStreamer(Napi::Env &env, Napi::Function &callback, chatglm::BaseTokenizer *tokenizer)
        : env_(env), callback_(callback), tokenizer_(tokenizer), is_prompt_(true), is_first_line_(true), print_len_(0) {}

    void put(const std::vector<int> &output_ids) override
    {
        if (is_prompt_)
        {
            // skip prompt
            is_prompt_ = false;
            return;
        }

        static const std::vector<char> puncts{',', '!', ':', ';', '?'};

        token_cache_.insert(token_cache_.end(), output_ids.begin(), output_ids.end());
        std::string text = tokenizer_->decode(token_cache_);
        if (is_first_line_)
        {
            ltrim(text);
        }
        if (text.empty())
        {
            return;
        }

        std::string printable_text;
        if (text.back() == '\n')
        {
            // flush the cache after newline
            printable_text = text.substr(print_len_);
            is_first_line_ = false;
            token_cache_.clear();
            print_len_ = 0;
        }
        else if (std::find(puncts.begin(), puncts.end(), text.back()) != puncts.end())
        {
            // last symbol is a punctuation, hold on
        }
        else if (text.size() >= 3 && text.compare(text.size() - 3, 3, "�") == 0)
        {
            // ends with an incomplete token, hold on
        }
        else
        {
            printable_text = text.substr(print_len_);
            print_len_ = text.size();
        }

        callback_.Call({Napi::String::New(env_, "data"), Napi::String::New(env_, printable_text)});
    };

    void end() override
    {
        std::string text = tokenizer_->decode(token_cache_);
        if (is_first_line_)
        {
            ltrim(text);
        }
        std::string printable_text = text.substr(print_len_);

        is_prompt_ = true;
        is_first_line_ = true;
        token_cache_.clear();
        print_len_ = 0;

        if (!printable_text.empty()) {
            callback_.Call({Napi::String::New(env_, "data"), Napi::String::New(env_, printable_text)});
        }
        callback_.Call({Napi::String::New(env_, "end"), Napi::String::New(env_, "")});
    };

private:
    Napi::Env &env_;
    Napi::Function &callback_;
    chatglm::BaseTokenizer *tokenizer_;
    bool is_prompt_;
    bool is_first_line_;
    std::vector<int> token_cache_;
    int print_len_;
};

Napi::Value chat(Napi::Env &env, Napi::Function &callback, Args args)
{
    if (args.prompt.empty())
    {
        return Napi::String::New(env, "prompt empty");
    }

    if (args.model_path.empty())
    {
        return Napi::String::New(env, "model_path empty");
    }

    chatglm::Pipeline pipeline(args.model_path);

    std::string model_name = pipeline.model->config.model_type_name();

    auto streamer = std::make_shared<EmitterStreamer>(env, callback, pipeline.tokenizer.get());

    chatglm::GenerationConfig gen_config(args.max_length, args.max_new_tokens, args.max_context_length, args.temp > 0,
                                         args.top_k, args.top_p, args.temp, args.repeat_penalty, args.num_threads);

    std::vector<chatglm::ChatMessage> system_messages;
    if (!args.system.empty())
    {
        system_messages.emplace_back(chatglm::ChatMessage::ROLE_SYSTEM, args.system);
    }

    if (args.mode == INFERENCE_MODE_CHAT)
    {
        std::vector<chatglm::ChatMessage> messages = system_messages;
        messages.emplace_back(chatglm::ChatMessage::ROLE_USER, args.prompt);
        chatglm::ChatMessage output = pipeline.chat(messages, gen_config, streamer.get());
    }
    else
    {
        std::string output = pipeline.generate(args.prompt, gen_config, streamer.get());
    }

    return Napi::String::New(env, "OK");
}

// /////////////////////////////////////////////////////////

Napi::Value Enter(const Napi::CallbackInfo &info)
{
    Napi::Env env = info.Env();
    Napi::Function callback = info[0].As<Napi::Function>();
    Napi::String model_path = info[1].As<Napi::String>();
    Napi::String prompt = info[2].As<Napi::String>();
    Napi::Number temp = info[3].As<Napi::Number>();
    Napi::Number top_p = info[4].As<Napi::Number>();
    Napi::Number top_k = info[5].As<Napi::Number>();

    Args args;
    args.model_path = model_path;
    args.prompt = prompt;
    args.temp = temp;
    args.top_p = top_p;
    args.top_k = top_k;

    return chat(env, callback, args);
}

// Init
Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "chat"), Napi::Function::New(env, Enter));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
