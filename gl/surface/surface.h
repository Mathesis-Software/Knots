#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <list>

class surface {

private:

  struct surfpoint
  {
    float vertex [3];
    float normal [3];
  };

	std::list<surfpoint*> *points;
  float frontRGB [3], backRGB [3];

  bool visible;

protected:

  enum {Front, Both};
  bool stripped;
  int sides;

  virtual void calculate (void) = 0;
  void addpoint (const float, const float, const float,
		 const float, const float, const float);
  
public:

  surface (void);
  virtual ~surface (void);

  bool isVisible (void)
    {return visible;};

  void show (void)
    {visible = 1;};
  void hide (void)
    {visible = 0;};
  void paint (void);
  void destroy (void);

  float *getFrontRGB (void)
    {return frontRGB;};
  float *getBackRGB (void)
    {return backRGB;};
  void setFrontRGB (const float*);
  void setBackRGB (const float*);
};

#endif /* __SURFACE_H__ */
