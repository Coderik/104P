/*
 * i_view.h
 *
 *  Created on: Feb 6, 2014
 *      Author: Vadim Fedorov
 */

#ifndef I_VIEW_H_
#define I_VIEW_H_

#include <string>
#include <gdkmm/pixbuf.h>

using namespace std;

class IView
{
public:
	virtual ~IView() { }

	virtual string get_title(){};
	virtual Gdk::Pixbuf get_pixbuf(unsigned int time = 0){};
};



#endif /* I_VIEW_H_ */
