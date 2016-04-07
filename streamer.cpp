
#include "streamer.h"
#include "op.h"
#include <map>
#include <set>
#include <functional>
#include <string>
#include <math.h>

#define SAMPLE_RATE 44100
#define PI 3.1415926535

void	Streamer::stream (float *buffer, int samples)
{
	for (Op *op : Ops)
		op->generate (samples);

	ConstStream	output = Out->getStream ();
	for (int i = 0; i < samples; ++i, ++output)
	{
		buffer[2*i+0] = (float)output;
		buffer[2*i+1] = (float)output;
	}
}

void	Streamer::ln (Op &op, const char *name)
{
	std::set<Op*>	visited;

	std::function<void(Op*)> visit = [&] (Op *o)
	{
		if (visited.find (o) != visited.end ())
			return;
		visited.insert (o);
		for (Op::In &input : o->inputs ())
			if (input.CnOp != NULL)
				visit (input.CnOp);
		for (Op::Out &output : o->outputs ())
			output.Buffer.resize (65536);

		Ops.push_back (o);
	};

	Ops.clear ();
	visit (&op);

	Out = &op.output (name);
}

typedef std::map<std::string, Streamer*>	StreamerMap;
static StreamerMap	*Streamers = NULL;

Streamer::Register::Register (const char *name, Streamer *streamer)
{
	if (Streamers == NULL)
		Streamers = new StreamerMap ();

	(*Streamers)[name] = streamer;
}

static Streamer*	Current = NULL;

Streamer	*Streamer::initStreamer (const char *name)
{
	if (Current)
		Current->release ();

	if (name != NULL)
	{
		StreamerMap::iterator	it = Streamers->find (name);
		if (it != Streamers->end ())
		{
			Current = it->second;
			if (Current->init ())
				return Current;
		}
	}

	for (StreamerMap::iterator it = Streamers->begin (); it != Streamers->end (); ++it)
	{
		Current = it->second;
		if (Current->init ())
			return Current;
	}
	Current = NULL;
	return NULL;
}

Streamer	*Streamer::getStreamer ()
{
	return Current;
}
