#ifndef __NONCOPYABLE_H__
#define __NONCOPYABLE_H__

class NonCopyable
{
protected:

	NonCopyable(){}

private:

	NonCopyable(const NonCopyable& other);

	NonCopyable& operator=(const NonCopyable& other);
};

#endif // __NONCOPYABLE_H__
