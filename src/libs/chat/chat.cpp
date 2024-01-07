#include <chatglm.h>
#include <chat.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#endif

namespace Chat {

    // trim from start (in place)
    inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch){ return !std::isspace(ch); }));
    }

    class EmitterStreamer : public chatglm::BaseStreamer
    {
    public:
        EmitterStreamer(Emitter emitter, chatglm::BaseTokenizer *tokenizer)
            : emitter_(emitter), tokenizer_(tokenizer), is_prompt_(true), is_first_line_(true), print_len_(0) {}

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

            emitter_.emit("data", printable_text);
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

            emitter_.emit("data", printable_text);
            emitter_.emit("end", "");
        };

    private:
        Emitter emitter_;
        chatglm::BaseTokenizer *tokenizer_;
        bool is_prompt_;
        bool is_first_line_;
        std::vector<int> token_cache_;
        int print_len_;
    };

    static void chat(Emitter &emitter, Args &args) {
        if (args.prompt.empty()) {
            return;
        }

        chatglm::Pipeline pipeline(args.model_path);

        std::string model_name = pipeline.model->config.model_type_name();

        auto streamer = std::make_shared<EmitterStreamer>(emitter, pipeline.tokenizer.get());

        chatglm::GenerationConfig gen_config(args.max_length, args.max_new_tokens, args.max_context_length, args.temp > 0,
                                            args.top_k, args.top_p, args.temp, args.repeat_penalty, args.num_threads);

        std::vector<chatglm::ChatMessage> system_messages;
        if (!args.system.empty()) {
            system_messages.emplace_back(chatglm::ChatMessage::ROLE_SYSTEM, args.system);
        }

        if (args.mode == INFERENCE_MODE_CHAT) {
            std::vector<chatglm::ChatMessage> messages = system_messages;
            messages.emplace_back(chatglm::ChatMessage::ROLE_USER, args.prompt);
            chatglm::ChatMessage output = pipeline.chat(messages, gen_config, streamer.get());
        } else {
            std::string output = pipeline.generate(args.prompt, gen_config, streamer.get());
        }
    }
}
