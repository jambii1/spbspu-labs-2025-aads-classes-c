#include <iostream>
#include <functional>
#include <string>

namespace maslevtsov {
  template< class T >
  struct BiTree
  {
    T data_;
    BiTree< T >* left_;
    BiTree< T >* right_;
    BiTree< T >* parent_;
  };

  template< class T >
  void clear(BiTree< T >* root) noexcept;

  template< class T, class Cmp >
  void insert(BiTree< T >* root, BiTree< T >* node, Cmp cmp) noexcept;

  template< class T, class Cmp >
  BiTree< T >* convert(const T* data, size_t s, Cmp cmp);

  template< class T, class Cmp >
  BiTree< T >* find(BiTree< T >* root, const T& value, Cmp cmp) noexcept;

  template< class T >
  BiTree< T >* rotate_left(BiTree< T >* root);

  template< class T >
  BiTree< T >* rotate_right(BiTree< T >* root);
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
  maslevtsov::BiTree< int >* root = nullptr;
  try {
    root = maslevtsov::convert(seq, seq_size, std::less< int >());
  } catch (const std::bad_alloc&) {
    delete[] seq;
    std::cerr << "<CONVERTATION ERROR\n>";
    return 1;
  }

  std::string command = "";
  int num = 0;
  while (std::cin >> command >> num || !std::cin.eof()) {
    maslevtsov::BiTree< int >* sub_root = root;
    try {
      if (command == "right") {
        maslevtsov::rotate_right(find(sub_root, num, std::less< int >()));
      } else if (command == "left") {
        maslevtsov::rotate_left(find(sub_root, num, std::less< int >()));
      } else {
        throw std::invalid_argument("invalid command");
      }
    } catch (const std::invalid_argument&) {
      delete[] seq;
      clear(root);
      std::cerr << "<INVALID COMMAND>\n";
      return 1;
    } catch (const std::logic_error&) {
      delete[] seq;
      clear(root);
      std::cerr << "INVALID ROTATE\n";
      return 1;
    }
  }
  if (!std::cin.eof() && !std::cin) {
    delete[] seq;
    clear(root);
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }
}

template< class T >
void maslevtsov::clear(BiTree< T >* root) noexcept
{
  if (!root) {
    return;
  }
  clear(root->left_);
  clear(root->right_);
  delete root;
}

template< class T, class Cmp >
void maslevtsov::insert(BiTree< T >* root, BiTree< T >* node, Cmp cmp) noexcept
{
  BiTree< T >* sub_root = root;
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
maslevtsov::BiTree< T >* maslevtsov::convert(const T* data, std::size_t s, Cmp cmp)
{
  if (!data || s == 0) {
    return nullptr;
  }
  BiTree< T >* root = new BiTree< T >{data[0], nullptr, nullptr, nullptr};
  try {
    for (std::size_t i = 1; i != s; ++i) {
      BiTree< T >* node = new BiTree< T >{data[i], nullptr, nullptr, nullptr};
      insert(root, node, cmp);
    }
  } catch (const std::bad_alloc&) {
    clear(root);
    throw;
  }
  return root;
}

template< class T >
maslevtsov::BiTree< T >* maslevtsov::rotate_left(BiTree< T >* root)
{
  if (!root || !root->right_) {
    throw std::logic_error("unable to rotate");
  }
  BiTree< T >* sub_root = root;
  root = root->right_;
  if (root->left_) {
    root->left_->parent_ = sub_root;
    sub_root->right_ = root->left_;
  }
  root->left_ = sub_root;
  if (sub_root->parent_) {
    if (sub_root->parent_->left_ == sub_root) {
      sub_root->parent_->left_ = root;
    }
    if (sub_root->parent_->right_ == sub_root) {
      sub_root->parent_->right_ = root;
    }
    root->parent_ = sub_root->parent_;
  }
  sub_root->parent_ = root;
  return root;
}

template< class T >
maslevtsov::BiTree< T >* maslevtsov::rotate_right(BiTree< T >* root)
{
  if (!root || !root->left_) {
    throw std::logic_error("unable to rotate");
  }
  BiTree< T >* sub_root = root;
  root = root->left_;
  if (root->right_) {
    root->right_->parent_ = sub_root;
    sub_root->left_ = root->right_;
  }
  root->right_ = sub_root;
  if (sub_root->parent_) {
    if (sub_root->parent_->left_ == sub_root) {
      sub_root->parent_->left_ = root;
    }
    if (sub_root->parent_->right_ == sub_root) {
      sub_root->parent_->right_ = root;
    }
    root->parent_ = sub_root->parent_;
  }
  sub_root->parent_ = root;
  return root;
}

template< class T, class Cmp >
maslevtsov::BiTree< T >* maslevtsov::find(BiTree< T >* root, const T& value, Cmp cmp) noexcept
{
  BiTree< T >* result = root;
  while (result && result->data_ != value) {
    if (cmp(result->data_, value)) {
      result = result->right_;
    } else {
      result = result->left_;
    }
  }
  return result;
}
