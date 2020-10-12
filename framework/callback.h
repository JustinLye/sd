#pragma once

#include <functional>

namespace sd {
namespace framework {
	template<class ReturnType = void, class ArgType = void>
	class Callback {
		typedef std::function<ReturnType(ArgType)> CallbackFunctionType;
		CallbackFunctionType m_CallbackFunction;
	public:
		Callback();
		Callback(CallbackFunctionType callback_function);

		ReturnType operator()(ArgType);
		void set_callback(CallbackFunctionType callback_function);
	};

	template<class ReturnType, class ArgType>
	Callback<ReturnType, ArgType>::Callback() :
		m_CallbackFunction() {}

	template<class ReturnType, class ArgType>
	Callback<ReturnType, ArgType>::Callback(CallbackFunctionType callback_function) :
		m_CallbackFunction(callback_function) {}

	template<class ReturnType, class ArgType>
	ReturnType Callback<ReturnType, ArgType>::operator()(ArgType arg) {
		return m_CallbackFunction(arg);
	}

	template<class ReturnType, class ArgType>
	void Callback<ReturnType, ArgType>::set_callback(CallbackFunctionType callback_function) {
		m_CallbackFunction = callback_function;
	}

}}

