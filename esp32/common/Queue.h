#ifndef __QUEUE_H
#define __QUEUE_H

#include <Arduino.h>

template <class T>
class Queue
{
  private:
    int _front, _back, _count;
    int _rfront, _rback, _rcount;
    T *_data;
    int _maxitems;

  public:
    Queue(int maxitems = 256)
    {
        _front = 0;
        _back = 0;
        _count = 0;
        _rfront = 0;
        _rback = 0;
        _rcount = 0;
        _maxitems = maxitems;
        _data = new T[maxitems + 1];
    }
    ~Queue()
    {
        delete[] _data;
    }
    inline int count();
    inline int front();
    inline int back();
    void push(const T &item);
    T peek();
    T pop();
    void clear();
    void setResetPoint();
    void reset();
};

template <class T>
inline int Queue<T>::count()
{
    return _count;
}

template <class T>
inline int Queue<T>::front()
{
    return _front;
}

template <class T>
inline int Queue<T>::back()
{
    return _back;
}

template <class T>
void Queue<T>::push(const T &item)
{
    if (_count < _maxitems)
    { // Drops out when full
        _data[_back++] = item;
        ++_count;
        // Check wrap around
        if (_back > _maxitems)
            _back -= (_maxitems + 1);
    }
}

template <class T>
T Queue<T>::pop()
{
    if (_count <= 0)
        return T(); // Returns empty
    else
    {
        T result = _data[_front];
        _front++;
        --_count;
        // Check wrap around
        if (_front > _maxitems)
            _front -= (_maxitems + 1);
        return result;
    }
}

template <class T>
T Queue<T>::peek()
{
    if (_count <= 0)
        return T(); // Returns empty
    else
        return _data[_front];
}

template <class T>
void Queue<T>::clear()
{
    _front = _back;
    _count = 0;
}

template <class T>
void Queue<T>::setResetPoint()
{
    _rfront = _front;
    _rback = _back;
    _rcount = _count;
}

template <class T>
void Queue<T>::reset()
{
    _front = _rfront;
    _back = _rback;
    _count = _rcount;
}

#endif