#ifndef FOREACH_H
#define FOREACH_H



#define FOREACH(ITERATOR, TARGET)	\
for (					\
	(ITERATOR) = (TARGET).begin();	\
	(ITERATOR) != (TARGET).end();	\
	++(ITERATOR))

#define FOREACH_T(TYPE, ITERATOR, TARGET)		\
for (							\
	TYPE::iterator ITERATOR = (TARGET).begin();	\
	ITERATOR != (TARGET).end();			\
	++ITERATOR)

#define FOREACH_T_CONST(TYPE, ITERATOR, TARGET)			\
for (								\
	TYPE::const_iterator ITERATOR = (TARGET).begin();	\
	ITERATOR != (TARGET).end();				\
	++ITERATOR)



#define FOREACH_REVERSE(ITERATOR, TARGET)	\
for (						\
	(ITERATOR) = (TARGET).rbegin();		\
	(ITERATOR) != (TARGET).rend();		\
	++(ITERATOR))

#define FOREACH_REVERSE_T(TYPE, ITERATOR, TARGET)		\
for (								\
	TYPE::reverse_iterator ITERATOR = (TARGET).rbegin();	\
	ITERATOR != (TARGET).rend();				\
	++ITERATOR)

#define FOREACH_REVERSE_T_CONST(TYPE, ITERATOR, TARGET)			\
for (									\
	TYPE::const_reverse_iterator ITERATOR = (TARGET).rbegin();	\
	ITERATOR != (TARGET).rend();					\
	++ITERATOR)



#endif /* FOREACH_H */



