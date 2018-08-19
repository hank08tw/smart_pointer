#include <iostream>
using namespace std;
template<class T>
class SmartPtr
{
public:
    SmartPtr(T *p);
     ~SmartPtr();
     SmartPtr(const SmartPtr<T> &orig);                // 浅拷贝
     SmartPtr<T>& operator=(const SmartPtr<T> &rhs);    // 浅拷贝
 private:
     T *ptr;
     // 将use_count声明成指针是为了方便对其的递增或递减操作
     int *use_count;
 };

 template<class T>
 SmartPtr<T>::SmartPtr(T *p)
 {
     try
     {
         this->ptr=p;
         use_count = new int(1);
     }
     catch (...)
     {
         delete ptr;
         ptr = nullptr;
         use_count = nullptr;
         cout << "Allocate memory for use_count fails." << endl;
         exit(1);
     }

     cout << "Constructor is called!" << endl;
 }

 template<class T>
 SmartPtr<T>::~SmartPtr()
 {
     // 只有當最後一個instance呼叫destructor時才把ptr跟use count都free掉
     if (--(*use_count) == 0)
     {
         delete ptr;
         delete use_count;
         ptr = nullptr;
         use_count = nullptr;
         cout << "Destructor is called!" << endl;
     }
 }

 template<class T>
 //copy constructor淺拷貝
 SmartPtr<T>::SmartPtr(const SmartPtr<T> &orig)
 {
     ptr = orig.ptr;
     use_count = orig.use_count;
     ++(*use_count);//多一個指向相同地方的指針
     cout << "Copy constructor is called!" << endl;
 }

 template<class T>
 SmartPtr<T>& SmartPtr<T>::operator=(const SmartPtr<T> &rhs)
 {
     /*
     //如果自己賦值給自己
     if(&rhs==this){
         cout << "Assignment operator overloaded (left right same) is called!" << endl;
         return *this;
     }
      */
     //右邊指針指向的地方多了一次
     ++(*rhs.use_count);

     // 左邊指針只的地方少了一次
     if (--(*use_count) == 0)
     {
         delete ptr;
         delete use_count;
         cout << "Left side object is deleted!" << endl;
     }

     ptr = rhs.ptr;
     use_count = rhs.use_count;

     cout << "Assignment operator overloaded is called!" << endl;
     return *this;
 }
int main() {
    /*
    //只允許一個指針擁有物件 不允許多個指針共享unique ptr
    unique_ptr<int> ptr(new int(0));
    //允許多個指針擁有指向相同物件
    shared_ptr<string> shared_ptr1(new string("theshared_ptr"),
                 [](string *p)
                 {
                         cout << "lambda override function: delete " << *p << endl;
                         delete p;
                     }
             );
    shared_ptr<int> shared_ptr2(new int[10],
                                   [](int *p)
                                   {
                                       cout << "lambda override function: delete " << *p << endl;
                                       delete[] p;
                                   }
    );
    */
    // Test Constructor and Assignment Operator Overloaded
    SmartPtr<int> p1(new int(0));
    p1 = p1;
    // Test Copy Constructor
    SmartPtr<int> p2(p1);
    // Test Assignment Operator Overloaded
    SmartPtr<int> p3(new int(1));
    p3 = p1;
    return 0;
}