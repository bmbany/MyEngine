#pragma once

template <typename T>
struct Getter
{
	inline T& operator()(T& value) const { return value; }
};

template <typename T>
struct Setter
{
	inline void operator()(T& out, const T& other) { out = other; }
};

constexpr bool READ_ONLY = true;
constexpr bool READ_WRITE = false;
template <typename T, bool AccessMode = READ_WRITE, class Gettor = Getter<T>, class Settor = Setter<T>>
class Property
{
public:
	Property(T& origin, Gettor gettor = Gettor(), Settor settor = Settor())
		: _value(origin), _gettor(gettor), _settor(settor) { }

	inline operator const T& () const { return _gettor(_value); }

	// ReadOnly
	inline const T& operator->() const requires (std::is_pointer_v<T>&& AccessMode == READ_ONLY) { return _value; }
	inline const T* operator->() const requires (!std::is_pointer_v<T>&& AccessMode == READ_ONLY) { return &_value; }

	// Read&Write
	inline T& operator->() requires (std::is_pointer_v<T>&& AccessMode == READ_WRITE) { return _gettor(_value); }
	inline T* operator->() requires (!std::is_pointer_v<T>&& AccessMode == READ_WRITE) { return &(_gettor(_value)); }

	inline Property& operator=(const Property& other) requires (AccessMode == READ_WRITE)
	{
		if (this != &other)
		{
			_settor(_value, other._gettor(other._value));
		}
		return *this;
	}

	inline Property& operator=(const T& other) requires (AccessMode == READ_WRITE)
	{
		_settor(_value, other);
		return *this;
	}

	inline void operator+=(const T& other) requires (AccessMode == READ_WRITE)
	{
		_settor(_value, _gettor(_value) + other);
	}

	inline void operator-=(const T& other) requires (AccessMode == READ_WRITE)
	{
		_settor(_value, _gettor(_value) - other);
	}

	inline void operator*=(const T& other) requires (AccessMode == READ_WRITE)
	{
		_settor(_value, _gettor(_value) * other);
	}

	inline void operator/=(const T& other) requires (AccessMode == READ_WRITE)
	{
		_settor(_value, _gettor(_value) / other);
	}

private:
	T& _value;
	Gettor _gettor;
	Settor _settor;
};