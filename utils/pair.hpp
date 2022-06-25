#pragma once

namespace ft {

	template <class T1, class T2>
		struct pair
		{
		public :

			typedef T1 first_type;
			typedef T2 second_type;
			first_type first;
			second_type second;

			pair()
			:
				first(),
				second()
			{}

			template<class U, class V>
				pair (const pair<U, V>& pr)
				:
					first(pr.first),
					second(pr.second)
				{}

			pair (const first_type& a, const second_type& b)
			:
				first(a),
				second(b)
			{}

			pair& operator=(const pair& pr)
			{
				if (*this == pr)
					return (*this);
				// remove_const(this->first) = pr.first;
				this->first = pr.first;
				this->second = pr.second;
				return (*this);
			}
		};

}
