#include "thImageList.h"

/* Defines */
#define CLASS_NAME TEXT("thImageList")
#define MAX_NUMBER_OF_DIGITS_FOR_INDEX 6

/* Static elements */
uint32_t thImageList::u32Index = 1;

/* defines */
#define DEFAULT_IMAGE_WIDTH     24
#define DEFAULT_IMAGE_HEIGHT    24

thImageList::thImageList(void) : m_hListHandle(NULL)
{
    TH_ENTER_FUNCTION;
    this->m_hListHandle = ImageList_Create(DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT, ILC_MASK | ILC_COLOR32, 4, 4);

    if (NULL != m_hListHandle) {
        /* Create Default thImageList Name */
        TCHAR szNameIndex[MAX_NUMBER_OF_DIGITS_FOR_INDEX] = { 0 }; // up to 5 digtits

        if (-1 == _stprintf_s(szNameIndex, MAX_NUMBER_OF_DIGITS_FOR_INDEX, TEXT("%d"), u32Index)) {
            MSG_ERROR(TEXT("CRITICAL_ERROR - window index buffer overflow"));
        }
        else {
            this->m_name += thString(szNameIndex);
        }
        u32Index++;
        MSG_SUCCESS(TEXT("Created new Image List: %s with ID=%d"), this->m_name.c_str(), this->m_id);
    }
    else {
        MSG_ERROR(TEXT("ImageList_Create failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thImageList::~thImageList(void)
{
    TH_ENTER_FUNCTION;

    if (m_hListHandle) {
        BOOL fResult = FALSE;
        
        fResult = ImageList_Destroy(this->m_hListHandle);

        if (FALSE == fResult) {
            MSG_ERROR(TEXT("ImageList_Destroy failed with error = 0x%X"), GetLastError());
        }
    }

    TH_LEAVE_FUNCTION;
}
