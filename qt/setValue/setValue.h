#ifndef __SETVALUE_H__
#define __SETVALUE_H__

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

char *setString (const char*, char*, const int);
int setInt (const char*, int, int, int);
double setDouble (const char*, double, double, double);

class setupString : public QDialog {

Q_OBJECT
  
private:

  setupString (char*, const int);
  ~setupString (void);

  QLineEdit *label;
  QPushButton *okButton, *cancelButton;

  char *orig;

private slots:

  void accept ();
  
  friend char *setString (const char*, char*, const int);
};

class setupInt : public QDialog {

Q_OBJECT
  
private:

  setupInt (int*, int, int);
  ~setupInt (void);

  QLineEdit *label;
  QPushButton *okButton, *cancelButton;
  
  int *current;
  int minValue, maxValue;

private slots:

  void test ();

public:

  friend int setInt (const char*, int, int, int);
};

class setupDouble : public QDialog {

Q_OBJECT
  
private:

  setupDouble (double*, double, double);
  ~setupDouble (void);

  QLineEdit *label;
  QPushButton *okButton, *cancelButton;
  
  double *current;
  double minValue, maxValue;

private slots:

  void test ();

public:

  friend double setDouble (const char*, double, double, double);
};

#endif /* __SETVALUE_H__ */
