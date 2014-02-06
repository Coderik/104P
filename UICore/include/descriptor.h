/*
 * descriptor.h
 *
 *  Created on: Feb 6, 2014
 *      Author: Vadim Fedorov
 */

#ifndef DESCRIPTOR_H_
#define DESCRIPTOR_H_

class Descriptor
{
public:
	static Descriptor create();

	Descriptor(const Descriptor &other);
	~Descriptor();

	Descriptor& operator= (const Descriptor &rhs);
	bool operator== (const Descriptor &d) const;
	bool operator!= (const Descriptor &d) const;
	friend inline bool operator< (const Descriptor& lhs, const Descriptor& rhs);
	friend inline bool operator> (const Descriptor& lhs, const Descriptor& rhs);
	friend inline bool operator<=(const Descriptor& lhs, const Descriptor& rhs);
	friend inline bool operator>=(const Descriptor& lhs, const Descriptor& rhs);

private:
	struct __Descriptor
	{
		unsigned int ref_counter;
	};

	__Descriptor *_descriptor;

	Descriptor();
};

// NOTE: definitions are in header in order to overload two argument versions.
inline bool operator< (const Descriptor& lhs, const Descriptor& rhs)
{
	return lhs._descriptor < rhs._descriptor;
}
inline bool operator> (const Descriptor& lhs, const Descriptor& rhs) { return operator< (rhs,lhs); }
inline bool operator<= (const Descriptor& lhs, const Descriptor& rhs) { return !operator> (lhs,rhs); }
inline bool operator>= (const Descriptor& lhs, const Descriptor& rhs) { return !operator< (lhs,rhs); }



#endif /* DESCRIPTOR_H_ */
