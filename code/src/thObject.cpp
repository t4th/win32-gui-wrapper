#include "thObject.h"


DWORD thObject::m_idPool = ID_POOL_START;

thObject::thObject()
{
    TH_ENTER_FUNCTION;

    this->m_id = m_idPool;

    if (m_idPool >= (WORD)0xFFFFU)
    {
        // out of ID pool
        // todo: throw error
    }
    else
    {
        m_idPool++;
    }

    TH_LEAVE_FUNCTION;
}


thObject::~thObject()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}