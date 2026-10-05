#include "FFT.h"
#include <complex>
#include <iostream>
#include <cmath>
#include <type_traits>

// 判断 C++ 标准版本
#if defined(_MSVC_LANG)
	#define CPP_STD _MSVC_LANG
#else
	#define CPP_STD __cplusplus
#endif

#if CPP_STD >= 202002L
	#include <bit>
#endif

//原理讲解链接
//抖音
//https://v.douyin.com/DKurI287y8I/
//B站
//https://www.bilibili.com/video/BV16k2wBsEwF/

static constexpr double PI = acos(-1);

std::vector<std::complex<double>>FFT(std::vector<std::complex<double>>nums, bool o) {
	int len = nums.size();
	if (len == 1) {
		return nums;
	}
	std::vector<std::complex<double>>odd;	//奇次项
	std::vector<std::complex<double>>even;	//偶次项

	//预分配内存
	odd.reserve(len / 2);
	even.reserve(len / 2);
	for (int i = 0; i < len; i++) {
		if (i & 1) {	//奇次项
			odd.push_back(nums[i]);
		} else {		//偶次项
			even.push_back(nums[i]);
		}
	}
	odd  = FFT(odd, o);
	even = FFT(even, o);

	std::complex<double> w;
	if (o) {
		w = std::polar(1.0, 2.0 * PI / len); // 角度就是 2πi/len,长度为1的复数
	} else {
		w = std::polar(1.0, -2.0 * PI / len);
	}
	std::vector<std::complex<double>> res(len);
	for (int i = 0; i < len / 2; i++) {
		std::complex<double> w_i =  pow(w, i);
		res[i]           = even[i] + odd[i] * w_i;
		res[i + len / 2] = even[i] - odd[i] * w_i;
	}
	return res;
};



template<typename T>
std::vector<T> FFT_solution(std::vector<T>a, std::vector<T>b) {
	int len_a = a.size(), len_b = b.size(), len = len_a + len_b;

	//补齐长度为2的幂 ( >= len-1 的最小2的幂 )
#if CPP_STD >= 202002L
	int n = static_cast<int>(std::bit_ceil(static_cast<unsigned>(len - 1)));
#else
	int n = 1;
	while (n < len - 1) n <<= 1;
#endif

	//转换为复数
	std::vector<std::complex<double>>complex_a(n), complex_b(n);
	for (int i = 0; i < len_a; i++) {
		complex_a[i] = std::complex<double>(static_cast<double>(a[i]));
	}
	for (int i = 0; i < len_b; i++) {
		complex_b[i] = std::complex<double>(static_cast<double>(b[i]));
	}
	complex_a = FFT(complex_a, true);
	complex_b = FFT(complex_b, true);

	// 频域逐点相乘
	std::vector<std::complex<double>> prod(n);
	for (int i = 0; i < n; i++) {
		prod[i] = complex_a[i] * complex_b[i] ;
	}
	prod = FFT(prod, false);

	std::vector<T>res(len - 1);
	if  constexpr (std::is_integral_v<T>) {//如果T是整数类型，需要四舍五入
		for (int i = 0; i < len - 1; i++ ) {
			res[i] = static_cast<T>(std::llround(prod[i].real() / n));
		}
	} else {
		for (int i = 0; i < len - 1; i++ ) {
			res[i] = static_cast<T>(prod[i].real() / n);
		}
	}

	return res;
}


template<typename T>
std::vector<T> normal_solution(std::vector<T>a, std::vector<T>b) {
	std::vector<T>res(a.size() + b.size() - 1);
	for (int i = 0; i < a.size(); i++) {
		for (int j = 0; j < b.size(); j++) {
			res[i + j] += a[i] * b[j];
		}
	}
	return res;
}



#include <random>
#include <chrono>
int main() {
	int len_a = 1e5, len_b = 1e5;
	int mn_num = -1000, mx_num = 1000;	//随机数范围 [mn_num, mx_num]


	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dist(mn_num, mx_num);

	std::vector<double>a(len_a), b(len_b);
	for (auto& x : a) x = dist(gen);
	for (auto& x : b) x = dist(gen);

	auto t1 = std::chrono::steady_clock::now();
	auto nums1 = FFT_solution(a, b);
	auto t2 = std::chrono::steady_clock::now();
	auto nums2 = normal_solution(a, b);
	auto t3 = std::chrono::steady_clock::now();

	auto d_fft    = std::chrono::duration<double, std::milli>(t2 - t1).count();
	auto d_normal = std::chrono::duration<double, std::milli>(t3 - t2).count();

	/** 需要查看运行结果请把下面的注释去除 */
//	std::cout << "a = ";
//	for (auto x : a) std::cout << x << " ";
//	std::cout << "\nb = ";
//	for (auto x : b) std::cout << x << " ";
//	std::cout << "\n\n";
//
//	std::cout << "nums1 = ";
//	for (auto x : nums1) std::cout << x << " ";
//	std::cout << std::endl;
//	std::cout << "nums2 = ";
//	for (auto x : nums2) std::cout << x << " ";
//	std::cout << std::endl;

	std::cout << "FFT_solution    : " << d_fft    << " ms\n";
	std::cout << "normal_solution : " << d_normal << " ms\n\n";
	return 0;
}
