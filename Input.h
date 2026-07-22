#pragma once
#include <dinput.h>
#include <wrl.h>

class Input {
public:

    static Input* GetInstance();

    void Initialize(HINSTANCE hInstance, HWND hwnd);
    void Update();

    bool PushKey(BYTE keyNumber);
    bool TriggerKey(BYTE keyNumber);
    bool ReleaseKey(BYTE keyNumber);

private:

    Input() = default;
    ~Input() = default;

    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

private:

    Microsoft::WRL::ComPtr<IDirectInput8> directInput_;
    Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_;

    BYTE key_[256]{};
    BYTE preKey_[256]{};
};