
// Set a value using the given getter and flag if anything changed
#define NOISES_SET_VALUE( VALUE, GETTER, FLAG ) \
if ( VALUE != GETTER ) \
{ \
	FLAG = true; \
	VALUE = GETTER; \
}

// Make an enum iterable by defining a ++ operator for it
#define NOISES_DECLARE_ITERABLE_ENUM( TYPE ) \
inline TYPE& operator++( TYPE& value ) \
{ \
	value = ( TYPE )( ( int )value + 1 ); \
	return value; \
}
