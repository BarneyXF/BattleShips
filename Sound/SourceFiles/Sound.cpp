#include "../../Headers/Sound/SoundHeaders.h"
#include <process.h>

void InitializingSound(SoundToPlay sound)
{
	
	std::thread *soundThread = new std::thread(Play, sound);
	soundThread->detach();
}

void Play(SoundToPlay sound)
{
	switch (sound)
	{
		case explosionSound:
		{
			PlaySound((LPCSTR)"..\\Resources\\shotSound.wav", NULL, SND_FILENAME);
			PlaySound((LPCSTR)"..\\Resources\\explosionSound.wav", NULL, SND_FILENAME);
			break;
		}
		case missSound:
		{
			PlaySound((LPCSTR)"..\\Resources\\shotSound.wav", NULL, SND_FILENAME);
			PlaySound((LPCSTR)"..\\Resources\\missSound.wav", NULL, SND_FILENAME);
			break;
		}
		case lostConnectionSound:
		{
			PlaySound((LPCSTR)"..\\Resources\\looseConnectionSound.wav", NULL, SND_FILENAME);
			break;
		}
		case looseSound:
		{
			PlaySound((LPCSTR)"..\\Resources\\looseSound.wav", NULL, SND_FILENAME);
			break;
		}
		case menuSound:
		{
			PlaySound((LPCSTR)"..\\Resources\\menuChooseSound.wav", NULL, SND_FILENAME);
			break;
		}
		case winSound:
		{
			PlaySound((LPCSTR)"..\\Resources\\winSound.wav", NULL, SND_FILENAME);
			break;
		}
	}
}