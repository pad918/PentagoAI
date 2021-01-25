#pragma once
#include "Pentago.h"

namespace std {
	template <>
	struct hash<ptg::PentagoSubBoard>
	{
		std::size_t operator()(const ptg::PentagoSubBoard& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<string>()(k.marbles)
				^ (hash<string>()(k.second) << 1)) >> 1)
				^ (hash<int>()(k.third) << 1);
		}
	};
}