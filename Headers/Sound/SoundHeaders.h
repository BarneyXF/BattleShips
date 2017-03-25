#pragma once
#include <iostream>
#include <conio.h>
#include <thread>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

enum SoundToPlay
{
	missSound, explosionSound, winSound, looseSound, lostConnectionSound, shotSound, menuSound
};

void Play(SoundToPlay sound);
void InitializingSound(SoundToPlay sound);