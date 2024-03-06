// #include <Windows.h>
#pragma once
#define NOMINMAX
#define logh(x) std::cout << x << "\n"
#include <memory>

#include <iostream>
#define logvec2(X) std::cout << X.x << ", " << X.y << "\n";
#define logvec3(X) std::cout << X.x << ", " << X.y << X.z << "\n";


#ifdef JR_PLATFORM_WINDOWS
#ifdef JR_BUILD_DLL
#define JAGUAR_API __declspec(dllexport)
#else
#define JAGUAR_API __declspec(dllimport)
#endif
#else
#error JAGUAR only supports windows!
#endif

template <typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Scope<T> MakeScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> MakeRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}
