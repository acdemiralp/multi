#include "internal/doctest.h"

#include <algorithm>
#include <cstddef>
#include <vector>

#include <multi/vector.hpp>

TEST_CASE("multi::vector")
{
  // 1D.
  {
    using vector_type    = multi::vector<float, 1>;
    using allocator_type = typename vector_type::allocator_type;

    // Constructor tests.
    {
      vector_type constructor1;
      REQUIRE(constructor1.size() == 0);

      vector_type constructor2 {allocator_type()};
      REQUIRE(constructor2.size() == 0);

      vector_type constructor3 (3, 1.0f);
      REQUIRE(constructor3.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(constructor3.at(i) == 1.0f);

      vector_type constructor4 (3, allocator_type());
      REQUIRE(constructor4.size() == 3);

      vector_type constructor5 (constructor3.storage().begin(), constructor3.storage().end());
      REQUIRE(constructor5.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(constructor5.at(i) == 1.0f);

      vector_type constructor6 {0.0f, 1.0f, 2.0f};
      REQUIRE(constructor6.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(constructor6.at(i) == static_cast<float>(i));

      vector_type constructor7 (constructor6);
      REQUIRE(constructor7.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(constructor7.at(i) == static_cast<float>(i));

      vector_type constructor8 (constructor6, allocator_type());
      REQUIRE(constructor8.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(constructor8.at(i) == static_cast<float>(i));

      vector_type constructor9 (std::move(constructor6));
      REQUIRE(constructor9.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(constructor9.at(i) == static_cast<float>(i));

      vector_type constructor10(std::move(constructor7), allocator_type());
      REQUIRE(constructor10.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(constructor10.at(i) == static_cast<float>(i));
    }
    
    // Assignment tests.
    {
      vector_type vector {0.0f, 1.0f, 2.0f};

      vector_type assignment1 = vector;
      REQUIRE(assignment1.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(assignment1.at(i) == static_cast<float>(i));

      vector_type assignment2 = std::move(assignment1);
      REQUIRE(assignment2.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(assignment2.at(i) == static_cast<float>(i));

      vector_type assignment3 = {0.0f, 1.0f, 2.0f};
      REQUIRE(assignment3.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(assignment3.at(i) == static_cast<float>(i));

      vector_type assignment4;
      assignment4.assign(3, 1.0f);
      REQUIRE(assignment4.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(assignment4.at(i) == 1.0f);

      vector_type assignment5;
      assignment5.assign(vector.begin(), vector.end());
      REQUIRE(assignment5.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(assignment5.at(i) == static_cast<float>(i));

      vector_type assignment6;
      assignment6.assign({0.0f, 1.0f, 2.0f});
      REQUIRE(assignment6.size() == 3);
      for (auto i = 0; i < 3; ++i)
        REQUIRE(assignment6.at(i) == static_cast<float>(i));
    }

    // Element access tests.
    {
      vector_type vector {0.0f, 1.0f, 2.0f};

      for (std::size_t i = 0; i < 3; ++i)
        REQUIRE(vector.at(i)   == static_cast<float>(i));
      for (std::size_t i = 0; i < 3; ++i)
        REQUIRE(vector.at({i}) == static_cast<float>(i));
      for (std::size_t i = 0; i < 3; ++i)
        REQUIRE(vector[i]      == static_cast<float>(i));
      for (std::size_t i = 0; i < 3; ++i)
        REQUIRE(vector[{i}]    == static_cast<float>(i));
      for (std::size_t i = 0; i < 3; ++i)
        REQUIRE(vector(i)      == static_cast<float>(i));
    }

    // Iterator tests.
    {
      vector_type vector {0.0f, 1.0f, 2.0f};

      REQUIRE(vector.front()    == 0.0f);
      REQUIRE(vector.data ()[1] == 1.0f);
      REQUIRE(vector.back ()    == 2.0f);

      REQUIRE(std::accumulate(vector.cbegin(), vector.cend(), 0) == 3.0f);

      std::iota(vector.begin (), vector.end (), 0);
      for (std::size_t i = 0; i < 3 ; ++i)
        REQUIRE(vector.at(i) == static_cast<float>(i));
        
      REQUIRE(std::accumulate(vector.crbegin(), vector.crend(), 0) == 3.0f);

      std::iota(vector.rbegin(), vector.rend(), 0);
      for (std::size_t i = 0; i < 3; ++i)
        REQUIRE(vector.at(2 - i) == static_cast<float>(i));
    }
    
    // Capacity tests.
    {
      vector_type vector1;
      vector_type vector2 {0.0f, 1.0f, 2.0f};

      REQUIRE( vector1.empty     ());
      REQUIRE(!vector2.empty     ());

      REQUIRE( vector1.size      () == 0);
      REQUIRE( vector2.size      () == 3);
      
      REQUIRE( vector1.dimensions()[0] == 0);
      REQUIRE( vector2.dimensions()[0] == 3);
    }

    // Modifier tests.
    {
      vector_type vector1 {0.0f, 1.0f, 2.0f};
      vector_type vector2;

      REQUIRE(!vector1.empty());
      vector1.clear();
      REQUIRE( vector1.empty());

      vector1.resize(3);
      REQUIRE(vector1.size() == 3);

      vector2.resize(3, 1.0f);
      REQUIRE(vector2.size() == 3);
      for (std::size_t i = 0; i < 3; ++i)
        REQUIRE(vector2.at(i) == 1.0f);

      vector2.swap(vector1);
      REQUIRE(vector2.size() == 3);
      for (std::size_t i = 0; i < 3 ; ++i)
        REQUIRE(vector2.at(i) == 0.0f);
    }

    // Non-member function tests.
    {
      vector_type vector1{0.0f, 1.0f, 2.0f};
      vector_type vector2{0.0f, 1.0f, 2.0f};
      vector_type vector3{3.0f, 4.0f, 5.0f};

      REQUIRE(vector1 == vector2);
      REQUIRE(vector1 != vector3);
      REQUIRE(vector1 <  vector3);
      REQUIRE(vector1 <= vector3);
      REQUIRE(vector3 >  vector1);
      REQUIRE(vector3 >= vector1);

      multi::swap(vector3, vector1);
      for (std::size_t i = 0; i < 3; ++i)
        REQUIRE(vector3.at(i) == static_cast<float>(i));
    }
  }

  // 2D.
  {
    using vector_type    = multi::vector<float, 2>;
    using allocator_type = typename vector_type::allocator_type;

    // Constructor tests.
    {
      vector_type constructor1;
      REQUIRE(constructor1.dimensions()[0] == 0);
      REQUIRE(constructor1.dimensions()[1] == 0);
      REQUIRE(constructor1.size() == 0);

      vector_type constructor2 {allocator_type()};
      REQUIRE(constructor2.dimensions()[0] == 0);
      REQUIRE(constructor2.dimensions()[1] == 0);
      REQUIRE(constructor2.size() == 0);

      vector_type constructor3 ({3, 3}, 1.0f);
      REQUIRE(constructor3.dimensions()[0] == 3);
      REQUIRE(constructor3.dimensions()[1] == 3);
      REQUIRE(constructor3.size() == 9);
      for (std::size_t x = 0; x < 3; ++x)
        for (std::size_t y = 0; y < 3; ++y)
          REQUIRE(constructor3.at(x, y) == 1.0f);

      vector_type constructor4 ({3, 3}, allocator_type());
      REQUIRE(constructor4.dimensions()[0] == 3);
      REQUIRE(constructor4.dimensions()[1] == 3);
      REQUIRE(constructor4.size() == 9);

      vector_type constructor5 ({3, 3}, constructor3.storage().begin(), constructor3.storage().end());
      REQUIRE(constructor5.dimensions()[0] == 3);
      REQUIRE(constructor5.dimensions()[1] == 3);
      REQUIRE(constructor5.size() == 9);
      for (std::size_t x = 0; x < 3; ++x)
        for (std::size_t y = 0; y < 3; ++y)
          REQUIRE(constructor5.at(x, y) == 1.0f);

      vector_type constructor6 ({3, 3}, {0.0f, 1.0f, 2.0f});
      REQUIRE(constructor6.dimensions()[0] == 3);
      REQUIRE(constructor6.dimensions()[1] == 3);
      REQUIRE(constructor6.size() == 9);
      for (std::size_t y = 0; y < 3; ++y)
        REQUIRE(constructor6.at(0, y) == static_cast<float>(y));
      
      vector_type constructor7 (constructor6);
      REQUIRE(constructor7.dimensions()[0] == 3);
      REQUIRE(constructor7.dimensions()[1] == 3);
      REQUIRE(constructor7.size() == 9);
      for (std::size_t y = 0; y < 3; ++y)
        REQUIRE(constructor7.at(0, y) == static_cast<float>(y));
      
      vector_type constructor8 (constructor6, allocator_type());
      REQUIRE(constructor8.dimensions()[0] == 3);
      REQUIRE(constructor8.dimensions()[1] == 3);
      REQUIRE(constructor8.size() == 9);
      for (std::size_t y = 0; y < 3; ++y)
        REQUIRE(constructor8.at(0, y) == static_cast<float>(y));
      
      vector_type constructor9 (std::move(constructor6));
      REQUIRE(constructor9.dimensions()[0] == 3);
      REQUIRE(constructor9.dimensions()[1] == 3);
      REQUIRE(constructor9.size() == 9);
      for (std::size_t y = 0; y < 3; ++y)
        REQUIRE(constructor9.at(0, y) == static_cast<float>(y));
      
      vector_type constructor10(std::move(constructor7), allocator_type());
      REQUIRE(constructor10.dimensions()[0] == 3);
      REQUIRE(constructor10.dimensions()[1] == 3);
      REQUIRE(constructor10.size() == 9);
      for (std::size_t y = 0; y < 3; ++y)
        REQUIRE(constructor10.at(0, y) == static_cast<float>(y));
    }
    
    // Assignment tests.
    {
      vector_type vector({3, 3}, {0.0f, 1.0f, 2.0f});

      vector_type assignment1 = vector;
      REQUIRE(assignment1.dimensions()[0] == 3);
      REQUIRE(assignment1.dimensions()[1] == 3);
      REQUIRE(assignment1.size() == 9);
      for (std::size_t y = 0; y < 3; ++y)
        REQUIRE(assignment1.at(0, y) == static_cast<float>(y));

      vector_type assignment2 = std::move(assignment1);
      REQUIRE(assignment2.dimensions()[0] == 3);
      REQUIRE(assignment2.dimensions()[1] == 3);
      REQUIRE(assignment2.size() == 9);
      for (std::size_t y = 0; y < 3; ++y)
        REQUIRE(assignment2.at(0, y) == static_cast<float>(y));
      
      vector_type assignment3 = {{3, 3}, {0.0f, 1.0f, 2.0f}};
      REQUIRE(assignment3.dimensions()[0] == 3);
      REQUIRE(assignment3.dimensions()[1] == 3);
      REQUIRE(assignment3.size() == 9);
      for (std::size_t y = 0; y < 3; ++y)
        REQUIRE(assignment3.at(0, y) == static_cast<float>(y));
      
      vector_type assignment4;
      assignment4.assign({3, 3}, 1.0f);
      REQUIRE(assignment4.dimensions()[0] == 3);
      REQUIRE(assignment4.dimensions()[1] == 3);
      REQUIRE(assignment4.size() == 9);
      for (std::size_t x = 0; x < 3; ++x)
        for (std::size_t y = 0; y < 3; ++y)
          REQUIRE(assignment4.at(x, y) == 1.0f);
      
      vector_type assignment5;
      assignment5.assign({3, 3}, vector.begin(), vector.end());
      REQUIRE(assignment5.dimensions()[0] == 3);
      REQUIRE(assignment5.dimensions()[1] == 3);
      REQUIRE(assignment5.size() == 9);
      for (std::size_t y = 0; y < 3; ++y)
        REQUIRE(assignment5.at(0, y) == static_cast<float>(y));
      
      vector_type assignment6;
      assignment6.assign({3, 3}, {0.0f, 1.0f, 2.0f});
      REQUIRE(assignment6.dimensions()[0] == 3);
      REQUIRE(assignment6.dimensions()[1] == 3);
      REQUIRE(assignment6.size() == 9);
      for (std::size_t y = 0; y < 3; ++y)
        REQUIRE(assignment6.at(0, y) == static_cast<float>(y));
    }
    
    // Element access tests.
    {
      vector_type vector {{2, 2}, {0.0f, 1.0f, 2.0f, 3.0f}};

      for (std::size_t i = 0; i < 4; ++i)
      {
        REQUIRE(vector.at (i)  == static_cast<float>(i));
        REQUIRE(vector.at({i}) == static_cast<float>(i));
        REQUIRE(vector    [i]  == static_cast<float>(i));
        REQUIRE(vector   [{i}] == static_cast<float>(i));
      }

      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
        {
          const auto target = static_cast<float>(x * 2 + y);
          REQUIRE(vector.at({x, y}) == target);
          REQUIRE(vector.at( x, y ) == target);
          REQUIRE(vector   [{x, y}] == target);
          REQUIRE(vector   ({x, y}) == target);
          REQUIRE(vector   ( x, y ) == target);
        }
    }

    // Iterator tests.
    {
      vector_type vector {{2, 2}, {0.0f, 1.0f, 2.0f, 3.0f}};

      REQUIRE(vector.front()    == 0.0f);
      REQUIRE(vector.data ()[1] == 1.0f);
      REQUIRE(vector.back ()    == 3.0f);

      REQUIRE(std::accumulate(vector.cbegin(), vector.cend(), 0) == 6.0f);

      std::iota(vector.begin (), vector.end (), 0);
      for (std::size_t i = 0; i < 4 ; ++i)
        REQUIRE(vector.at(i) == static_cast<float>(i));
        
      REQUIRE(std::accumulate(vector.crbegin(), vector.crend(), 0) == 6.0f);

      std::iota(vector.rbegin(), vector.rend(), 0);
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(vector.at(3 - i) == static_cast<float>(i));
    }
    
    // Capacity tests.
    {
      vector_type vector1;
      vector_type vector2 {{2, 2}, {0.0f, 1.0f, 2.0f, 3.0f}};

      REQUIRE( vector1.empty     ());
      REQUIRE(!vector2.empty     ());

      REQUIRE( vector1.size      () == 0);
      REQUIRE( vector2.size      () == 4);
      
      REQUIRE( vector1.dimensions()[0] == 0);
      REQUIRE( vector1.dimensions()[1] == 0);
      REQUIRE( vector2.dimensions()[0] == 2);
      REQUIRE( vector2.dimensions()[1] == 2);
    }

    // Modifier tests.
    {
      vector_type vector1 {{2, 2}, {0.0f, 1.0f, 2.0f, 3.0f}};
      vector_type vector2;

      REQUIRE(!vector1.empty());
      vector1.clear();
      REQUIRE( vector1.empty());

      vector1.resize({2, 2});
      REQUIRE(vector1.dimensions()[0] == 2);
      REQUIRE(vector1.dimensions()[1] == 2);
      REQUIRE(vector1.size() == 4);

      vector2.resize({2, 2}, 1.0f);
      REQUIRE(vector2.dimensions()[0] == 2);
      REQUIRE(vector2.dimensions()[1] == 2);
      REQUIRE(vector2.size() == 4);
      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
          REQUIRE(vector2.at(x, y) == 1.0f);

      vector2.swap(vector1);
      REQUIRE(vector2.dimensions()[0] == 2);
      REQUIRE(vector2.dimensions()[1] == 2);
      REQUIRE(vector2.size() == 4);
      for (std::size_t x = 0; x < 2; ++x)
        for (std::size_t y = 0; y < 2; ++y)
          REQUIRE(vector2.at(x, y) == 0.0f);
    }

    // Non-member function tests.
    {
      vector_type vector1 {{2, 2}, {0.0f, 1.0f, 2.0f, 3.0f}};
      vector_type vector2 {{2, 2}, {0.0f, 1.0f, 2.0f, 3.0f}};
      vector_type vector3 {{2, 2}, {4.0f, 5.0f, 6.0f, 7.0f}};

      REQUIRE(vector1 == vector2);
      REQUIRE(vector1 != vector3);
      REQUIRE(vector1 <  vector3);
      REQUIRE(vector1 <= vector3);
      REQUIRE(vector3 >  vector1);
      REQUIRE(vector3 >= vector1);

      multi::swap(vector3, vector1);
      for (std::size_t i = 0; i < 4; ++i)
        REQUIRE(vector3.at(i) == static_cast<float>(i));
    }
  }
}