
#ifndef OP_H
#define OP_H

#include "stream.h"
#include <vector>
#include <array>
#include <cstring>
#include <math.h>

#define SAMPLE_RATE 44100
#define PI 3.1415926535

class Op
{
public:

	virtual ~Op ()	{}

	virtual void	generate (int samples) = 0;

	class Out
	{
	public:
		const char			*Name;
		std::vector<float>	Buffer;

		Stream		getStream ()
		{
			return { &Buffer[0], 1 };
		}
		ConstStream	getStream () const
		{
			return { &Buffer[0], 1 };
		}
	};

	class In
	{
	public:
		const char			*Name;
		float				Value;
		Op					*CnOp;
		const Out			*CnOut;

		ConstStream	getStream ()
		{
			return CnOut ? CnOut->getStream () : ConstStream { &Value, 0 };
		}

		In	&operator = (float x)
		{
			Value = x;
			CnOp = NULL;
			CnOut = NULL;
			return *this;
		}

		void	ln (Op &op, const char *name)
		{
			CnOp = &op;
			CnOut = &op.output (name);
		}
	};

	template<class T>
	class range
	{
	public:
		T	*B, *E;
		T	*begin ()	{ return B; }
		T	*end ()		{ return E; }
	};

	virtual range<In>	inputs () = 0;
	virtual range<Out>	outputs () = 0;

	In	&operator () (const char *name)
	{
		for (In &item : inputs ())
			if (strcmp (item.Name, name) == 0)
				return item;
		static In	_def;
		return _def;
	}
	virtual Out	&output (const char *name)
	{
		for (Out &item : outputs ())
			if (strcmp (item.Name, name) == 0)
				return item;
		static Out	_def;
		return _def;
	}
};

class Osc : public Op
{
public:

	std::array<In, 1>	Ins;
	std::array<Out, 1>	Outs;

	float	W;

	Osc ()
	{
		W = 0.0f;
		Ins =
		{
			In { "fq", 0.0f },
		};

		Outs =
		{
			Out { "ou" }
		};
	}

	virtual range<In>	inputs ()	{ return range<In> {Ins.begin (),Ins.end ()}; }
	virtual range<Out>	outputs ()	{ return range<Out> {Outs.begin (),Outs.end ()}; }

	void	generate (int samples)
	{
		ConstStream	fq = Ins[0].getStream ();
		Stream		ou = Outs[0].getStream ();

		const float	dt = 1.0f / SAMPLE_RATE;

		for (int i = 0; i < samples; ++i)
		{
			ou = W;
			W += (float)fq * dt;
			if (W > 1.0f)
				W -= 1.0f;

			++fq;
			++ou;
		}
	}
};

class Sin : public Op
{
public:

	std::array<In, 3>	Ins;
	std::array<Out, 1>	Outs;

	Sin ()
	{
		Ins =
		{
			In { "w", 0.0f },
			In { "ga", 1.0f },
			In { "of", 0.0f },
		};

		Outs =
		{
			Out { "ou" }
		};
	}

	virtual range<In>	inputs ()	{ return range<In> {Ins.begin (),Ins.end ()}; }
	virtual range<Out>	outputs ()	{ return range<Out> {Outs.begin (),Outs.end ()}; }

	void	generate (int samples)
	{
		ConstStream	w = Ins[0].getStream ();
		ConstStream	ga = Ins[1].getStream ();
		ConstStream	of = Ins[2].getStream ();
		Stream		ou = Outs[0].getStream ();

		for (int i = 0; i < samples; ++i)
		{
			ou = sin (2 * PI * (float)w) * (float)ga + (float)of;
			++w;
			++ga;
			++of;
			++ou;
		}
	}
};

class Saw : public Op
{
public:

	std::array<In, 3>	Ins;
	std::array<Out, 1>	Outs;

	Saw ()
	{
		Ins =
		{
			In { "w", 0.0f },
			In { "ga", 1.0f },
			In { "of", 0.0f },
		};

		Outs =
		{
			Out { "ou" }
		};
	}

	virtual range<In>	inputs ()	{ return range<In> {Ins.begin (),Ins.end ()}; }
	virtual range<Out>	outputs ()	{ return range<Out> {Outs.begin (),Outs.end ()}; }

	void	generate (int samples)
	{
		ConstStream	w = Ins[0].getStream ();
		ConstStream	ga = Ins[1].getStream ();
		ConstStream	of = Ins[2].getStream ();
		Stream		ou = Outs[0].getStream ();

		for (int i = 0; i < samples; ++i)
		{
			ou = (2.0f*(float)w - 1.0f) * (float)ga + (float)of;
			++w;
			++ga;
			++ou;
		}
	}
};

class Square : public Op
{
public:

	std::array<In, 4>	Ins;
	std::array<Out, 1>	Outs;

	Square ()
	{
		Ins =
		{
			In { "w", 0.0f },
			In { "cy", 0.5f },
			In { "ga", 1.0f },
			In { "of", 0.0f },
		};

		Outs =
		{
			Out { "ou" }
		};
	}

	virtual range<In>	inputs ()	{ return range<In> {Ins.begin (),Ins.end ()}; }
	virtual range<Out>	outputs ()	{ return range<Out> {Outs.begin (),Outs.end ()}; }

	void	generate (int samples)
	{
		ConstStream	w = Ins[0].getStream ();
		ConstStream	cy = Ins[1].getStream ();
		ConstStream	ga = Ins[2].getStream ();
		ConstStream	of = Ins[2].getStream ();
		Stream		ou = Outs[0].getStream ();

		for (int i = 0; i < samples; ++i)
		{
			ou = ((float)w < (float)cy ? -1.0f : 1.0f) * (float)ga + (float)of;
			++w;
			++cy;
			++ga;
			++ou;
		}
	}
};

class Exp : public Op
{
public:

	std::array<In, 3>	Ins;
	std::array<Out, 1>	Outs;

	Exp ()
	{
		Ins =
		{
			In { "to", 0.0f },
			In { "sp", 1.0f },
			In { "cu", 0.0f },
		};

		Outs =
		{
			Out { "ou" }
		};
	}

	virtual range<In>	inputs ()	{ return range<In> {Ins.begin (),Ins.end ()}; }
	virtual range<Out>	outputs ()	{ return range<Out> {Outs.begin (),Outs.end ()}; }

	void	generate (int samples)
	{
		ConstStream	to = Ins[0].getStream ();
		ConstStream	sp = Ins[1].getStream ();
		Stream		ou = Outs[0].getStream ();

		for (int i = 0; i < samples; ++i)
		{
			ou = Ins[2].Value;
			Ins[2].Value += ((float)to - Ins[2].Value) * (float)sp;

			++to;
			++sp;
			++ou;
		}
	}
};

#endif // OP_H
