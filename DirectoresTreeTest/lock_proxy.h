#pragma once
#include<mutex>
template<class M,class C>
class lock_proxy final
{
public: 
	lock_proxy(M& mutex, C& container) : _guard(mutex)
	{
		this->container = container;
		//_guard(mutex);
	}
	C& container;
private:
	std::lock_guard<M> _guard;
};

