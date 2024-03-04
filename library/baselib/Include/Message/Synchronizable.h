
/*!
    \file Synchronizable.h
    \brief 可同步接口定义
    \details 无
    \author
*/

#ifndef __BASE_LIB__MSG_FRW__SYNCHRONIZABLE_H__
#define __BASE_LIB__MSG_FRW__SYNCHRONIZABLE_H__

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

namespace baselib {

/*!
    \brief     同步接口
    \details   所有同步对象的基类,即可以lock与unlock的类
    \author
*/
class ISynchronizable
{
public:
    /*!
     \brief wait调用的错误码
     */
    enum {
        E_WAIT_SUCC, //等待信号成功
        E_WAIT_TIMEOUT,  //等待超时
        E_WAIT_INVALID   //对象无效
    };
    
    virtual ~ISynchronizable() {}

    /*!
        \brief          锁定对象
        \return         {void}.无返回值
    */
    virtual void lock() /*throw( const char* )*/ = 0;

    /*!
        \brief          对象解锁
        \return         {void}.无返回值
    */
    virtual void unlock() /*throw( const char* )*/ = 0;

    /*!
        \brief          仿照java提供wait方法,只有拥有同步锁的线程(即代码运行在lock与unclock之间的线程)可以调用wait方法
                        wait可以让调用线程(拥有同步锁)暂时挂起,让出一段时间给其他线程运行
                        其他线程使用完后调用notify发送一个信号(或者wait超时),原来拥有锁的线程将被唤醒,继续执行
        \return         {void}.返回值见 E_WAIT 枚举
    */
    virtual int wait() /*throw( const char* )*/ = 0;

    
    /*!
        \brief          等待millisecs(毫秒),如果没有信号则返回
        \param[in]      millisecs {unsigned long}. 等待的时间
        \return         {void}. 返回值见 E_WAIT 枚举
    */
    virtual int wait(unsigned long millisecs)	/*throw( const char* )*/ = 0;

    /*!
        \brief          向一个此对象的等待者发送一个信号,以唤醒其继续执行,调用此方法前对象必须先被锁定
        \return         {void}.无返回值
    */
    virtual void notify() /*throw( const char* )*/ = 0;

    /*!
        \brief          向所有此对象的等待者发送信号,调用此方法前对象必须先被锁定
        \return         {void}.无返回值
    */
    virtual void notifyAll() /*throw( const char* )*/ = 0;
};

/*!
    \brief     同步锁类定义
    \details   与synchronized一起使用提供类似高级语言的同步机制
    \author
*/
class Lock
{
public:
    /*!
        \brief     构造函数
        \details   构造锁对象
        \param[in]      object {ISynchronizable*}. 同步接口
        \param[in]      intiallyLocked {const bool}. 初始状态是否锁定
        \author
    */
    Lock(ISynchronizable* object, const bool intiallyLocked = true)
    {
        /*	if(!object)
            {
                throw -1;
            }
            try*/
        {
            syncObject = object;
            locked = false;

            if (intiallyLocked)
            {
                lock();
            }
        }
        /*catch( int )
        {

        }*/
    }

    /*!
        \brief     析构函数
        \details   函数析构,会自动解锁
        \author
    */
    virtual ~Lock()
    {
        /*try*/
        {
            if (locked)
            {
                syncObject->unlock();
            }
        }
        /*		catch( int )
                {
                    throw -1;
                }*/
    }

    /*!
        \brief     锁定对象
        \details   无
        \author
    */
    void lock()
    {
        /*try*/
        {
            syncObject->lock();
            locked = true;
        }
        /*	catch( int )
            {
                throw -1;
            }*/
    }

    /*!
        \brief     解锁对象
        \details   无
        \author
    */
    void unlock()
    {
        /*	try*/
        {
            if (locked)
            {
                syncObject->unlock();
                locked = false;
            }
        }
        /*		catch( int )
                {
                    throw -1;
                }*/
    }

    /*!
        \brief     获取对象是否被锁定
        \details   无
        \author
    */
    bool isLocked() const
    {
        return locked;
    }

private:
    bool locked;                    //!< 是否已锁定的标志
    ISynchronizable* syncObject;    //!< 真正进行锁定的对象
};

//! 有异常抛出版本
#define  asl_synchronized(W)									\
	for( /*tbt::*/Lock lock_W(W);						\
	lock_W.isLocked(); lock_W.unlock() )
} // namespace baselib

#endif // __BASE_LIB__MSG_FRW__SYNCHRONIZABLE_H__



