#pragma once

#include <vector>

namespace Polaris
{
	/*
	* Add unique element into a vector
	*/
	template <typename T>
	void add_unique(std::vector<T>& v, T element) {
		if (std::find(v.begin(), v.end(), element) == v.end())
		{
			v.push_back(element);
		}
	}

	/*
	* const char* specialization
	*/
	template <>
	void add_unique(std::vector<const char*>& v, const char* c_str) {
		bool contains = false;
		for (const char* e : v) {
			if (!strcmp(e, c_str)) {
				contains = true;
				break;
			}
		}
		if (!contains) v.push_back(c_str);
	}
} // namespace Polaris

