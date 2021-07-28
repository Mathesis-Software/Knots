#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include <string>

class Knot;

class parameter {

public:
	const Knot &knot;
	const std::string name;

private:
	bool ready;
	double internalValue;

protected:
	virtual double compute() = 0;

public:
	parameter(const Knot &knot, const std::string &name) : knot(knot), name(name), ready(false) {}
	virtual ~parameter() {}

	double value() {
		if (!this->ready) {
			this->internalValue = this->compute();
			this->ready = true;
		}

		return this->internalValue;
	}

	bool isReady() {return this->ready;}
	void destroy() {this->ready = 0;}
};

#endif /* __PARAMETER_H__ */
