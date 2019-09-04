#include <iostream>
#include <random>
#include <chrono>
#include <cassert>

/* set to non-zero to use the same seed */
static constexpr unsigned int seed_value = 0;
/* array size to sort */
static constexpr size_t array_sz = 10;
/* test iterations count */
static constexpr size_t loops = 10000000;

static unsigned bubble_arr[loops][array_sz];
static unsigned qsort_arr[loops][array_sz];

static unsigned int get_seed(void)
{
	unsigned seed;
	if (seed_value) {
		seed = seed_value;
	} else {
		std::random_device rng_dev;
		seed = rng_dev();
	}

	std::cout << "Seed: " << seed << "\n";
	return seed;
}

static void bubble_sort(unsigned *a, size_t size)
{
	for (size_t i = 0; i < size - 1; i++) {
		for (size_t j = i + 1; j < size; j++) {
			if (a[i] > a[j]) {
				auto t = a[i];
				a[i] = a[j];
				a[j] = t;
			}
		}
	}
}

static void quick_sort(unsigned *a, long lo, long hi)
{
	long i = lo;
	long j = hi;
	auto pivot = a[(i + j) / 2];

	while (i <= j) {
		while (a[i] < pivot)
			i++;
		while (a[j] > pivot)
			j--;
		if (i <= j) {
			auto t = a[i];
			a[i] = a[j];
			a[j] = t;
			i++;
			j--;
		}
	}

	if (j > lo)
		quick_sort(a, lo, j);
	if (i < hi)
		quick_sort(a, i, hi);
}

int main(void)
{
	namespace chrono = std::chrono;

	std::mt19937 rng(get_seed());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 99);

	std::cout << "Array size: " << array_sz << "\nLoops: " << loops << "\n";

	for (size_t i = 0; i < loops; i++)
		for (size_t j = 0; j < array_sz; j++)
			bubble_arr[i][j] = qsort_arr[i][j] = dist(rng);

	/* bubble sort */
	auto start = chrono::high_resolution_clock::now();
	for (size_t i = 0; i < loops; i++)
		bubble_sort(bubble_arr[i], array_sz);
	auto end = chrono::high_resolution_clock::now();

	std::cout << "Bubble sort time: " <<
		chrono::duration_cast<chrono::microseconds>(end - start).count() <<
		" us\n";

	/* quick sort */
	start = chrono::high_resolution_clock::now();
	for (size_t i = 0; i < loops; i++)
		quick_sort(qsort_arr[i], 0, array_sz - 1);
	end = chrono::high_resolution_clock::now();

	std::cout << "Quick sort time: " <<
		chrono::duration_cast<chrono::microseconds>(end - start).count() <<
		" us\n";

	for (size_t i = 0; i < loops; i++)
		for (size_t j = 0; j < array_sz; j++)
			assert(bubble_arr[i][j] == qsort_arr[i][j]);

	return 0;
}
