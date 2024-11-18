#pragma once

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define FAILED_CHECK_RETURN(hr, message) if (((HRESULT)(hr)) < 0) return hr;
#define FAILED_CHECK_THROW(hr, message) hr;

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
			: _value(origin), _gettor(gettor), _settor(settor) {}

		inline operator const T& () const { return _gettor(_value); }

		// ReadOnly
		inline const T& operator->() const requires (std::is_pointer_v<T> && AccessMode == READ_ONLY) { return _value; }
		inline const T* operator->() const requires (!std::is_pointer_v<T> && AccessMode == READ_ONLY) { return &_value; }
		
		// Read&Write
		inline T& operator->() requires (std::is_pointer_v<T> && AccessMode == READ_WRITE) { return _gettor(_value); }
		inline T* operator->() requires (!std::is_pointer_v<T> && AccessMode == READ_WRITE) { return &(_gettor(_value)); }

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

	inline std::vector<uint8_t> ReadData(_In_z_ const wchar_t* name)
	{
		std::ifstream inFile(name, std::ios::in | std::ios::binary | std::ios::ate);

		#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
		if (!inFile)
		{
			wchar_t moduleName[_MAX_PATH] = {};
			if (!GetModuleFileNameW(nullptr, moduleName, _MAX_PATH))
				throw std::system_error(std::error_code(static_cast<int>(GetLastError()), std::system_category()), "GetModuleFileNameW");

			wchar_t drive[_MAX_DRIVE];
			wchar_t path[_MAX_PATH];

			if (_wsplitpath_s(moduleName, drive, _MAX_DRIVE, path, _MAX_PATH, nullptr, 0, nullptr, 0))
				throw std::runtime_error("_wsplitpath_s");

			wchar_t filename[_MAX_PATH];
			if (_wmakepath_s(filename, _MAX_PATH, drive, path, name, nullptr))
				throw std::runtime_error("_wmakepath_s");

			inFile.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
		}
		#endif

		if (!inFile)
		{
			return std::vector<uint8_t>();
			// throw std::runtime_error("ReadData");
		}

		const std::streampos len = inFile.tellg();

		std::vector<uint8_t> blob;
		blob.resize(size_t(len));

		inFile.seekg(0, std::ios::beg);
		if (!inFile)
			throw std::runtime_error("ReadData");

		inFile.read(reinterpret_cast<char*>(blob.data()), len);
		if (!inFile)
			throw std::runtime_error("ReadData");

		inFile.close();

		return blob;
	}
}