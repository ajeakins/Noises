
// Settings write helpers

#define SET_VALUE( VALUE, GETTER, FLAG ) \
if ( VALUE != GETTER ) \
{ \
	FLAG = true; \
	VALUE = GETTER; \
}
