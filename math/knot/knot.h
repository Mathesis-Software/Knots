#ifndef __KNOT_H__
#define __KNOT_H__

#include <iostream>
#include <list>
#include "../headers/parameter.h"
#include "../seifert/seifert.h"

class diagram;
class knot;
class knot_surface;

/***********************************************************************/

#define	addParameterClass(PARAM)	\
class PARAM : public parameter {	\
private:				\
  knot *Parent;				\
  void compute (void);			\
public:					\
  PARAM (knot *p, const char *n)	\
    : parameter (n)			\
  {Parent = p;};			\
}

#define	addParameterClassWithOrder(PARAM)	\
class PARAM : public parameter {	\
private:				\
  knot *Parent;				\
  int order;				\
  void compute (void);			\
public:					\
  PARAM (knot *p, int o, const char *n)	\
    : parameter (n)			\
  {Parent = p; order = o;};		\
}

/***********************************************************************/

class knot : public seifert_base {

protected:

  char *Caption;
  int length;
  double **points;

	std::list<parameter*> *parameterList;
  parameter *Length;

private:

  double *len_table;

  void create_depend (void);
  void clear_depend (void);

  void create_len_table (void);
  void delete_len_table (void);

  bool noMorePoints (const double*);
  double minDist (const double*);

protected:

  knot (void);
  knot (diagram*, int, int);
  ~knot (void);

  int next (int);
  int prev (int);

  bool isEmpty (void);
  void decreaseEnergy (void);
  void setLength (double);
  void center (void);
  void normalize (int);
  void getGradient (const double*, double*);

  friend std::istream & operator >> (std::istream &, knot *);
  friend std::ostream & operator << (std::ostream &, knot *);

  addParameterClass(prmLength);
  addParameterClass(prmEnergy);
  addParameterClass(prmAcn);
  addParameterClass(prmSAcn);
  addParameterClass(prmAen);
  addParameterClassWithOrder(prmKI);
  addParameterClass(prmExperimental);
  addParameterClass(prmSingular);
  addParameterClassWithOrder(prmExperimental2);

  friend class knot_surface;

private:

  knot (const knot&);
  knot& operator = (const knot&);
};

/***********************************************************************/

inline int knot::next (int i)
{
  return (i == length - 1) ? 0 : i + 1;
}

inline int knot::prev (int i)
{
  return i ? i - 1 : length - 1;
}

inline bool knot::isEmpty (void)
{
  return (length == 0);
}

#endif /* __KNOT_H__ */
