#ifndef __VECTOR_H__
#define __VECTOR_H__
#define INIT_SIZE 4
typedef unsigned int uint;

template <typename T>
class Vector
{
    public:
        Vector() : _begin(nullptr), _capacity(INIT_SIZE), _size(0)
        {
            _begin = new T[_capacity];
        }

        ~Vector()
        {
            delete[] _begin;
        }

        void push_back(T val)
        {
            if (_size>=_capacity)
            {
                reserve(_capacity*2);
            }
            *(_begin + (_size++)) = val;
        }


        T * begin() const
        {
            return _begin;
        }

        T * end() const
        {
            return _begin+_size;
        }

        void insert_back(T * start, T * end)
        {
            while(start!=end)
            {
                push_back(*(start++));
                
            }
        }

        T operator[](int index){ return _begin[index];}

        int size(void){return _size;}
    private:

        void reserve(int new_capacity)
        {
            if(new_capacity<_capacity) return;
            T * tmp_begin = new T[new_capacity];

            for(int ptr=0; ptr<=_size; ptr++) tmp_begin[ptr] = _begin[ptr];
            
            delete[] _begin;
            _begin = tmp_begin;
            _capacity = new_capacity;
        }


        T * _begin;
        uint _capacity;
        uint _size;
};


#endif //__VECTOR_H__