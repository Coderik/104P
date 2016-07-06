/**
 * Copyright (C) 2016, Vadim Fedorov <coderiks@gmail.com>
 *
 * This program is free software: you can use, modify and/or
 * redistribute it under the terms of the simplified BSD
 * License. You should have received a copy of this license along
 * this program. If not, see
 * <http://www.opensource.org/licenses/bsd-license.html>.
 */

/// Created on: Feb 6, 2014

#ifndef DESCRIPTOR_H_
#define DESCRIPTOR_H_

class Descriptor
{
public:
	static Descriptor create();
	static Descriptor empty();

	Descriptor();
	Descriptor(const Descriptor &other);
	~Descriptor();

	bool is_empty();

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

	Descriptor(__Descriptor *descriptor);
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
