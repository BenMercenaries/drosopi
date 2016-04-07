
#include "streamer.h"
#include <portaudio.h>

#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_RATE 44100

class PaStreamer : public Streamer
{
public:

	virtual bool	init ();
	virtual bool	release ();

	virtual bool	start ();
	virtual bool	stop ();

	PaStream		*Stream;

	static int	callback (const void *input,
							void *output,
							unsigned long frameCount,
							const PaStreamCallbackTimeInfo *timeinfo,
							PaStreamCallbackFlags flags,
							void *ud);
};

int	PaStreamer::callback (const void *input,
						void *output,
						unsigned long frameCount,
						const PaStreamCallbackTimeInfo *timeinfo,
						PaStreamCallbackFlags flags,
						void *ud)
{
	reinterpret_cast<PaStreamer*>(ud)->stream ((float*)output, frameCount);
	return paContinue;
}

static void	_paVerify (PaError res, const char *file, int line, const char *exp)
{
	if (res != paNoError)
	{
		printf ("PortAudio error: %s\n", Pa_GetErrorText (res));
		printf ("%s:%d: %s\n", file, line, exp);
		exit (-1);
	}
#ifdef VERBOSE
	printf ("succeded: %s\n", exp);
#endif
}
#define PA_VERIFY(exp)	_paVerify (exp, __FILE__, __LINE__, #exp)

bool	PaStreamer::init ()
{
	if (Pa_Initialize () != paNoError)
		return false;

	if (Pa_OpenDefaultStream (&Stream, 0, 2, paFloat32, SAMPLE_RATE, 256, PaStreamer::callback, this) != paNoError)
	{
		Pa_Terminate ();
		return false;
	}

	return true;
}

bool	PaStreamer::release ()
{
	Pa_Terminate ();
	return true;
}

bool	PaStreamer::start ()
{
	PA_VERIFY (Pa_StartStream (Stream));
	return true;
}

bool	PaStreamer::stop ()
{
	PA_VERIFY (Pa_StopStream (Stream));
	return true;
}

REGISTER_STREAMER(PortAudio, PaStreamer)
