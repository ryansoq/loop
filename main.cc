#include <iostream>

void space(int s)
{
    for (int i = 0; i < s; i++)
        std::cout << "    ";
}

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
    expr* getPrev()
    {
        return m_prev;
    }

    void setChild(expr* setChild)
    {
        m_child = setChild;
    }

    void setSibling(expr* setSibling)
    {
        m_sibling = setSibling;
    }

    void setPrev(expr* setPrev)
    {
        m_prev = setPrev;
    }

    expr* m_prev;
    expr* m_sibling;
    expr* m_child;
    int m_level;
};

class stmt : public expr {
public:
    stmt(std::string s, int level = 0)
        : m_stmt(s)
    {
        m_level = level;
    }

    void setStmt(std::string s)
    {
        m_stmt = s;
    }
    void gen()
    {
        space(m_level);
        std::cout << m_stmt;
        std::cout << "\n";
        if (m_child != nullptr)
            m_child->gen();
        // std::cout << " // address : " << this << "\n";
        if (m_sibling != nullptr)
            m_sibling->gen();
    }

private:
    std::string m_stmt;
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
        std::string p_name = child_name;
        space(m_level);
        std::cout << "int " << p_name << " = " << from_loop;
        std::cout << ";\n";
        if (m_child != nullptr)
            m_child->gen();

        // std::cout << " // address : " << this << "\n";
        if (m_sibling != nullptr)
            m_sibling->gen();
    }
    std::string from_loop;

private:
    int setChild;
    std::string child_name;
    std::string ax_b;
    int a = 1, x, b = 0;
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
    int level() const { return m_level; }

    void setIndex(index* index)
    {
        childdex_ptr = index;
    }

    void setStart(int value)
    {
        m_start = value;
    }

    void setEnd(int value)
    {
        m_end = value;
    }

    void setStep(int value)
    {
        m_step = value;
    }

    index* getIndex()
    {
        return childdex_ptr;
    }

    std::string getChildName()
    {
        return child_name;
    }

    int getIntChild()
    {
        return intChild;
    }

    void gen()
    {
        std::string v_name = child_name + std::to_string(intChild);
        std::string p_name = child_name;
        if (childdex_ptr->from_loop == "")
            childdex_ptr->from_loop = std::to_string(a) + " * " + v_name + " + " + std::to_string(b);
        else
            childdex_ptr->from_loop += " + " + std::to_string(a) + " * " + v_name + " + " + std::to_string(b);

        space(m_level);
        std::cout << "for (int " << v_name << " = " << m_start << "; " << v_name << " < " << m_end << "; " << v_name << " += " << m_step << ") {\n";
        // std::cout << " // address : " << this << "\n";
        if (m_child != nullptr)
            m_child->gen();
        space(m_level);
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
    std::string ax_b;
    int a = 1, x, b = 0;
};

class functopn : public expr {
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
        loop_index->setPrev(loop);
        return loop;
    }

    stmt* create_stmt(std::string str, int level = 0)
    {
        stmt* s = new stmt(str, level);
        return s;
    }

private:
    expr* m_start;
};

void split(for_loop* target, int size, int index = 0)
{
    target->setStep(size);
    for_loop* loop = new for_loop(target->getChildName(), 0, size, target->getIntChild() + 1, 1,
        target->level() + 1);
    loop->setIndex(target->getIndex());
    loop->setChild(target->getIndex());
    target->getIndex()->setPrev(loop);
    target->setChild(loop);
}

void swap(expr* i, expr* j)
{
    expr* i_prev = i->getPrev();
    expr* j_prev = j->getPrev();
    expr* i_ch = i->getChild();
    expr* i_sb = i->getSibling();
    expr* j_ch = j->getChild();
    expr* j_sb = j->getSibling();

    i->setChild(j_ch);
    i->setSibling(j_sb);
    j->setChild(i_ch);
    j->setSibling(i_sb);

    int level_temp = i->m_level;
    i->m_level = j->m_level;
    j->m_level = level_temp;

    i_prev->setChild(j);
    j_prev->setChild(i);
}

void swap_loop_index(for_loop* i, for_loop* j)
{
    expr* i_child = i->getIndex();
    expr* j_child = j->getIndex();
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
    stmt* s = AST.create_stmt("C[i,j] = A[i,k] * B[k,j];", 3);
    i->getChild()->setSibling(j);
    j->getChild()->setSibling(k);
    k->getIndex()->setSibling(s);
    // split(i, 16);
    // split(j, 16);
    // split(k, 16);
    // swap(i->getIndex(), k->getIndex());
    i->gen();
    return 0;
}
