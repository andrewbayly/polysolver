#include <napi.h>
namespace functionexample {

  std::string solve(std::string x);
  Napi::String SolveWrapped(const Napi::CallbackInfo& info);
  
  Napi::Object Init(Napi::Env env, Napi::Object exports);
}