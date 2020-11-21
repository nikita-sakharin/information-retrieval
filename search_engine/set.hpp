#ifndef SET_HPP
#define SET_HPP

#include <functional>

template <class Key, class Compare = std::less<Key>>
class set final
{
    struct node final
    {
        node *parent, *left, *right;
        enum : bool
        {
            black = false,
            read = true
        } color;
        Key data;

        inline node() noexcept = default;
        inline constexpr node(const node &) noexcept = default;
        inline constexpr node(node &&) noexcept = default;
        inline constexpr node &operator=(const node &) noexcept = default;
        inline constexpr node &operator=(node &&) noexcept = default;
        inline ~node() noexcept = default;
    };

    node *root;
    std::size_t count;
    Compare comp;

public:
    class iterator : std::iterator<std::forward_iterator_tag, Key>
    {
        node *current;
    public:
        inline constexpr iterator() noexcept : current(nullptr) {}
        inline constexpr iterator(node * const rhs_current) noexcept :
            current(rhs_current) {}
        inline constexpr iterator(const iterator &) noexcept = default;
        inline constexpr iterator(const iterator &&) noexcept = default;
        inline constexpr iterator &operator=(const iterator &) noexcept =
            default;
        inline constexpr iterator &operator=(const iterator &&) noexcept =
            default;
        inline ~iterator() noexcept = default;

        inline constexpr iterator operator++(int) noexcept
        {
            const iterator temp(*this);
            operator++();
            return temp;
        }

        constexpr iterator &operator++() noexcept
        {
            if (current->right)
            {
                current = current->right;
                while (current->left)
                    current = current->left;
                return *this;
            }

            while (current->parent && current->parent->right == current)
                current = current->parent;
            current = current->parent;
            return *this;
        }

        inline constexpr bool operator==(const iterator &rhs) const noexcept
        {
            return current == rhs.current;
        }

        inline constexpr bool operator!=(const iterator &rhs) const noexcept
        {
            return !operator==(rhs);
        }
    };

    inline constexpr set() noexcept : set(Compare()) {}

    inline constexpr set(const Compare &compare) noexcept :
        root(nullptr), count(0), comp(compare) {}

    inline constexpr set(const set &) noexcept = delete;
    inline constexpr set(set &&rhs) noexcept : set()
    {
        operator=(rhs);
    }

    inline constexpr set &operator=(const set &) noexcept = delete;
    inline constexpr set &operator=(set &&rhs) noexcept
    {
        clear();
        std::swap(root, rhs.root);
        std::swap(count, rhs.count);
        std::swap(comp, rhs.comp);
        return *this;
    }

    inline ~set() noexcept
    {
        clear();
    }

    constexpr iterator begin() noexcept
    {
        const node *current = root;
        while (current->left)
            current = current->left;
        return iterator(current);
    }

    constexpr void clear() noexcept
    {
        for (node *current = root, *parent; current; current = parent)
        {
            while (current->left || current->right)
                if (current->left)
                    current = current->left;
                else
                    current = current->right;

            if (parent = current->parent; parent)
            {
                if (parent->left == current)
                    parent->left = nullptr;
                else
                    parent->right = nullptr;
            }
            delete current;
        }
        root = nullptr;
        count = 0;
    }

    void clear() noexcept
    {
        for (node *current, *parent = root; parent;
            parent = current->parent, delete current)
        {
            if (parent->left == current && parent->right)
                current = parent->right;
            else
            {
                current = parent;
                continue;
            }
            while (true)
                if (current->left)
                    current = current->left;
                else if (current->right)
                    current = current->right;
                else
                    break;
        }
        root = nullptr;
        count = 0;
    }

    inline constexpr size_t empty() const noexcept
    {
        return size() == 0;
    }

    inline constexpr iterator end() const noexcept
    {
        return iterator();
    }

