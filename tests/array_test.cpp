#include "internal/doctest.h"

#include <algorithm>
#include <cstddef>
#include <vector>

#include <multi/array.hpp>

TEST_CASE("multi::array")
{
  // 1D.
  {
    using array_type = multi::array<float, multi::dimensions<4>>;
    
    // Constructor tests.
    {
      array_type constructor1;
      REQUIRE(constructor1.size() == 4);

      array_type constructor2 (1.0f);
      REQUIRE(constructor2.size() == 4);
      for (auto i = 0; i < 4; ++i)
        REQUIRE(constructor2.at(i) == 1.0f);

      array_type constructor3(constructor2.storage().begin(), constructor2.storage().end());
      REQUIRE(constructor3.size() == 4);
      for (auto i = 0; i < 4; ++i)
        REQUIRE(constructor3.at(i) == 1.0f);

      array_type constructor4 {0.0f, 1.0f, 2.0f, 3.0f};
      REQUIRE(constructor4.size() == 4);
      for (auto i = 0; i < 4; ++i)
        REQUIRE(constructor4.at(i) == static_cast<float>(i));

      array_type constructor5 (constructor4);
      REQUIRE(constructor5.size() == 4);
      for (auto i = 0; i < 4; ++i)
        REQUIRE(constructor5.at(i) == static_cast<float>(i));

      array_type constructor6 (std::move(constructor4));
      REQUIRE(constructor6.size() == 4);
      for (auto i = 0; i < 4; ++i)
        REQUIRE(constructor6.at(i) == static_cast<float>(i));
    }
    
    // Assignment tests.
    {
      array_type array {0.0f, 1.0f, 2.0f, 3.0f};

      array_type assignment1 = array;
      REQUIRE(assignment1.size() == 4);
      for (auto i = 0; i < 4; ++i)
        REQUIRE(assignment1.at(i) == static_cast<float>(i));

      array_type assignment2 = std::move(assignment1);
      REQUIRE(assignment2.size() == 4);
      for (auto i = 0; i < 4; ++i)
        REQUIRE(assignment2.at(i) == static_cast<float>(i));

      array_type assignment3 = {0.0f, 1.0f, 2.0f, 3.0f};
      REQUIRE(assignment3.size() == 4);
      for (auto i = 0; i < 4; ++i)
        REQUIRE(assignment3.at(i) == static_cast<float>(i));
    }

    // Element access tests.
    {
      array_type array {0.0f, 1.0f, 2.0f, 3.0f};

      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(array.at(i)   == static_cast<float>(i));
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(array.at({i}) == static_cast<float>(i));
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(array[i]      == static_cast<float>(i));
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(array[{i}]    == static_cast<float>(i));
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(array(i)      == static_cast<float>(i));
    }

    // Iterator tests.
    {
      array_type array {0.0f, 1.0f, 2.0f, 3.0f};

      REQUIRE(array.front()    == 0.0f);
      REQUIRE(array.data ()[1] == 1.0f);
      REQUIRE(array.data ()[2] == 2.0f);
      REQUIRE(array.back ()    == 3.0f);

      REQUIRE(std::accumulate(array.cbegin(), array.cend(), 0) == 6.0f);

      std::iota(array.begin (), array.end (), 0);
      for (std::size_t i = 0; i < 4 ; ++i)
        REQUIRE(array.at(i) == static_cast<float>(i));
        
      REQUIRE(std::accumulate(array.crbegin(), array.crend(), 0) == 6.0f);

      std::iota(array.rbegin(), array.rend(), 0);
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(array.at(3 - i) == static_cast<float>(i));
    }

    // Capacity tests.
    {
      array_type array1;
      array_type array2 {0.0f, 1.0f, 2.0f, 3.0f};

      REQUIRE(!array1.empty     ());
      REQUIRE(!array2.empty     ());

      REQUIRE( array1.size      () == 4);
      REQUIRE( array2.size      () == 4);
      
      REQUIRE( array1.dimensions()[0] == 4);
      REQUIRE( array2.dimensions()[0] == 4);
    }

    // Operations tests.
    {
      array_type array1;
      array_type array2 {0.0f, 1.0f, 2.0f, 3.0f};

      array1.fill(4.0f);
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(array1.at(i) == 4.0f);

      array1.swap(array2);
      for (std::size_t i = 0; i < 4 ; ++i)
        REQUIRE(array1.at(i) == static_cast<float>(i));
    }

    // Non-member function tests.
    {
      array_type array1 {0.0f, 1.0f, 2.0f, 3.0f};
      array_type array2 {0.0f, 1.0f, 2.0f, 3.0f};
      array_type array3 {4.0f, 5.0f, 6.0f, 7.0f};

      REQUIRE(array1 == array2);
      REQUIRE(array1 != array3);
      REQUIRE(array1 <  array3);
      REQUIRE(array1 <= array3);
      REQUIRE(array3 >  array1);
      REQUIRE(array3 >= array1);

      REQUIRE(multi::get<0>(array1) == 0.0f);
      REQUIRE(multi::get<1>(array1) == 1.0f);
      REQUIRE(multi::get<2>(array1) == 2.0f);
      REQUIRE(multi::get<3>(array1) == 3.0f);

      multi::swap(array3, array1);
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(array3.at(i) == static_cast<float>(i));
    }
  }

  // 2D.
  {
    using array_type = multi::array<float, multi::dimensions<2, 2>>;
    
    // Constructor tests.
    {
      array_type constructor1;
      REQUIRE(constructor1.dimensions()[0] == 2);
      REQUIRE(constructor1.dimensions()[0] == 2);
      REQUIRE(constructor1.size() == 4);

      array_type constructor2(1.0f);
      REQUIRE(constructor2.dimensions()[0] == 2);
      REQUIRE(constructor2.dimensions()[0] == 2);
      REQUIRE(constructor2.size() == 4);
      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
          REQUIRE(constructor2.at(x, y) == 1.0f);

      array_type constructor3(constructor2.storage().begin(), constructor2.storage().end());
      REQUIRE(constructor3.size() == 4);
      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
          REQUIRE(constructor3.at(x, y) == 1.0f);

      array_type constructor4 {0.0f, 1.0f, 2.0f, 3.0f};
      REQUIRE(constructor4.size() == 4);
      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
          REQUIRE(constructor4.at(x, y) == static_cast<float>(x * 2 + y));

      array_type constructor5(constructor4);
      REQUIRE(constructor5.size() == 4);
      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
          REQUIRE(constructor5.at(x, y) == static_cast<float>(x * 2 + y));

      array_type constructor6(std::move(constructor4));
      REQUIRE(constructor6.size() == 4);
      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
          REQUIRE(constructor6.at(x, y) == static_cast<float>(x * 2 + y));
    }
    
    // Assignment tests.
    {
      array_type array {0.0f, 1.0f, 2.0f, 3.0f};

      array_type assignment1 = array;
      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
          REQUIRE(assignment1.at(x, y) == static_cast<float>(x * 2 + y));

      array_type assignment2 = std::move(assignment1);
      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
          REQUIRE(assignment2.at(x, y) == static_cast<float>(x * 2 + y));

      array_type assignment3 = {0.0f, 1.0f, 2.0f, 3.0f};
      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
          REQUIRE(assignment3.at(x, y) == static_cast<float>(x * 2 + y));
    }

    // Element access tests.
    {
      array_type array {0.0f, 1.0f, 2.0f, 3.0f};

      for (std::size_t i = 0; i < 4; ++i)
      {
        REQUIRE(array.at (i)  == static_cast<float>(i));
        REQUIRE(array.at({i}) == static_cast<float>(i));
        REQUIRE(array    [i]  == static_cast<float>(i));
        REQUIRE(array   [{i}] == static_cast<float>(i));
      }

      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
        {
          const auto target = static_cast<float>(x * 2 + y);
          REQUIRE(array.at({x, y}) == target);
          REQUIRE(array.at( x, y ) == target);
          REQUIRE(array   [{x, y}] == target);
          REQUIRE(array   ({x, y}) == target);
          REQUIRE(array   ( x, y ) == target);
        }
    }

    // Iterator tests.
    {
      array_type array {{0.0f, 1.0f, 2.0f, 3.0f}};

      REQUIRE(array.front()    == 0.0f);
      REQUIRE(array.data ()[1] == 1.0f);
      REQUIRE(array.data ()[2] == 2.0f);
      REQUIRE(array.back ()    == 3.0f);

      REQUIRE(std::accumulate(array.cbegin(), array.cend(), 0) == 6.0f);

      std::iota(array.begin (), array.end (), 0);
      for (std::size_t i = 0; i < 4 ; ++i)
        REQUIRE(array.at(i) == static_cast<float>(i));
        
      REQUIRE(std::accumulate(array.crbegin(), array.crend(), 0) == 6.0f);

      std::iota(array.rbegin(), array.rend(), 0);
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(array.at(3 - i) == static_cast<float>(i));
    }

    // Capacity tests.
    {
      array_type array1;
      array_type array2 {0.0f, 1.0f, 2.0f, 3.0f};

      REQUIRE(!array1.empty     ());
      REQUIRE(!array2.empty     ());

      REQUIRE( array1.size      () == 4);
      REQUIRE( array2.size      () == 4);
      
      REQUIRE( array1.dimensions()[0] == 2);
      REQUIRE( array1.dimensions()[1] == 2);
      REQUIRE( array2.dimensions()[0] == 2);
      REQUIRE( array2.dimensions()[1] == 2);
    }

    // Operations tests.
    {
      array_type array1;
      array_type array2 {0.0f, 1.0f, 2.0f, 3.0f};

      array1.fill(4.0f);
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(array1.at(i) == 4.0f);

      array1.swap(array2);
      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
          REQUIRE(array1.at(x, y) == static_cast<float>(x * 2 + y));
    }

    // Non-member function tests.
    {
      array_type array1 {0.0f, 1.0f, 2.0f, 3.0f};
      array_type array2 {0.0f, 1.0f, 2.0f, 3.0f};
      array_type array3 {4.0f, 5.0f, 6.0f, 7.0f};

      REQUIRE(array1 == array2);
      REQUIRE(array1 != array3);
      REQUIRE(array1 <  array3);
      REQUIRE(array1 <= array3);
      REQUIRE(array3 >  array1);
      REQUIRE(array3 >= array1);
      
      REQUIRE(multi::get<0>(array1) == 0.0f);
      REQUIRE(multi::get<1>(array1) == 1.0f);
      REQUIRE(multi::get<2>(array1) == 2.0f);
      REQUIRE(multi::get<3>(array1) == 3.0f);

      multi::swap(array3, array1);
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(array3.at(i) == static_cast<float>(i));
    }
  }
}