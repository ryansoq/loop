#include <iostream>

class expr {
public:
    expr()
        : m_sibling(nullptr)
        , m_child(nullptr)
    {
    }
    virtual void gen() = 0; // Pure Virtual Fun
    expr* getChild()
    {
        return m_child;
    }
    expr* getSibling()
    {
        return m_sibling;
    }
    void setChild(expr* setChild)
    {
        m_child = setChild;
    }

    void setSibling(expr* setSibling)
    {
        m_sibling = setSibling;
    }

    expr* m_sibling;
    expr* m_child;
    int m_level;
};

class index : public expr {
public:
    index(std::string index_name, int level = 0, int index = 0)
        : child_name(index_name)
        , setChild(index)
        , expr()
    {
        // m_sibling = nullptr;
        m_level = level;
    }

    void setSibling(expr* setSibling)
    {
        m_sibling = setSibling;
    }

    void gen()
    {
        std::string v_name = child_name + std::to_string(setChild);
        std::string p_name = child_name;
        for (int i = 0; i < m_level; i++)
            std::cout << "    "; // space;
        std::cout << "int " << p_name << " = " << std::to_string(a) << " * " << v_name << " + " << std::to_string(b);
        std::cout << ";\n";
        // std::cout << " // address : " << this << "\n";
        if (m_sibling != nullptr)
            m_sibling->gen();
    }

private:
    int setChild;
    std::string child_name;
    std::string ax_b;
    int a = 1, x, b = 0;
    // expr* m_sibling;
};

class for_loop : public expr {
public:
    for_loop(std::string index_name, int start, int end, int index = 0, int step = 1, int level = 0)
        : child_name(index_name)
        , m_start(start)
        , m_end(end)
        , intChild(index)
        , m_step(step)
        , expr()
    {
        m_level = level;
    }

    int start() const { return m_start; }
    int end() const { return m_end; }
    int step() const { return m_step; }

    void setIndex(index* index)
    {
        childdex_ptr = index;
    }

    index* getIndex()
    {
        return childdex_ptr;
    }

    void gen()
    {
        std::string v_name = child_name + std::to_string(intChild);
        std::string p_name = child_name;
        for (int i = 0; i < m_level; i++)
            std::cout << "    "; // space;

        std::cout << "for (int " << v_name << " = " << m_start << "; " << v_name << " < " << m_end << "; " << v_name << " += " << m_step << ") {\n";
        // std::cout << " // address : " << this << "\n";
        if (m_child != nullptr)
            m_child->gen();

        for (int i = 0; i < m_level; i++)
            std::cout << "    "; // space;
        std::cout << "}\n";

        if (m_sibling != nullptr)
            m_sibling->gen();
    }

private:
    int m_start;
    int m_end;
    int m_step;
    int intChild;
    std::string child_name;

    index* childdex_ptr;
};

class ast : public expr {

public:
    ast()
        : m_start(nullptr)
        , expr()
    {
    }

    void gen()
    {
        if (m_start != nullptr)
            m_start->gen();
    }

    void swap(expr* x, expr* y)
    {
        expr* temp = x->getChild();
        x = y->getChild();
        y = temp;
    }

    for_loop* create_in_loop(std::string index_name, int start, int end, int level = 0)
    {
        for_loop* loop = new for_loop(index_name, start, end, 0, 1, level);
        index* loop_index = new index(index_name, level + 1);
        loop->setChild(loop_index);
        loop->setIndex(loop_index);
        return loop;
    }

private:
    expr* m_start;
};

void swap_loop_index(expr* i, expr* j)
{
    expr* i_child = i->getChild();
    expr* j_child = j->getChild();
    expr* i_child_ch = i_child->getChild();
    expr* i_child_sb = i_child->getSibling();
    expr* j_child_ch = j_child->getChild();
    expr* j_child_sb = j_child->getSibling();

    i_child->setChild(j_child_ch);
    i_child->setSibling(j_child_sb);
    j_child->setChild(i_child_ch);
    j_child->setSibling(i_child_sb);

    int level_temp = i_child->m_level;
    i_child->m_level = j_child->m_level;
    j_child->m_level = level_temp;

    i->setChild(j_child);
    j->setChild(i_child);
}

int main()
{
    ast AST;
    for_loop* i = AST.create_in_loop("i", 0, 128, 0);
    for_loop* j = AST.create_in_loop("j", 0, 128, 1);
    for_loop* k = AST.create_in_loop("k", 0, 128, 2);
    i->getChild()->setSibling(j);
    j->getChild()->setSibling(k);
    i->gen();
    return 0;
}
