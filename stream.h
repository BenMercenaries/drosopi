
#ifndef STREAM_H
#define STREAM_H

class Stream
{
public:
	float		*Ptr;
	int			Incr;

	Stream	&operator = (float value)
	{
		*Ptr = value;
		return *this;
	}

	operator float () const
	{
		return *Ptr;
	}

	Stream	&operator ++ ()
	{
		Ptr += Incr;
		return *this;
	}
};

class ConstStream
{
public:
	const float	*Ptr;
	int			Incr;

	operator float () const
	{
		return *Ptr;
	}

	ConstStream	&operator ++ ()
	{
		Ptr += Incr;
		return *this;
	}
};

#endif // STREAM_H
