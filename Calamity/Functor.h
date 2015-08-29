#pragma once

namespace clm {

class Functor {
public:
	virtual ~Functor() { };
	virtual void operator()() = 0;
};

}
