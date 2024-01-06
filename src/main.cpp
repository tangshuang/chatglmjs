#include "chat.h"

#include <napi.h>
#include <chrono>
#include <iostream>
#include <thread>

class Emitter: public Chat::Emitter {
  public:
    Emitter(Napi::Env &env, Napi::Function &callback)
      : env_(env), callback_(callback) {};
    void emit(std::string type, std::string msg) {
      callback_.Call({Napi::String::New(env_, type), Napi::String::New(env_, msg)});
    }

  private:
    Napi::Env env_;
    Napi::Function callback_;
};

Napi::Value Enter(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Function callback = info[0].As<Napi::Function>();
  Napi::String model_path = info[1].As<Napi::String>();
  Napi::String prompt = info[2].As<Napi::String>();

  Chat::Args args;
  args.model_path = model_path;
  args.prompt = prompt;

  Emitter emitter(env, callback);

  Chat::chat(emitter, args);

  return Napi::String::New(env, "OK");
}

// Init
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "chat"), Napi::Function::New(env, Enter));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
