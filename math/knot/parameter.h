#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include <string>

class parameter {

public:
	const std::string name;

private:
	bool ready;
	double internalValue;

protected:
	virtual double compute() = 0;

public:
	parameter(const std::string &name) : name(name), ready(false) {}
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
