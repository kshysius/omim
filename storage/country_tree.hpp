#pragma once

#include "base/assert.hpp"

#include "std/algorithm.hpp"
#include "std/unique_ptr.hpp"
#include "std/vector.hpp"

/// This class is developed for using in CountryTreeFacade. It's a implementation of a tree.
/// It should be filled with AddAtDepth method.
/// This class is used in filled based on countries.txt (countries_migrate.txt).
/// While filling CountryTree nodes in countries.txt should be visited in DFS order.
template <class T>
class CountryTree
{
  T m_value;

  /// \brief m_children contains all first generation descendants of the node.
  /// Note. Once created the order of elements of |m_children| should not be changed.
  /// See implementation of AddAtDepth and Add methods for details.
  vector<unique_ptr<CountryTree<T>>> m_children;
  CountryTree<T> * m_parent;

  /// @return reference is valid only up to the next tree structure modification
  CountryTree<T> * Add(T const & value)
  {
    m_children.emplace_back(make_unique<CountryTree<T>>(value, this));
    return m_children.back().get();
  }

public:
  CountryTree(T const & value = T(), CountryTree<T> * parent = nullptr)
    : m_value(value), m_parent(parent)
  {
  }

  /// @return reference is valid only up to the next tree structure modification
  T const & Value() const { return m_value; }

  /// @return reference is valid only up to the next tree structure modification
  T & Value() { return m_value; }

  /// @return reference is valid only up to the next tree structure modification
  CountryTree<T> * AddAtDepth(int level, T const & value)
  {
    CountryTree<T> * node = this;
    while (level-- > 0 && !node->m_children.empty())
      node = node->m_children.back().get();
    ASSERT_EQUAL(level, -1, ());
    return node->Add(value);
  }

  /// Deletes all children and makes tree empty
  void Clear() { m_children.clear(); }

  bool operator<(CountryTree<T> const & other) const { return Value() < other.Value(); }

  bool HasParent() const { return m_parent != nullptr; }

  CountryTree<T> const & Parent() const
  {
    CHECK(HasParent(), ());
    return *m_parent;
  }

  CountryTree<T> const & Child(size_t index) const
  {
    ASSERT_LESS(index, m_children.size(), ());
    return *m_children[index];
  }

  size_t ChildrenCount() const { return m_children.size(); }

  /// \brief Calls functor f for each first generation descendant of the node.
  template <class TFunctor>
  void ForEachChild(TFunctor && f)
  {
    for (auto & child : m_children)
      f(*child);
  }

  template <class TFunctor>
  void ForEachChild(TFunctor && f) const
  {
    for (auto const & child : m_children)
      f(*child);
  }

  /// \brief Calls functor f for all nodes (add descendant) in the tree.
  template <class TFunctor>
  void ForEachDescendant(TFunctor && f)
  {
    for (auto & child : m_children)
    {
      f(*child);
      child->ForEachDescendant(f);
    }
  }

  template <class TFunctor>
  void ForEachDescendant(TFunctor && f) const
  {
    for (auto const & child: m_children)
    {
      f(*child);
      child->ForEachDescendant(f);
    }
  }

  template <class TFunctor>
  void ForEachInSubtree(TFunctor && f)
  {
    f(*this);
    for (auto & child: m_children)
      child->ForEachInSubtree(f);
  }

  template <class TFunctor>
  void ForEachInSubtree(TFunctor && f) const
  {
    f(*this);
    for (auto const & child: m_children)
      child->ForEachInSubtree(f);
  }

  template <class TFunctor>
  void ForEachAncestorExceptForTheRoot(TFunctor && f)
  {
    if (m_parent == nullptr || m_parent->m_parent == nullptr)
      return;
    f(*m_parent);
    m_parent->ForEachAncestorExceptForTheRoot(f);
  }

  template <class TFunctor>
  void ForEachAncestorExceptForTheRoot(TFunctor && f) const
  {
    if (m_parent == nullptr || m_parent->m_parent == nullptr)
      return;
    f(*m_parent);
    m_parent->ForEachAncestorExceptForTheRoot(f);
  }
};
