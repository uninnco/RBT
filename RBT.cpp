#include <iostream>
#include <functional>
#include <cassert>

using namespace std;

template<class TKey,class TValue>
class RBT{
    /**
    * @brief 节点颜色
    */
    enum NodeColor{
        RED,    ///< 红节点
        BLACK   ///< 黑节点
    };
    /**
    * @brief 节点
    */
    struct Node{
        TKey key_;                ///< 键
        TValue value_;            ///< 值
        NodeColor color_;         ///< 节点颜色
        int degree_ = 1;          ///< 出度
        Node* pleft_ = nullptr;   ///< 左子节点
        Node* pright_ = nullptr;  ///< 右子节点
        Node(TKey const& key,TValue const& value,NodeColor color=RED)
        :key_(key),value_(value),color_(color){}
    };
public:
    /**
    * @brief 获取@c key的值
    */
    TValue& GetValue(TKey const& key){
        return GetValue(proot_, key);
    }
    void Insert(TKey const& key, TValue const& value){
        proot_ = Insert(proot_, key, value);
        proot_->color_ = BLACK;
    }
    void Remove(TKey const& key){

    }
    friend ostream& operator<< (ostream& os,RBT const& rbt){
        rbt.Traverse([&os](Node const* pnode){
            assert(nullptr != pnode);
            os << " Key:" << pnode->key_
               << " Value:" << pnode->value_
               << " Color:" << (pnode->color_ == RED?"RED":"BLACK")
               << endl;
        });
        return os;
    }
private:
    typedef function<void(Node const*)> Transvesal;
    void Traverse(Transvesal func)const{
        assert(nullptr != proot_);

        // 递归遍历函数
        Transvesal Traverse = [&](Node const* pnode){
            if(nullptr == pnode){
                return;
            }
            Traverse(pnode->pleft_);
            func(pnode);
            Traverse(pnode->pright_);
        };

        Traverse(proot_); 
    }
    Node* Insert(Node* pnode, TKey const& key, TValue const& value){
        if (nullptr == pnode) 
            return new Node(key, value);

        // 递归插入
        if (key < pnode->key_) pnode->pleft_ = Insert(pnode->pleft_, key, value);
        else if (key > pnode->key_) pnode->pright_ = Insert(pnode->pright_, key, value);
        else pnode->value_ = value;

        //平衡化操作
        if (IsRed(pnode->pright_) and !IsRed(pnode->pleft_)) 
            pnode = RotateLeft(pnode);
        if (IsRed(pnode->pright_) and IsRed(pnode->pleft_->pleft_)) 
            pnode = RotateRight(pnode);
        if (IsRed(pnode->pleft_) and IsRed(pnode->pright_)) 
            pnode = FlipColor(pnode);

        pnode->degree_ = Degree(pnode->pleft_) + Degree(pnode->pright_) + 1;
        return pnode;
    }
    Node* FlipColor(Node* pnode)const{
        assert(nullptr != pnode);
        cout<<"FlipColor"<<endl;
        pnode->pleft_->color_ = BLACK;
        pnode->pright_->color_ = BLACK;
        pnode->color_ = RED;
        return pnode;
    }
    bool IsRed(Node* pnode)const{
        return nullptr != pnode and RED == pnode->color_;
    }
    int Degree(Node* pnode)const{
        return nullptr != pnode?pnode->degree_:0;
    }
    TValue& GetValue(Node* pnode,TKey const& key){
        if (nullptr == pnode) 
            return TValue();
        if (pnode->key_ == key) 
            return pnode->value_;
        return pnode->key_ > key?GetValue(pnode->pleft_, key):GetValue(pnode->pright_,key);
    }
    //左旋转
    //   k1              k2
    //  /  \            /  \
    // A   k2  ---->   k1   C
    //     / \         / \
    //    B  C        A   B
    //
    Node* RotateLeft(Node* k1) {
        cout << "RotateLeft"<<endl;
        Node* k2 = k1->pright_;
        //将k2的左节点复制给h右节点
        k1->pright_ = k2->pleft_;
        //将h复制给k2右节点
        k2->pleft_ = k1;
        k2->color_ = k1->color_;
        k1->color_ = RED;
        return k2;
    }
    //右旋转
    //     k1              k2
    //    /  \            /  \
    //   k2   C   ---->  A   k1 
    //  /  \                 / \
    // A    B               A   C
    //
    Node* RotateRight(Node* k1){
        cout << "RotateRight" <<endl;
        Node* k2 = k1->pleft_;
        k1->pleft_ = k2->pright_;
        k2->pright_ = k1;
        k2->color_ = k1->color_;
        k1->color_ = RED;
        return k2;
    }

private:
    Node* proot_ = nullptr;     ///< 根节点指针
};

template<class TKey,class TValue>
void InsertTest(RBT<TKey,TValue> &rbt,TKey const& key,TValue const& value){
    cout << "Insert(" << key << ',' << value << ')' << endl;
    rbt.Insert(key,value);
    cout << rbt;
}
int main(){
    RBT<int,char> rbt;
    for(auto p:(pair<int,char>[]){{1,'A'},{2,'B'},{3,'C'},{4,'D'},{5,'E'}}){
        InsertTest(rbt,p.first,p.second);
    }
}
