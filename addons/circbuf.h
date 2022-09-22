/*
 * circbuf.h
 *
 *  Created on: Sep 18, 2022
 *      Author: shchakr
 */

#ifndef CIRCBUF_CIRCBUF_H_
#define CIRCBUF_CIRCBUF_H_

#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Structure for maintaining handle to circular buffer.
 * Only supports items of 1-byte size.
 * Items added to position after head, and removed from tail.
 */
typedef struct
{
    uint8_t* buffer; // Pointer to buffer in memory
    size_t size; // Total size of buffer
    size_t count; // Number of items in buffer
    uint8_t* head; // Pointer to newest item added
    uint8_t* tail; // Pointer to oldest item added
}circbuf_struct;

/**
 * @brief Status return codes for circular buffer functions
 */

typedef enum
{
    CIRCBUF_SUCCESS, // no error
    CIRCBUF_NULL_ERROR, // if any pointer parameter is null
    CIRCBUF_LENGTH_ERROR, // if attempting to initialize with zero length
    CIRCBUF_FULL_ERROR, // if attempting to add to full buffer
    CIRCBUF_EMPTY_ERROR, // if attempting to remove from empty buffer
    CIRCBUF_POSITION_ERROR, // if attempting to peek at invalid position
    CIRCBUF_ALLOCATION_FAILURE, // if malloc fails
}circbuf_enum;


/**
 * @brief Initialize circular buffer with 'length' bytes
 * Allocates dynamic memory, and populates struct.
 *
 * @param ptr Pointer to circular buf struct handle.
 * @param length Number of bytes to allocate in buffer
 *
 * @return CIRCBUF status code
 */
circbuf_enum circbuf_init(circbuf_struct *ptr, uint8_t * buffer, size_t length);

/**
 * @brief Free circular buffer dynamic memory and reset struct fields
 *
 * @param ptr Pointer to circular buf struct handle
 *
 * @return CIRCBUF status code
 */
circbuf_enum circbuf_destroy(circbuf_struct *ptr);

/**
 * @brief Adds byte to circular buffer after head.
 * If buffer is full, will not add item, and instead returns
 * CIRCBUF_FULL_ERROR.
 *
 * @param ptr Pointer to circular buf struct handle
 * @param data_add Byte to add to buffer
 *
 * @return CIRCBUF status code
 */
circbuf_enum circbuf_buffer_add_item(circbuf_struct *ptr, uint8_t data_add);

/**
 * @brief Removes byte from tail of circular buffer.
 * If buffer is empty, will not populate data_remove pointer,
 * and will return CIRCBUF_EMPTY_ERROR.
 *
 * @param ptr Pointer to circular buf struct handle
 * @param data_remove Pointer of where to write data removed.
 *
 * @return CIRCBUF status code
 */
circbuf_enum circbuf_buffer_remove_item(circbuf_struct *ptr, uint8_t *data_remove);

/**
 * @brief Checks if circular buffer is full
 *
 * @param ptr Pointer to circular buf struct handle
 *
 * @return 1 if full, 0 if not full
 * Todo - what should we return if ptr is null?
 */
static inline int circbuf_is_full(circbuf_struct *ptr)
{
    if (ptr == NULL || ptr->head == NULL || ptr->tail == NULL || ptr->buffer == NULL) //check for null pointer
    {
        return -1; // Evaluates to True / Full
    }
    else if ((ptr->tail == ptr->head + 1) || (ptr->count == ptr->size)) // tail is 1 position ahead of header, buffer is full
    //else if (0)
    {
        return 1; // Full
    }
    return 0; // Not Full
}


/**
 * @brief Checks if circular buffer is empty
 *
 * @param ptr Pointer to circular buf struct handle
 *
 * @return 1 if empty, 0 if not empty
 */
static inline int circbuf_is_empty(circbuf_struct *ptr)
{
    if (ptr == NULL || ptr->head == NULL || ptr->tail == NULL || ptr->buffer == NULL) //check for null pointer
    {
        return -1; // Evaluates to True / Empty
    }
    else if((ptr->count == 0) && (ptr->tail == ptr->head)) //current item count in the buffer is 0 if buffer is empty
    {
        return 1; // Empty
    }
    return 0; // Not empty
}


/**
 * @brief Shows an item at any position in circular buffer
 * without removing item from buffer. If position is not populated,
 * will return CIRCBUF_POSITION_ERROR.
 *
 * @param ptr Pointer to circular buf struct handle
 * @param position Index from head of where to peek (head is index 0)
 * @param holder Pointer of where to write peeked item.
 *
 * @return CIRCBUF status code
 */
circbuf_enum circbuf_peek(circbuf_struct *ptr, uint8_t position, uint8_t *data);


#endif /* CIRCBUF_CIRCBUF_H_ */
