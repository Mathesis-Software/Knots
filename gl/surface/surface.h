#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <vector>

namespace KE { namespace GL {

class Surface {

private:
  struct SurfacePoint {
    float vertex[3];
    float normal[3];

		SurfacePoint(float v0, float v1, float v2, float n0, float n1, float n2) {
			vertex[0] = v0;
			vertex[1] = v1;
			vertex[2] = v2;
			normal[0] = n0;
			normal[1] = n1;
			normal[2] = n2;
		}
  };

	std::vector<SurfacePoint> points;
  float frontRGB[3], backRGB[3];

  bool visible;

protected:

  enum {Front, Both};
  bool stripped;
  int sides;

  virtual void calculate() = 0;
  void addpoint(float v0, float v1, float v2, float n0, float n1, float n2);
  
public:
  Surface();
  virtual ~Surface();

  bool isVisible()
    {return visible;};

  void show()
    {visible = 1;};
  void hide()
    {visible = 0;};
  void paint();
  void destroy();

  float *getFrontRGB()
    {return frontRGB;};
  float *getBackRGB()
    {return backRGB;};
  void setFrontRGB (const float*);
  void setBackRGB (const float*);

private:
	Surface(const Surface&) = delete;
	Surface& operator = (const Surface&) = delete;
};

}}

#endif /* __SURFACE_H__ */
