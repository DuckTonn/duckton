#include "Sound.h"
void setVolume(WORD volume) {
    // Kiểm tra âm lượng từ 0 đến 65535
    if (volume > 65535) {
        volume = 65535;
    }

    // Thiết lập âm lượng
    DWORD dwVolume = ((DWORD)volume << 16) | (DWORD)volume;
    waveOutSetVolume(0, dwVolume);
}
void playsoundeffect() {
    setVolume(65535);
    PlaySound(TEXT("select.wav"), NULL, SND_ASYNC | SND_FILENAME);
}
void mutesoundeffect() {
    PlaySound(NULL, 0, 0);
}
void sound1() {
    mciSendString(L"open waveaudio", nullptr, 0, nullptr);
    int volumeLevel = 300;
    wchar_t setVolumeCommand[256];
    swprintf_s(setVolumeCommand, L"setaudio waveaudio volume to %d", volumeLevel);
    mciSendString(setVolumeCommand, nullptr, 0, nullptr);
    wchar_t command[512];
    swprintf_s(command, L"open \"songname.mp3\" type mpegvideo alias mp3");
    mciSendString(command, NULL, 0, NULL);
    swprintf_s(command, L"play mp3");
    mciSendString(command, NULL, 0, NULL);

}

void mutesound() {
    wchar_t command[512];
    swprintf_s(command, L"open \"8bit.mp3\" type mpegvideo alias mp3");
    mciSendString(command, NULL, 0, NULL);
    swprintf_s(command, L"stop mp3");
    mciSendString(command, NULL, 0, NULL);
}
void sounddead() {
    mciSendString(L"open waveaudio", nullptr, 0, nullptr);
    int volumeLevel = 500;
    wchar_t setVolumeCommand[256];
    swprintf_s(setVolumeCommand, L"setaudio waveaudio volume to %d", volumeLevel);
    mciSendString(setVolumeCommand, nullptr, 0, nullptr);
    wchar_t command[512];
    swprintf_s(command, L"open \"deathsound.mp3\" type mpegvideo alias mp3");
    mciSendString(command, NULL, 0, NULL);
    swprintf_s(command, L"play mp3");
    mciSendString(command, NULL, 0, NULL);
}
void soundeat() {
    PlaySound(TEXT("eateffect.wav"), NULL, SND_ASYNC | SND_FILENAME);
}
void soundvictory() {
    PlaySound(TEXT("victory.wav"), NULL, SND_ASYNC | SND_FILENAME);
}