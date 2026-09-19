#include "FFT.h"
#include <complex>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <functional>

//原理讲解链接
//抖音
//https://v.douyin.com/DKurI287y8I/
//B站
//https://www.bilibili.com/video/BV16k2wBsEwF/

static constexpr double PI=acos(-1);

std::vector<std::complex<double>>FFT(std::vector<std::complex<double>>nums, int n) {
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
	odd=FFT(odd,n);
	even=FFT(even,n);
	
	std::complex<double>w(cos(PI*len/n),sin(PI*len/n));
	std::vector<std::complex<double>>res(len);//返回的列表
	for(int i=0;i<len/2;i++){
		res[i]=even[i];
		res[i*2+1]=odd[i]*w;
	}
	return res;
};



template<typename T>
std::vector<T> Compute_convolution(std::vector<T>a, std::vector<T>b) {
	int len_a = a.size(), len_b = b.size();
	int n = 1 << (std::__lg(static_cast<uint32_t>(len_a + len_b - 1)) + 1); //补齐长度为2的幂
	std::vector<std::complex<double>>complex_a(n), complex_b(n);
	for (int i = 0; i < len_a; i++) {
		complex_a[i] = std::complex<double>(static_cast<double>(a[i]));
	}
	for (int i = 0; i < len_b; i++) {
		complex_b[i] = std::complex<double>(static_cast<double>(b[i]));
	}
	complex_a=FFT(complex_a,n);
	complex_b=FFT(complex_b,n);
	

}

int main() {
	std::cout<<PI;

	return 0;
}
