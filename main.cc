#include <iostream>
#include <vector>

//#define DEBUG

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

    void setPrev(expr* Prev)
    {
        m_prev = Prev;
    }

    void setChild(expr* Child)
    {
        m_child = Child;
        if (Child != nullptr) {
            Child->setPrev(this);
            Child->prev_att = "C";
        }
    }

    void setSibling(expr* Sibling)
    {
        m_sibling = Sibling;
        if (Sibling != nullptr) {
            Sibling->setPrev(this);
            Sibling->prev_att = "S";
        }
    }

    void find_position()
    {
        expr* prev = getPrev();
        if (prev != nullptr) {
            if (prev_att == "C") {
                m_level = prev->m_level + 1;
                space(prev->m_level + 1);
            } else {
                m_level = prev->m_level;
                space(prev->m_level);
            }
        }
    }

    expr* m_prev;
    expr* m_sibling;
    expr* m_child;
    int m_level;
    std::string prev_att = "";
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
        find_position();

        std::cout << m_stmt;
        std::cout << "\n";
        if (m_child != nullptr)
            m_child->gen();
#ifdef DEBUG
        std::cout << " // address : " << this << ", prev_address : " << getPrev() << "\n";
#endif
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
        m_level = level;
    }

    void gen()
    {
        find_position();

        std::string p_name = child_name;
        std::cout << "int " << p_name << " = " << from_loop;
        std::cout << ";\n";
        if (m_child != nullptr)
            m_child->gen();
#ifdef DEBUG
        std::cout << " // address : " << this << ", prev_address : " << getPrev() << "\n";
#endif
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
        index_ptr = index;
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

    void setBound(int value)
    {
        m_bound = value;
    }

    void setA(int value)
    {
        a = value;
    }

    void setSplit(bool value)
    {
        isSplit = value;
    }

    index* getIndex()
    {
        return index_ptr;
    }

    std::string getChildName()
    {
        return child_name;
    }

    int getIntChild()
    {
        return intChild;
    }

    int getEnd()
    {
        return m_end;
    }

    void gen()
    {
        find_position();

        std::string v_name = child_name + std::to_string(intChild);
        std::string p_name = child_name;

        if (index_ptr->from_loop == "")
            index_ptr->from_loop = std::to_string(a) + " * " + v_name + " + " + std::to_string(b);
        else
            index_ptr->from_loop += " + " + std::to_string(a) + " * " + v_name + " + " + std::to_string(b);

        if (!isSplit)
            std::cout << "for (int " << v_name << " = " << m_start << "; " << v_name << " < " << m_end << "; " << v_name << " += " << m_step << ") {\n";
        else {
            std::string p_name = child_name + std::to_string(intChild - 1); // parent name
            std::cout << "for (int " << v_name << " = " << p_name << "; " << v_name << " < std::min(" << p_name << " + " << m_end << ", " << m_bound << "); " << v_name << " += " << m_step << ") {\n";
        }
#ifdef DEBUG
        std::cout << " // address : " << this << ", prev_address : " << getPrev() << "\n";
#endif
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
    int m_bound;
    std::string child_name;

    index* index_ptr;
    std::string ax_b;
    int a = 1, x, b = 0;
    bool isSplit = false;
};

class function : public expr {
public:
    function(std::string s, int level = 0)
        : m_name(s)
    {
        m_name = s;
    }

    void gen()
    {
        find_position();
        std::cout << m_name;
        std::cout << " {\n";
        if (m_child != nullptr)
            m_child->gen();
#ifdef DEBUG
        std::cout << " // address : " << this << ", prev_address : " << getPrev() << "\n";
#endif
        std::cout << "}\n";
        if (m_sibling != nullptr)
            m_sibling->gen();
    }

private:
    std::string m_name;
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

    stmt* create_stmt(std::string str, int level = 0)
    {
        stmt* s = new stmt(str, level);
        return s;
    }

private:
    expr* m_start;
};

for_loop* split(for_loop* target, int size, int index = 0)
{
    target->setStep(size);
    for_loop* loop = new for_loop(target->getChildName(), 0, size, target->getIntChild() + 1, 1,
        target->level() + 1);
    loop->setSplit(true);
    loop->setIndex(target->getIndex());
    loop->setChild(target->getIndex());
    loop->setBound(target->getEnd());
    target->setChild(loop);
    target->setA(0);

    return loop;
}

// insert i to j below
void insert(index* i, index* j)
{
    expr* i_prev = i->getPrev();
    expr* j_prev = j->getPrev();

    std::string i_prev_att = i->prev_att;
    std::string j_prev_att = j->prev_att;

    expr* i_sb = i->getSibling();
    if (i_prev_att == "C")
        i_prev->setChild(i_sb);
    if (i_prev_att == "S")
        i_prev->setSibling(i_sb);

    i->setSibling(j);

    if (j_prev_att == "C")
        j_prev->setChild(i);
    if (j_prev_att == "S")
        j_prev->setSibling(i);

    i->m_level = j->m_level;
}
void swap(expr* i, expr* j)
{
    std::cout << " // i : " << i << ", j : " << j << "\n";

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

    if (i_prev != nullptr)
        i_prev->setChild(j);
    if (j_prev != nullptr)
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

void schedule(std::vector<expr*> sh)
{
    for (auto i = 0; i < sh.size() - 1; i++) {
        sh[i]->setChild(sh[i + 1]);
    }
}

int main()
{
    ast AST;
    function* func = new function("void matmul(float A, float B, float C)");
    for_loop* i = AST.create_in_loop("i", 0, 128);
    for_loop* j = AST.create_in_loop("j", 0, 128);
    for_loop* k = AST.create_in_loop("k", 0, 128);
    stmt* s = AST.create_stmt("C[i * 128 + j] += A[i * 128 + k] * B[k * 128 + j];");
    i->getIndex()->setSibling(j);
    j->getIndex()->setSibling(k);

    auto i1 = split(i, 16);
    auto j1 = split(j, 16);
    auto k1 = split(k, 16);

    insert(k->getIndex(), j->getIndex());
    insert(i->getIndex(), j->getIndex());
    j->getIndex()->setSibling(s);
    schedule({ i, j, k, k1, i1, j1 });

    func->setChild(i);
    func->gen();
    return 0;
}
