
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "streamer.h"
#include <portaudio.h>

#include "op.h"



int main (int argc, char **argv)
{
	Streamer	*streamer = Streamer::initStreamer ("PortAudio");
	if (!streamer)
	{
		printf ("Can't init streamer.\n");
		return 0;
	}
/*
	Exp	exp1;
	exp1 ("cu") = 880.0f;
	exp1 ("to") = 0.0f;
	exp1 ("sp") = 0.00001f;

	Osc	osc1;
	osc1 ("fq").ln (exp1, "ou");

	Saw	lfo1;
	lfo1 ("w").ln (osc1, "ou");

	streamer->ln (lfo1, "ou");
*/

	Osc	osc1;
	osc1 ("fq") = 3.0f;

	Saw	lfo1;
	lfo1 ("w").ln (osc1, "ou");
	lfo1 ("ga") = -440.0f;
	lfo1 ("of") = 440.0f;

	Osc	osc2;
	osc2 ("fq").ln (lfo1, "ou");

	Square	wf;
	wf ("w").ln (osc2, "ou");

	streamer->ln (wf, "ou");

/*
	Exp	exp1;
	exp1 ("to") = 0.0f;
	exp1 ("sp") = 0.0001f;
	exp1 ("cu") = 880.0f;

	Osc	osc1;
	osc1 ("fq") = 440.0f;

	Sin	lfo1;
	lfo1 ("to") = 0.0f;
	exp1 ("sp") = 0.0001f;
	exp1 ("cu") = 880.0f;

	Square	wf1;
	wf1 ("cy") = 0.1f;
	wf1 ("w").ln (osc1, "ou");

	streamer->ln (wf1, "ou");
*/

	streamer->start ();
	Pa_Sleep (10*1000);
	streamer->stop ();

	streamer->release ();

	return 0;
}