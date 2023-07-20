//
// This prefix is used to override new CFSTR calls in Mac OS 10.4 which
// causes FM crash when unloading plugin.
//
 
#if defined( __GNUC__ )

  #undef __CONSTANT_CFSTRINGS__
  
  #include <CoreServices/CoreServices.h>

#endif