    inline constexpr size_t size() const noexcept
    {
        return count;
    }
};
/*
int rb_tree_insert(RBTree * restrict tree, const void * restrict value)
{
#   ifdef _RB_TREE_H_SAVE_
    if (tree == NULL || value == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    register RBTreeNode *node_ptr = tree->root, **link_ptr = &tree->root, *parent_ptr = NULL;
    while (node_ptr != NULL)
    {
        register int cmp_result = tree->cmp(value, node_ptr->data);
        parent_ptr = node_ptr;
        if (cmp_result < 0)
        {
            link_ptr = &node_ptr->left;
            node_ptr = node_ptr->left;
        }
        else if (cmp_result > 0)
        {
            link_ptr = &node_ptr->right;
            node_ptr = node_ptr->right;
        }
        else
        {
            return RB_TREE_ALREADY_EXIST;
        }
    }

    node_ptr = aligned_alloc(alignof(RBTreeNode), sizeof(RBTreeNode) + tree->size);
    if (node_ptr == NULL)
    {
        return RB_TREE_NO_MEMORY;
    }

    node_ptr->parent = parent_ptr;
    node_ptr->left = node_ptr->right = NULL;
    node_ptr->color = RED;
    memcpy(node_ptr->data, value, tree->size);

    *link_ptr = node_ptr;
    ++tree->count;

    rebalance_insert(node_ptr, &tree->root);

    return RB_TREE_SUCCESS;
}

int rb_tree_find(const RBTree * restrict tree, const void *value, void *buffer)
{
#   ifdef _RB_TREE_H_SAVE_
    if (tree == NULL || value == NULL || buffer == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    for (register const RBTreeNode *node_ptr = tree->root; node_ptr != NULL;)
    {
        register int cmp_result = tree->cmp(value, node_ptr->data);
        if (cmp_result < 0)
        {
            node_ptr = node_ptr->left;
        }
        else if (cmp_result > 0)
        {
            node_ptr = node_ptr->right;
        }
        else
        {
            memcpy(buffer, node_ptr->data, tree->size);

            return RB_TREE_SUCCESS;
        }
    }

    return RB_TREE_NOT_EXIST;
}

const void *rb_tree_at(const RBTree * restrict tree, const void * restrict value)
{
#   ifdef _RB_TREE_H_SAVE_
    if (tree == NULL || value == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    for (register const RBTreeNode *node_ptr = tree->root; node_ptr != NULL;)
    {
        register int cmp_result = tree->cmp(value, node_ptr->data);
        if (cmp_result < 0)
        {
            node_ptr = node_ptr->left;
        }
        else if (cmp_result > 0)
        {
            node_ptr = node_ptr->right;
        }
        else
        {
            return node_ptr->data;
        }
    }

    return NULL;
}

int rb_tree_erase(RBTree * restrict tree, const void * restrict value)
{
#   ifdef _RB_TREE_H_SAVE_
    if (tree == NULL || value == NULL)
    {
        exit(EXIT_FAILURE);
    }
#   endif

    register RBTreeNode *node_ptr = tree->root, *parent_ptr = NULL, **link_ptr = &tree->root;
    while (node_ptr != NULL)
    {
        register int cmp_result = tree->cmp(value, node_ptr->data);
        if (cmp_result < 0)
        {
            parent_ptr = node_ptr;
            link_ptr = &node_ptr->left;
            node_ptr = node_ptr->left;
        }
        else if (cmp_result > 0)
        {
            parent_ptr = node_ptr;
            link_ptr = &node_ptr->right;
            node_ptr = node_ptr->right;
        }
        else
        {
            break;
        }
    }

    if (node_ptr == NULL)
    {
        return RB_TREE_NOT_EXIST;
    }

    if (tree->destructor != NULL)
    {
        tree->destructor(node_ptr->data);
    }
    if (node_ptr->left != NULL && node_ptr->right != NULL)
    {
        void *data = node_ptr->data;

        parent_ptr = node_ptr;
        link_ptr = &node_ptr->right;
        node_ptr = node_ptr->right;
        while (node_ptr->left != NULL)
        {
            parent_ptr = node_ptr;
            link_ptr = &node_ptr->left;
            node_ptr = node_ptr->left;
        }

        *link_ptr = node_ptr->right;
        if (node_ptr->right != NULL)
        {
            node_ptr->right->parent = parent_ptr;
        }
        memcpy(data, node_ptr->data, tree->size);
    }
    else
    {
        if (node_ptr->left != NULL)
        {
            *link_ptr = node_ptr->left;
            node_ptr->left->parent = parent_ptr;
        }
        else if (node_ptr->right != NULL)
        {
            *link_ptr = node_ptr->right;
            node_ptr->right->parent = parent_ptr;
        }
        else
        {
            *link_ptr = NULL;
        }
    }

    if (node_ptr->color == BLACK)
    {
        rebalance_delete(*link_ptr, parent_ptr, &tree->root);
    }

    free(node_ptr);
    --tree->count;

    return RB_TREE_SUCCESS;
}

static RBTreeNode *sibling(const RBTreeNode *, const RBTreeNode *);
static RBTreeNode *grandparent(const RBTreeNode *);
static RBTreeNode **link(const RBTreeNode *, RBTreeNode **);

static void rotate_left(RBTreeNode *, RBTreeNode **);
static void rotate_right(RBTreeNode *, RBTreeNode **);

inline static void rebalance_insert(register RBTreeNode * restrict node_ptr, register RBTreeNode ** restrict root)
{
    while (node_ptr->parent != NULL && node_ptr->parent->color == RED)
    {
        register RBTreeNode *parent_ptr = node_ptr->parent;
        register RBTreeNode *uncle = sibling(parent_ptr, parent_ptr->parent), *grand = grandparent(node_ptr);
        if (uncle != NULL && uncle->color == RED)
        {
            parent_ptr->color = uncle->color = BLACK;
            grand->color = RED;
            node_ptr = grand;
        }
        else
        {
            if (node_ptr == parent_ptr->right && parent_ptr == grand->left)
            {
                rotate_left(parent_ptr, &grand->left);
                node_ptr = parent_ptr;
                parent_ptr = node_ptr->parent;
            }
            else if (node_ptr == parent_ptr->left && parent_ptr == grand->right)
            {
                rotate_right(parent_ptr, &grand->right);
                node_ptr = parent_ptr;
                parent_ptr = node_ptr->parent;
            }

            parent_ptr->color = BLACK;
            grand->color = RED;
            register RBTreeNode **link_ptr = link(grand, root);
            if (node_ptr == parent_ptr->right && parent_ptr == grand->right)
            {
                rotate_left(grand, link_ptr);
            }
            else
            {
                rotate_right(grand, link_ptr);
            }
        }
    }
    (*root)->color = BLACK;
}

inline static void rebalance_delete(register RBTreeNode * restrict node_ptr, register RBTreeNode * restrict parent_ptr, register RBTreeNode ** restrict root)
{
    while (parent_ptr != NULL && (node_ptr == NULL || node_ptr->color == BLACK))
    {
        register RBTreeNode *sib = sibling(node_ptr, parent_ptr);
        if (sib->color == RED)
        {
            sib->color = BLACK;
            parent_ptr->color = RED;
            register RBTreeNode **link_ptr = link(parent_ptr, root);
            if (node_ptr == parent_ptr->left)
            {
                rotate_left(parent_ptr, link_ptr);
            }
            else
            {
                rotate_right(parent_ptr, link_ptr);
            }
        }
        else
        {
            if ((sib->left == NULL || sib->left->color == BLACK) && (sib->right == NULL || sib->right->color == BLACK))
            {
                sib->color = RED;
                node_ptr = parent_ptr;
                parent_ptr = node_ptr->parent;
            }
            else if (sib == parent_ptr->left && (sib->left == NULL || sib->left->color == BLACK))
            {
                sib->color = RED;
                sib->right->color = BLACK;
                rotate_left(sib, link(sib, root));
            }
            else if (sib == parent_ptr->right && (sib->right == NULL || sib->right->color == BLACK))
            {
                sib->color = RED;
                sib->left->color = BLACK;
                rotate_right(sib, link(sib, root));
            }
            else
            {
                sib->color = parent_ptr->color;
                parent_ptr->color = BLACK;
                register RBTreeNode **link_ptr = link(parent_ptr, root);
                if (sib == parent_ptr->right)
                {
                    sib->right->color = BLACK;
                    rotate_left(parent_ptr, link_ptr);
                }
                else
                {
                    sib->left->color = BLACK;
                    rotate_right(parent_ptr, link_ptr);
                }
                break;
            }
        }
    }

    if (node_ptr != NULL)
    {
        node_ptr->color = BLACK;
    }
}

inline static RBTreeNode *sibling(register const RBTreeNode * restrict node_ptr, register const RBTreeNode * restrict parent_ptr)
{
    if (node_ptr == parent_ptr->left)
    {
        return parent_ptr->right;
    }
    else
    {
        return parent_ptr->left;
    }
}

inline static RBTreeNode *grandparent(register const RBTreeNode * restrict node_ptr)
{
    return node_ptr->parent->parent;
}

inline static RBTreeNode **link(register const RBTreeNode * restrict node_ptr, register RBTreeNode ** restrict root)
{
    if (node_ptr->parent == NULL)
    {
        return root;
    }

    if (node_ptr == node_ptr->parent->left)
    {
        return &node_ptr->parent->left;
    }
    else
    {
        return &node_ptr->parent->right;
    }
}

inline static void rotate_left(register RBTreeNode * restrict node_ptr, register RBTreeNode ** restrict link_ptr)
{
    register RBTreeNode *temp = node_ptr->right;

    node_ptr->right = temp->left;
    if (temp->left != NULL)
    {
        temp->left->parent = node_ptr;
    }

    *link_ptr = temp;
    temp->parent = node_ptr->parent;

    node_ptr->parent = temp;
    temp->left = node_ptr;
}

inline static void rotate_right(register RBTreeNode * restrict node_ptr, register RBTreeNode ** restrict link_ptr)
{
    register RBTreeNode *temp = node_ptr->left;

    node_ptr->left = temp->right;
    if (temp->right != NULL)
    {
        temp->right->parent = node_ptr;
    }

    *link_ptr = temp;
    temp->parent = node_ptr->parent;

    node_ptr->parent = temp;
    temp->right = node_ptr;
}
*/
#endif
