#pragma once
// Implementation of the vector by creating a dynamic array.

template<class T>
class MyVector{
private:
    int vsize, vcapacity;
    T* ptr_array;
    void reAllocMem();

public:
     // Constructors
    MyVector(int num=2);
    MyVector(const MyVector& sourceVec);

    // Destructor
    ~MyVector();


     // Methods------------------------------------------------------
    void push_back(const T &elem);
    void add(const T &elem);    
    int size();
    void clear();
    int capacity();


    // Operators-----------------------------------------------------
    T& operator[] (int i)const;
    MyVector<T>& operator=(const MyVector& sourceVec);
};

template<class T>
MyVector<T>::MyVector(int num){
    vsize=0;
    vcapacity=num;
    ptr_array=new T[num];
}

template<class T>
MyVector<T>::MyVector(const MyVector& sourceVec){
    vsize=sourceVec.vsize;
    vcapacity=sourceVec.vcapacity;
    ptr_array=new T[vcapacity];
    for(int i=0;i<vsize;i++)
        ptr_array[i]=sourceVec.ptr_array[i];
}

template<class T>
MyVector<T>& MyVector<T>::operator=(const MyVector& sourceVec){
    if(this != &sourceVec){
        delete [] ptr_array;
    }
    vsize = sourceVec.vsize;
    ptr_array = new T[vsize];
    for(int i=0;i != vsize;++i){
        ptr_array[i] = sourceVec.ptr_array[i];
    }
    return *this;
}

template<class T>
MyVector<T>::~MyVector(){
    delete [] ptr_array;
}

template<class T>
void MyVector<T>::push_back(const T &elem){
    if(vsize==vcapacity)
        reAllocMem();
    ptr_array[vsize]=elem;
    vsize++;
}

template<class T>
T& MyVector<T>::operator[] (int i)const
{
    return ptr_array[i];
}

template<class T>
int MyVector<T>::size(){
    return vsize;
}

template<class T>
void MyVector<T>::add(const T &elem){push_back(elem);}

template<class T>
int MyVector<T>::capacity(){
    return vcapacity;
}

template<class T>
void MyVector<T>::reAllocMem(){
    vcapacity *= 2;
    T* ptr_tmp = new T[vcapacity];
    for(int i=0;i<vsize;i++)
        ptr_tmp[i]=ptr_array[i];
    delete[] ptr_array;
    ptr_array=ptr_tmp;
}

template <typename T>
void MyVector<T>::clear(){
    vsize = 0;
}
