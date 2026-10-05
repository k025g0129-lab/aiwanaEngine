#include "Input.h"

#include <cassert>
#include <cstring>

Input* Input::GetInstance() {
    static Input instance;
    return &instance;
}

void Input::Initialize(HINSTANCE hInstance, HWND hwnd) {

    HRESULT hr;

    // DirectInput生成
    hr = DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,reinterpret_cast<void**>(directInput_.GetAddressOf()),nullptr);
    assert(SUCCEEDED(hr));

    // キーボード生成
    hr = directInput_->CreateDevice(GUID_SysKeyboard,keyboard_.GetAddressOf(),nullptr);
    assert(SUCCEEDED(hr));

    // データ形式
    hr = keyboard_->SetDataFormat(&c_dfDIKeyboard);
    assert(SUCCEEDED(hr));

    // 協調レベル
    hr = keyboard_->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
    assert(SUCCEEDED(hr));

    // 初回取得
    keyboard_->Acquire();
}

void Input::Update() {

    memcpy(preKey_, key_, 256);

    keyboard_->Acquire();

    keyboard_->GetDeviceState(sizeof(key_), key_);
}

bool Input::PushKey(BYTE keyNumber) {

    return key_[keyNumber];
}

bool Input::TriggerKey(BYTE keyNumber) {

    return key_[keyNumber] &&!preKey_[keyNumber];
}

bool Input::ReleaseKey(BYTE keyNumber) {

    return !key_[keyNumber] &&preKey_[keyNumber];
}