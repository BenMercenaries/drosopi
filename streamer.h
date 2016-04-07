
#ifndef STREAMER_H
#define STREAMER_H

#include <vector>
#include "op.h"

class Streamer
{
public:

	unsigned long long	T;

	Streamer () : T (0)		{}
	virtual ~Streamer ()	{}

	void	ln (Op &op, const char *name);

	void	stream (float *buffer, int samples);

	virtual bool	init () = 0;
	virtual bool	release () = 0;

	virtual bool	start () = 0;
	virtual bool	stop () = 0;

public:
	static Streamer	*initStreamer (const char *name);
	static Streamer	*getStreamer ();

	class Register
	{
	public:
		Register (const char *name, Streamer *streamer);
	};

	std::vector<Op*>	Ops;
	const Op::Out		*Out;
};

#define REGISTER_STREAMER(name, c) \
static c _##c; Streamer::Register _reg##c (#name, &_##c);

#endif // STREAMER_H
