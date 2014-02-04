/*
 * carrier.h
 *
 *  Created on: Feb 4, 2014
 *      Author: Vadim Fedorov
 */

#ifndef CARRIER_H_
#define CARRIER_H_

template<class B>
class Carrier
{
public:
	template<class D>
	void init()
	{
		_internal = new CarrierInternal<D>();
	}

	bool test(B *b)
	{
		return _internal->test(b);
	}

	/*template<class D>
	D* retrieve()
	{

	}*/

private:

	class BaseInternal
	{
	public:
		virtual bool test(B *b) = 0;
	};

	template<class D>
	class CarrierInternal : public BaseInternal
	{
	public:
		virtual bool test(B *b)
		{
			D *d = dynamic_cast<D* >(b);
			/*if (d != 0) {
				_derived = d;
			}*/
			return d != 0;
		}

	private:
		/*D *_derived;*/
	};

	BaseInternal *_internal;
};


template<class B, class D>
class Car : public Carrier<B>
{
public:
	Car()
	{
		base->init();
	}
};


#endif /* CARRIER_H_ */
