// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <set>
#include <random>
#include <iterator>
#include <stdexcept>
#include <utility>

/**
 * Extensiond of std::set that provides access to a random element
 */
template <class T>

class SECRETBASEGENERATOR_API ESet
{
public:

	ESet(unsigned seed = 0)
		:m_seed(seed)
	{
		m_rand_engine.seed(m_seed);
	}

	ESet(size_t count, T const& obj, unsigned seed = 0)
		:m_vec(count, obj),
		m_seed(seed)
	{
		m_rand_engine.seed(m_seed);
	}

	bool insert(T const& obj)
	{
		if (!contains(obj)) {
			m_vec.push_back(obj);
			return true;
		}
		else {
			return false;
		}
	}
	void erase(T const& obj)
	{
		for (auto it = m_vec.begin(); it != m_vec.end(); it++) {
			if (obj == *it) {
				m_vec.erase(it);
				return;
			}
		}
	}

	void replace(T const& obj)
	{
		erase(obj);
		insert(obj);
	}

	typename std::vector<T>::iterator begin() { return m_vec.begin(); }
	typename std::vector<T>::iterator end() { return m_vec.end(); }

	typename std::vector<T>::iterator erase(typename std::vector<T>::iterator it) { return m_vec.erase(it); }

	bool contains(T const& obj) const
	{
		auto loc = where_is(obj);
		return loc.second;
	}

	std::pair<typename std::vector<T>::const_iterator, bool> where_is(T const& obj) const
	{
		for (auto it = m_vec.begin(); it != m_vec.end(); it++) {
			if (obj == *it) {
				return std::make_pair(it, true);
			}
		}
		return std::make_pair(m_vec.end(), false);
	}

	T& get_random()
	{
		size_t i;
		return get_random(i);
	}

	T& get_random(size_t & i)
	{
		if (!empty()) {
			if (size() == 1) {
				i = 0;
			}
			else {
				std::uniform_int_distribution<> dist(0, m_vec.size() - 1);
				i = dist(m_rand_engine);
			}

			UE_LOG(LogTemp, Warning, TEXT("returning index: %d"), i);
			return m_vec[i];
		}
		else {
			throw std::out_of_range("cannot get random value from empty ESet");
		}
	}

	T& operator[](size_t i)
	{
		if (i < m_vec.size())
			return m_vec[i];
		else {
			throw std::out_of_range("ESet: invalid index access");
		}
	}

	bool empty() const
	{
		return m_vec.empty();
	}

	size_t size() const
	{
		return m_vec.size();
	}


	std::vector<T> to_vector() const { return m_vec; }

	std::vector<T>& vec() { return m_vec; }

protected:
	std::vector<T> m_vec;
	unsigned m_seed;

	std::mt19937 m_rand_engine;
};
