#pragma once

namespace Engine
{
	template <typename T>
	void SafeRelease(T& pointer)
	{
		if (pointer)
		{
			pointer->Release();
			pointer = nullptr;
		}
	}

	class NonCopyable
	{
	protected:
		NonCopyable() = default;
		~NonCopyable() = default;
		NonCopyable(const NonCopyable& other) = delete;
		NonCopyable& operator=(const NonCopyable& other) = delete;
		NonCopyable(NonCopyable&& other) = delete;
		NonCopyable& operator=(NonCopyable&& other) = delete;
	};

	template <typename T>
	class Singleton
	{
	protected:
		Singleton() = default;
		~Singleton() = default;
		Singleton(const Singleton& other) = delete;
		Singleton& operator=(const Singleton& other) = delete;
		Singleton(Singleton&& other) = delete;
		Singleton& operator=(Singleton&& other) = delete;

	public:
		static T* GetInstance()
		{
			if (nullptr == _pInstance)
				_pInstance = new T;

			return _pInstance;
		}
		static void ResetInstance() { _pInstance = nullptr; }
	private:
		static T* _pInstance;
	};
	template <typename T>
	T* Singleton<T>::_pInstance = nullptr;

	template <typename T>
	struct Getter
	{
		inline const T& operator()(const T& value) { return value; }
	};
	
	template <typename T>
	struct Setter
	{
		inline void operator()(T& out, const T& other) { out = other; }
	};

	constexpr bool READ_ONLY = true;
	template <typename T, bool readOnly = false, class Gettor = Getter<T>, class Settor = Setter<T>>
	class Property
	{
	public:
		Property(T& origin, Gettor gettor = Gettor(), Settor settor = Settor())
			: _value(origin), _gettor(gettor), _settor(settor) {}

		inline operator const T&() { return _gettor(_value); }
		inline const T& operator->() const { return _value; }
		inline const T& operator*() const { return _value; }
		inline void operator=(const T& other) requires (!readOnly)
		{ _settor(_value, other); }
		inline void operator=(const Property<T>& other) requires (!readOnly)
		{ _settor(_value, other._value); }
		inline void operator+=(const T& other) requires (!readOnly)
		{ _settor(_value, _value + other); }
		inline void operator-=(const T& other) requires (!readOnly)
		{ _settor(_value, _value - other); }
		inline void operator*=(const T& other) requires (!readOnly)
		{ _settor(_value, _value * other); }
		inline void operator/=(const T& other) requires (!readOnly)
		{ _settor(_value, _value / other); }
	private:
		T& _value;
		Gettor _gettor;
		Settor _settor;
	};
}