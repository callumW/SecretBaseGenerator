// Fill out your copyright notice in the Description page of Project Settings.


#include "ESet.h"

#include <stdexcept>

// template <class T>
// T ESet::get_random()
// {
//     if (!m_set.empty()) {
//         std::uniform_int_distribution<> dist(0, m_set.size() - 1);
//         int index = dist(m_rand_engine);
//
//         auto it = m_set.begin();
//         while (index-- > 0) {
//             it++;
//         }
//
//         return *it;
//     }
//     else {
//         throw std::out_of_range("cannot get random value from empty ESet");
//     }
// }
