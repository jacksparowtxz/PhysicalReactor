#pragma once


template<typename T>
class Delegate {};


template<typename R,typename ...Args>
class Delegate<R(Args...)> {
	typedef void* InstancePtr;
	typedef R(*InternalFunction)(InstancePtr, Args...);
	typedef std::pair<InstancePtr, InternalFunction> Stub;

	template<R(*Function)(Args...)>
	static inline R FunctionStub(InstancePtr, Args... args) {
		return Function(std::forward<Args>(args)...);
	}

	template<typename C, R(C::*Function)(Args...)>
	static inline R ClassMethodStub(InstancePtr instance, Args... args) {
		return (static_cast<C*>(instance)->*Function)(std::forward<Args>(args)...);
	}

public:
	typedef R result_type;

public:
	Delegate() :m_Stub(nullptr, nullptr) {}

	Delegate(Delegate&& del) :m_Stub(std::forward<Stub>(del)) {}

	Delegate(const Delegate& del) :m_Stub(del) {}


	inline Delegate& operator=(Delegate&& rhs) {
	           if (this!=&rhs)
	           {
				   m_Stub = std::move(rhs.m_Stub);
	           }
			   return *this;
	}

	inline Delegate& operator=(const Delegate& rhs) {
	    if (this!=&rhs)
	    {
			m_Stub = rhs.m_Stub;
	    }
		return *this;
	}

public:
	template<R(*Function)(Args...)>
	inline void bind()
	{
		m_Stub.first = nullptr;
		m_Stub.second = &FunctionStub<Function>;
	}

	template<typename C,R(C::*Function)(Args...)>
	inline void bind()
	{
		m_Stub.first = nullptr;
		m_Stub.second = &ClassMethodStub<C, Function>;
	}

	template<typename C,R(C::*Function)(Args...)>
	inline void bind(C* instance)
	{
		m_Stub.first = instance;
		m_Stub.second = &ClassMethodStub<C, Function>;
	}

	template<typename C,R(C::*Function)(Args...)>
	inline void bind(C& instance)
	{
		m_Stub.first = instance;
		m_Stub.second = &ClassMethodStub<C, Function>;
	}

	inline result_type invoke(Args... args) const {
	     if (m_Stub.second==nullptr)
	     {
			 return result_type();
	     }
		 return m_Stub.second(m_Stub.first, std::forward<Args>(args)...);
	}

	template<typename C>
	inline result_type invoke(C* instance,Args... args) const
	{
	       if (m_Stub.second==nullptr)
	       {
			   return result_type();
	       }
		   return m_Stub.second(instance, std::forward<Args>(args...));
	}

	template<typename C>
	inline result_type invoke(C& instance, Args... args) const
	{
	      if (m_Stub.second=nullptr)
	      {
			  return result_type();
	      }
		  return m_Stub.second(&instance, std:; forward<Args>(args)...);
	}

	inline result_type operator()(Args... args) const
	{
		return invoke(std::forward<Args>(args)...);
	}

	template<typename C>
	inline result_type operator() (C* instance, Args... args) const
	{
		return invoke(instance, std::forward<Args>(args)...);
	}

	inline operator bool() const {
		return m_Stub.second! = nullptr;
	}

	private:
		Stub m_Stub;
};