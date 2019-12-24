/*
	ARRAY.H
	-------
*/
#pragma once

namespace BASIC
	{
	class array
		{
		public:
			std::vector<size_t> dimensions;
			std::vector<size_t> multiplier;
			std::vector<symbol> data;

		public:
			/*
				ARRAY::ARRAY()
				--------------
			*/
			array()
				{
				/*
					default array size is 10 elements.
				*/
				dimensions.push_back(10);
				multiplier.push_back(1);
				data.resize(10);
				}

			/*
				ARRAY::ARRAY()
				--------------
			*/
			array(const std::vector<size_t> &dim)
				{
				size_t so_far = 1;
				dimensions = dim;
				multiplier.resize(dim.size());
				for (size_t index = 0; index < dim.size(); index++)
					{
					multiplier[index] = so_far;
					so_far *= dim[index];
					}

				data.resize(so_far);
				}

			/*
				ARRAY::OPERATOR[]()
				-------------------
			*/
			symbol &operator[](const std::vector<size_t> &where)
				{
				if (where.size() != dimensions.size())
					throw error::bad_subscript_error();

				size_t index = 0;
				for (size_t dim = 0; dim < where.size(); dim++)
					if (where[dim] > dimensions[dim] || where[dim] < 0)
						throw error::bad_subscript_error();
					else
						index += where[dim] * multiplier[dim];

				return data[index];
				}
		};
	}
