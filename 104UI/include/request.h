/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 4, 2014

#ifndef REQUEST_H_
#define REQUEST_H_

template<class B>
class RequestBase
{
public:
	RequestBase() :_internal(0) { }

	RequestBase(const RequestBase<B>& other)
	{
		this->_internal = other._internal;
	}

	template<class D>
	void init()
	{
		_internal = new __Internal<D>();
	}

	bool match(B *b)
	{
		return _internal && _internal->match(b);
	}

	template<class InputIterator>
	bool match(InputIterator first, InputIterator last)
	{
		for (InputIterator it = first; it != last; ++it) {
			if (match(*it)) {
				return true;
			}
		}

		return false;
	}

	template<class D>
	D* get_responce()
	{
		if (!_internal) {
			return 0;
		}

		B* responce = _internal->get();
		if (responce) {
			D* casted_responce = dynamic_cast<D* >(responce);
			return casted_responce;
		}

		return 0;
	}

private:

	class __InternalBase
	{
	public:
		virtual ~__InternalBase() {};

		virtual bool match(B *b) = 0;
		virtual B* get() = 0;
	};


	template<class D>
	class __Internal : public __InternalBase
	{
	public:
		__Internal() : _requested(0) { }
		virtual ~__Internal() {};

		virtual bool match(B *b)
		{
			D *d = dynamic_cast<D* >(b);
			if (d != 0) {
				_requested = d;
			}
			return d != 0;
		}

		virtual B* get()
		{
			return dynamic_cast<B* >(_requested);
		}

	private:
		D *_requested;
	};


	__InternalBase *_internal;
};


template<class B, class D>
class Request : public RequestBase<B>
{
public:
	Request()
	{
		this->template init<D>();
	}

	D* get_responce()
	{
		return RequestBase<B>::template get_responce<D>();
	}
};


#endif /* REQUEST_H_ */
