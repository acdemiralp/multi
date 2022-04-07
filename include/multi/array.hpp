#pragma once

#include <array>
#include <cstddef>
#include <utility>

#include <multi/third_party/mdspan.hpp>

namespace multi
{
template <std::size_t... _sizes>
struct dimensions
{
  using extents_type = std::experimental::extents<_sizes...>;

  static constexpr std::array<std::size_t, sizeof...(_sizes)> sizes      ()
  {
    return {_sizes...};
  }
  static constexpr std::size_t                                linear_size() 
  {
    return (_sizes * ...);
  }
};

template <
  typename _type       ,
  typename _dimensions ,
  typename _layout     = std::experimental::layout_right,
  typename _accessor   = std::experimental::default_accessor<_type>>
class array
{
public:
  using dimensions_type        = _dimensions;
  using storage_type           = std::array<_type, dimensions_type::linear_size()>;
  using span_type              = std::experimental::mdspan<_type, typename dimensions_type::extents_type, _layout, _accessor>;

  using value_type             = typename storage_type::value_type;
  using size_type              = typename storage_type::size_type;
  using difference_type        = typename storage_type::difference_type;
  using reference              = typename storage_type::reference;
  using const_reference        = typename storage_type::const_reference;
  using pointer                = typename storage_type::pointer;
  using const_pointer          = typename storage_type::const_pointer;
  using iterator               = typename storage_type::iterator;
  using const_iterator         = typename storage_type::const_iterator;
  using reverse_iterator       = typename storage_type::reverse_iterator;
  using const_reverse_iterator = typename storage_type::const_reverse_iterator;
  
  using multi_size_type        = std::array<size_type, span_type::rank()>;

  constexpr  array() noexcept
  : span_(storage_.data())
  {

  }
  constexpr  array(const_reference value)
  : span_(storage_.data())
  {
    storage_.fill(value);
  }
  template <typename _input_iterator>
  constexpr  array(_input_iterator first, _input_iterator last)
  : span_(storage_.data())
  {
    std::copy(first, last, storage_.begin());
  }
  constexpr  array(std::initializer_list<_type> list)
  : span_(storage_.data())
  {
    std::copy(list.begin(), list.end(), storage_.begin());
  }
  constexpr  array(const array&  that)
  : storage_(that.storage_)           , span_(storage_.data())
  {

  }
  constexpr  array(      array&& temp) noexcept
  : storage_(std::move(temp.storage_)), span_(storage_.data())
  {

  }
  constexpr ~array() = default;

  constexpr array&                 operator=    (const array&  that)
  {
    if (this != &that)
    {
      storage_ = that.storage_;
      // Spans are not copied.
    }
    return *this;
  }
  constexpr array&                 operator=    (      array&& temp) noexcept
  {
    if (this != &temp)
    {
      storage_ = std::move(temp.storage_);
      // Spans are not moved. Nothing is invalidated.
    }
    return *this;
  }
  constexpr array&                 operator=    (std::initializer_list<_type> list)
  {
    storage_ = list;
    return *this;
  }

  // Element access.

  constexpr reference              at           (size_type              position)
  {
    return storage_.at(position);
  }
  constexpr const_reference        at           (size_type              position) const
  {
    return storage_.at(position);
  }
  constexpr reference              at           (const multi_size_type& position)
  {
    return span_(position);
  }
  constexpr const_reference        at           (const multi_size_type& position) const
  {
    return span_(position);
  }
  template <typename... _positions>
  constexpr reference              at           (_positions...          position)
  {
    return span_(position...);
  }
  template <typename... _positions>
  constexpr const_reference        at           (_positions...          position) const
  {
    return span_(position...);
  }
  
  constexpr reference              operator[]   (size_type              position)
  {
    return storage_[position];
  }
  constexpr const_reference        operator[]   (size_type              position) const
  {
    return storage_[position];
  }
  constexpr reference              operator[]   (const multi_size_type& position)
  {
    return span_(position);
  }
  constexpr const_reference        operator[]   (const multi_size_type& position) const
  {
    return span_(position);
  }
  
  constexpr reference              operator()   (const multi_size_type& position)
  {
    return span_(position);
  }
  constexpr const_reference        operator()   (const multi_size_type& position) const
  {
    return span_(position);
  }
  template <typename... _positions>
  constexpr reference              operator()   (_positions...          position)
  {
    return span_(position...);
  }
  template <typename... _positions>
  constexpr const_reference        operator()   (_positions...          position) const
  {
    return span_(position...);
  }

  constexpr reference              front        ()
  {
    return storage_.front();
  }
  constexpr const_reference        front        () const
  {
    return storage_.front();
  }

  constexpr reference              back         ()
  {
    return storage_.back();
  }
  constexpr const_reference        back         () const
  {
    return storage_.back();
  }

  constexpr pointer                data         () noexcept
  {
    return storage_.data();
  }
  constexpr const_pointer          data         () const noexcept
  {
    return storage_.data();
  }

  // Iterators.

  constexpr iterator               begin        () noexcept
  {
    return storage_.begin ();
  }
  constexpr const_iterator         begin        () const noexcept
  {
    return storage_.begin ();
  }
  constexpr const_iterator         cbegin       () const noexcept
  {
    return storage_.cbegin();
  }
  
