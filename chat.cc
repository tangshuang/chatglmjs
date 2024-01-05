#include "chatglm.h"

#include <napi.h>
#include <fstream>
#include <iomanip>
#include <iostream>

#ifdef _WIN32
#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include <windows.h>
#endif

namespace Chat {

enum InferenceMode {
    INFERENCE_MODE_CHAT,
    INFERENCE_MODE_GENERATE,
};

struct Args {
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

static void chat(Napi::Function &emit, Args &args) {
    if (args.prompt.empty()) {
        return;
    }

    ggml_time_init();
    int64_t start_load_us = ggml_time_us();
    chatglm::Pipeline pipeline(args.model_path);
    int64_t end_load_us = ggml_time_us();

    std::string model_name = pipeline.model->config.model_type_name();

    auto text_streamer = std::make_shared<chatglm::EmitterStreamer>(emit, pipeline.tokenizer.get());

    chatglm::GenerationConfig gen_config(args.max_length, args.max_new_tokens, args.max_context_length, args.temp > 0,
                                         args.top_k, args.top_p, args.temp, args.repeat_penalty, args.num_threads);

    std::vector<chatglm::ChatMessage> system_messages;
    if (!args.system.empty()) {
        system_messages.emplace_back(chatglm::ChatMessage::ROLE_SYSTEM, args.system);
    }

    if (args.mode == INFERENCE_MODE_CHAT) {
        std::vector<chatglm::ChatMessage> messages = system_messages;
        messages.emplace_back(chatglm::ChatMessage::ROLE_USER, args.prompt);
        chatglm::ChatMessage output = pipeline.chat(messages, gen_config, text_streamer.get());
    } else {
        std::string output = pipeline.generate(args.prompt, gen_config, text_streamer.get());
    }
}

}
