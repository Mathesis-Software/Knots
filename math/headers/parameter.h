#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include <string.h>

class parameter {

private:

  char *name;
  bool ready;

protected:

  float internalValue;
  virtual void compute (void) = 0;

public:

  parameter (const char *n)
  {
    ready = 0;
    name = new char [strlen (n) + 1];
    strcpy (name, n);
  };
  virtual ~parameter (void)
    {delete[] name;};

  float value (void)
  {
    if (!ready)
    {
      compute ();
      ready = 1;
    }
  
    return internalValue;
  };

  bool isReady (void)
    {return ready;};
  const char *getName (void)
    {return name;};
  void destroy (void)
    {ready = 0;};
};

#endif /* __PARAMETER_H__ */
