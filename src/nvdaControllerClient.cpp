#include <node.h>
#include <v8.h>
#include <iostream>
#include "nvdaController.h"

using namespace v8;

void SetSpeakText(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.Length() < 1) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments").ToLocalChecked()));
        return;
    }

    if (!args[0]->IsString()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Expected a string as the first argument").ToLocalChecked()));
        return;
    }

    String::Utf8Value str(isolate, args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked());
    std::string cppStr = std::string(*str);

    std::wstring wideStr(cppStr.begin(), cppStr.end());

    nvdaController_cancelSpeech();

    nvdaController_speakText(wideStr.c_str());

    args.GetReturnValue().Set(Undefined(isolate));
}

void SetStop(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    nvdaController_cancelSpeech();

    args.GetReturnValue().Set(Undefined(isolate));
}

void Initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "setSpeakText", SetSpeakText);
    NODE_SET_METHOD(exports, "setStop", SetStop);
}

NODE_MODULE(myaddon, Initialize)
