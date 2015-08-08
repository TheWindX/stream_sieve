#include <iostream>
#include <functional>

template<typename T>
class IEnumerable
{
	T _cur;
	std::function<IEnumerable<T>() > _next;
	IEnumerable<T>();
public:
	T cur();
	IEnumerable<T> next()const;
	IEnumerable<T> where(std::function<bool(T)> f)const;
	IEnumerable(T v, std::function<IEnumerable<T>() > func)
		: _cur(v), _next(func){}
	bool valid()const;
	void foreach(std::function<void(T)>, size_t n=-1);
	IEnumerable<T> take(size_t n);
	IEnumerable<T> takeWhile(std::function<bool(T)> pred);
};

template<typename T>
T IEnumerable<T>::cur()
{
	return _cur;
}

template<typename T>
IEnumerable<T>::IEnumerable(){};

template<typename T>
IEnumerable<T> IEnumerable<T>::next()const
{
	return _next();
}

template<typename T>
bool IEnumerable<T>::valid()const
{
	if(_next)return true;
	return false;
}

template<typename T>
IEnumerable<T> IEnumerable<T>::where(std::function <bool(T)> f)const
{
	if (f(_cur))
	{
		IEnumerable<T> copy = *this;
		std::function<IEnumerable<T>() > func 
			= [=]()
		{
			return copy._next().where(f);
		};

		return IEnumerable<T>(_cur, func);
	}
	else
	{
		return _next().where(f);
	}
}

template<typename T>
void IEnumerable<T>::foreach(std::function<void(T)> f, size_t s = -1)
{
	if (s == 0) return;
	if(!f) return;

	IEnumerable<T> tmp = *this;
	while (tmp.valid())
	{
		f(tmp.cur());
		tmp = tmp.next();
	}
}

template<typename T>
IEnumerable<T> IEnumerable<T>::take(size_t n)
{
	if(n == 0) return IEnumerable<T>( );
	auto copy = *this;
	return IEnumerable(_cur, [=]()
		{
			return copy.next().take(n-1);
		});
}

template<typename T>
IEnumerable<T> IEnumerable<T>::takeWhile(std::function<bool(T)> pred)
{
	if(!valid()) return IEnumerable<T>( ); 
	if(!pred(_cur)) return IEnumerable<T>( );

	auto copy = *this;
	return IEnumerable(_cur, [=]()
	{
		return copy.next().takeWhile(pred);
	});
}

IEnumerable<int> rangefrom(int n)
{
	return IEnumerable<int>(n,  [=](){return rangefrom(n + 1); });
};

IEnumerable<int> sieve(IEnumerable<int> s)
{
	auto c = s.cur();
	return IEnumerable<int>(s.cur()
		, [=](){return s.next()
						.where([=](int n){return n % c != 0; }); });
};

int main(int argc, char* argv[])
{	
	sieve(rangefrom(2))
		.takeWhile([](int n){return n<100;})
		.foreach([=](int n){std::cout<<n<<std::endl;});
	return 0;
}