#ifndef __CHAT__
#define __CHAT__

#include <iostream>
#include <chatglm.h>

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

    class Emitter {
        public:
            void emit(std::string type, std::string msg);
    };

    static void chat(Emitter &emitter, Args &args);
}

#endif