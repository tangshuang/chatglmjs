#include "chat.cpp"

#include <napi.h>
#include <chrono>
#include <iostream>
#include <thread>

Napi::Value Enter(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  Napi::Function emit = info[0].As<Napi::Function>();
  Napi::String model_path = info[1].As<Napi::String>();
  Napi::String prompt = info[2].As<Napi::String>();

  Chat::Args args;
  args.model_path = model_path;
  args.prompt = prompt;

  Chat::chat(env, emit, args);

  return Napi::String::New(env, "OK");
}

// Init
Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "chat"), Napi::Function::New(env, Enter));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
