#ifndef BLOCKER_H
#define BLOCKER_H

#define DISALLOW_COPY_AND_ASSING(type) \
	type(const type&);\
	void operator=(const type&)

#define NO_DEFAULTS(type)\
	type();\
	~type();	

#endif