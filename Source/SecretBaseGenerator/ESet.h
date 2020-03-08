// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <set>
#include <random>
#include <iterator>
#include <stdexcept>

/**
 * Extensiond of std::set that provides access to a random element
 */
template <class T, class Compare = std::less<T>>
class SECRETBASEGENERATOR_API ESet
{
public:

	ESet(unsigned seed = 0):m_seed(seed) { m_rand_engine.seed(m_seed); }

	void set_seed(unsigned seed);

	bool insert(T const& obj) { return m_set.insert(obj).second; }
	void erase(T const& obj) { m_set.erase(obj); }

	auto begin() { return m_set.begin(); }
	auto end() { return m_set.end(); }

	bool contains(T const& obj) { return m_set.end() != m_set.find(obj); }

	T get_random()
	{
		if (!m_set.empty()) {
			std::uniform_int_distribution<> dist(0, m_set.size() - 1);
			int index = dist(m_rand_engine);

			auto it = m_set.begin();
			while (index-- > 0) {
				it++;
			}

			return *it;
		}
		else {
			throw std::out_of_range("cannot get random value from empty ESet");
		}
	}

protected:
	std::set<T, Compare> m_set;
	unsigned m_seed;

	std::mt19937 m_rand_engine;
};
