#include <iostream>
#include <string>
#include <utility>

namespace maslevtsov {
  template< class T, class Cmp >
  struct BiTree
  {
    T data_;
    Cmp cmp_;
    BiTree< T, Cmp >* left_;
    BiTree< T, Cmp >* right_;
    BiTree< T, Cmp >* parent_;
  };

  template< class T, class Cmp >
  void clear(BiTree< T, Cmp >* root) noexcept;

  template< class T, class Cmp >
  void insert(BiTree< T, Cmp >* root, BiTree< T, Cmp >* node, Cmp cmp) noexcept;

  template< class T, class Cmp >
  BiTree< T, Cmp >* convert(const T* data, size_t s, Cmp cmp);

  template< class T, class Cmp >
  BiTree< T, Cmp >* find(BiTree< T, Cmp >* root, const T& value, Cmp cmp) noexcept;

  template< class T, class Cmp >
  BiTree< T, Cmp >* print(const BiTree< T, Cmp >* root) noexcept;

  template< class T, class Cmp >
  BiTree< T, Cmp >* extract(BiTree< T, Cmp >* root, const T& value, BiTree< T, Cmp >** result);
}

int main()
{
  std::size_t seq_size = 0;
  std::cin >> seq_size;
  int* seq = nullptr;
  try {
    seq = new int[seq_size];
  } catch (const std::bad_alloc&) {
    std::cerr << "<MEMORY ALLOCATION ERROR>\n";
    return 1;
  }
  std::size_t i = 0;
  while (std::cin && i != seq_size) {
    std::cin >> seq[i++];
  }
  if (!std::cin) {
    delete[] seq;
    std::cerr << "<INVALID SEQUENCE>\n";
    return 1;
  }
  maslevtsov::BiTree< int, std::less< int > >* root = nullptr;
  try {
    root = maslevtsov::convert(seq, seq_size, std::less< int >());
  } catch (const std::bad_alloc&) {
    delete[] seq;
    std::cerr << "<CONVERTATION ERROR\n>";
    return 1;
  }

  int num = 0;
  while (std::cin >> num) {
    if (!std::cin) {
      delete[] seq;
      clear(root);
      std::cerr << "<INVALID INPUT>\n";
      return 1;
    }
  }

  delete[] seq;
  clear(root);
}

template< class T, class Cmp >
void maslevtsov::clear(BiTree< T, Cmp >* root) noexcept
{
  if (!root) {
    return;
  }
  clear(root->left_);
  clear(root->right_);
  delete root;
}

template< class T, class Cmp >
void maslevtsov::insert(BiTree< T, Cmp >* root, BiTree< T, Cmp >* node, Cmp cmp) noexcept
{
  BiTree< T, Cmp >* sub_root = root;
  while (sub_root) {
    if (cmp(sub_root->data_, node->data_)) {
      if (!sub_root->right_) {
        sub_root->right_ = node;
        break;
      }
      sub_root = sub_root->right_;
    } else {
      if (!sub_root->left_) {
        sub_root->left_ = node;
        break;
      }
      sub_root = sub_root->left_;
    }
  }
  node->parent_ = sub_root;
}

template< class T, class Cmp >
maslevtsov::BiTree< T, Cmp >* maslevtsov::convert(const T* data, std::size_t s, Cmp cmp)
{
  if (!data || s == 0) {
    return nullptr;
  }
  auto root = new BiTree< T, Cmp >{data[0], std::less< int >(), nullptr, nullptr, nullptr};
  try {
    for (std::size_t i = 1; i != s; ++i) {
      auto node = new BiTree< T, Cmp >{data[i], std::less< int >(), nullptr, nullptr, nullptr};
      insert(root, node, cmp);
    }
  } catch (const std::bad_alloc&) {
    clear(root);
    throw;
  }
  return root;
}

template< class T, class Cmp >
maslevtsov::BiTree< T, Cmp >* maslevtsov::find(BiTree< T, Cmp >* root, const T& value, Cmp cmp) noexcept
{
  BiTree< T, Cmp >* result = root;
  while (result && result->data_ != value) {
    if (cmp(result->data_, value)) {
      result = result->right_;
    } else {
      result = result->left_;
    }
  }
  return result;
}
