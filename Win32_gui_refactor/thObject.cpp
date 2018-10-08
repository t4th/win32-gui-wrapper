#include "thObject.h"

#define ID_POOL_START   (int)100
#define NO_MESSAGE      (uint32_t)-2333

int thObject::m_idPool = ID_POOL_START;

thObject::thObject()
{
    TH_ENTER_FUNCTION;

    this->m_id = reinterpret_cast<HMENU>(m_idPool);
    m_idPool += 1;

    TH_LEAVE_FUNCTION;
}


thObject::~thObject()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}