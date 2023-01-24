#pragma once

#include<map>

namespace noveil {
	template<typename KEY, typename DATA>
	using hash_map=std::map<KEY,DATA>;
}