  constexpr iterator               end          () noexcept
  {
    return storage_.end   ();
  }
  constexpr const_iterator         end          () const noexcept
  {
    return storage_.end   ();
  }
  constexpr const_iterator         cend         () const noexcept
  {
    return storage_.cend  ();
  }

  constexpr reverse_iterator       rbegin       () noexcept
  {
    return storage_.rbegin ();
  }
  constexpr const_reverse_iterator rbegin       () const noexcept
  {
    return storage_.rbegin ();
  }
  constexpr const_reverse_iterator crbegin      () const noexcept
  {
    return storage_.crbegin();
  }
  
  constexpr reverse_iterator       rend         () noexcept
  {
    return storage_.rend   ();
  }
  constexpr const_reverse_iterator rend         () const noexcept
  {
    return storage_.rend   ();
  }
  constexpr const_reverse_iterator crend        () const noexcept
  {
    return storage_.crend  ();
  }
  
  // Capacity.

  constexpr bool                   empty        () const noexcept
  {
    return storage_.empty   ();
  }
  constexpr size_type              size         () const noexcept
  {
    return storage_.size    ();
  }
  constexpr multi_size_type        dimensions   () const noexcept
  {
    return dimensions_type::sizes();
  }
  constexpr size_type              max_size     () const noexcept
  {
    return storage_.max_size();
  }
  
  // Operations.
  
  constexpr void                   fill         (const_reference value) noexcept
  {
    storage_.fill(value);
  }
  constexpr void                   swap         (array& that) noexcept
  {
    std::swap(storage_, that.storage_);
    // Spans are not swapped.
  }

  // Member access.

  constexpr const storage_type&    storage      () const noexcept
  {
    return storage_;
  }
  constexpr const span_type&       span         () const noexcept
  {
    return span_;
  }

protected:
  template <std::size_t _i, typename _t, typename _d, typename _l, typename _a>
  friend constexpr       _t&  get(      array<_t, _d, _l, _a>& ) noexcept;
  template <std::size_t _i, typename _t, typename _d, typename _l, typename _a>
  friend constexpr       _t&& get(      array<_t, _d, _l, _a>&&) noexcept;
  template <std::size_t _i, typename _t, typename _d, typename _l, typename _a>
  friend constexpr const _t&  get(const array<_t, _d, _l, _a>& ) noexcept;
  template <std::size_t _i, typename _t, typename _d, typename _l, typename _a>
  friend constexpr const _t&& get(const array<_t, _d, _l, _a>&&) noexcept;

  storage_type storage_;
  span_type    span_   ;
};

// Non-member functions (no correspondent for to_array, no multidimensional get).

template <typename _type, typename _dimensions, typename _layout, typename _accessor>
constexpr bool operator== (
  const array<_type, _dimensions, _layout, _accessor>& lhs, 
  const array<_type, _dimensions, _layout, _accessor>& rhs)
{
  return lhs.storage() == rhs.storage() && lhs.span().mapping() == rhs.span().mapping();
}
template <typename _type, typename _dimensions, typename _layout, typename _accessor>
constexpr auto operator<=>(
  const array<_type, _dimensions, _layout, _accessor>& lhs, 
  const array<_type, _dimensions, _layout, _accessor>& rhs)
{
  return lhs.storage() <=> rhs.storage();
}

template <std::size_t _index, typename _type, typename _dimensions, typename _layout, typename _accessor>
constexpr       _type&  get(      array<_type, _dimensions, _layout, _accessor>&  container) noexcept
{
  return container.storage_[_index];
}
template <std::size_t _index, typename _type, typename _dimensions, typename _layout, typename _accessor>
constexpr       _type&& get(      array<_type, _dimensions, _layout, _accessor>&& container) noexcept
{
  return std::move(container.storage_[_index]);
}
template <std::size_t _index, typename _type, typename _dimensions, typename _layout, typename _accessor>
constexpr const _type&  get(const array<_type, _dimensions, _layout, _accessor>&  container) noexcept
{
  return container.storage_[_index];
}
template <std::size_t _index, typename _type, typename _dimensions, typename _layout, typename _accessor>
constexpr const _type&& get(const array<_type, _dimensions, _layout, _accessor>&& container) noexcept
{
  return std::move(container.storage_[_index]);
}

template <typename _type, typename _dimensions, typename _layout, typename _accessor>
constexpr void swap(
  array<_type, _dimensions, _layout, _accessor>& lhs, 
  array<_type, _dimensions, _layout, _accessor>& rhs) noexcept
{
  lhs.swap(rhs);
}

// Helper classes.

template <typename _type>
struct tuple_size;
template <typename _type, typename _dimensions, typename _layout, typename _accessor>
struct tuple_size<array<_type, _dimensions, _layout, _accessor>> : std::integral_constant<std::size_t, _dimensions::linear_size()>
{

};

template <std::size_t _index, typename _type>
struct tuple_element;
template <std::size_t _index, typename _type, typename _dimensions, typename _layout, typename _accessor>
struct tuple_element<_index, array<_type, _dimensions, _layout, _accessor>>
{
  using type = _type;
};
}