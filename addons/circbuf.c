/*
 * circbuf.c
 *
 *  Created on: Sep 18, 2022
 *      Author: shchakr
 */


#include "circbuf.h"
#include <stdint.h>
#include <stdlib.h>
#include "stm32g0xx_hal.h"

#define BEGIN_CRITICAL()	__disable_irq()
#define END_CRITICAL()		__enable_irq()


/*initialization and allocation of dynamic memory*/
circbuf_enum circbuf_init(circbuf_struct *ptr, uint8_t * buffer, size_t length)
{
    circbuf_enum status;
    if (ptr == NULL )  //check for null and negative length
    {
        status = CIRCBUF_NULL_ERROR;
    }
    else if (length <= 0)
    {
        status = CIRCBUF_LENGTH_ERROR;
    }
    else
    {
        ptr -> buffer = buffer; // allocates the length no of items for the buffer memory
		ptr->size = length;
		ptr->head = ptr->buffer;
		ptr->tail = ptr->buffer;
		ptr->count = 0;
		status = CIRCBUF_SUCCESS;
    }
    return status;
}


/*The functions takes in a pointer to the circular buffer to be destroyed by deallocating using free*/
circbuf_enum circbuf_destroy(circbuf_struct *ptr)
{
    BEGIN_CRITICAL();
    circbuf_enum status;
    if (ptr == NULL) //check for null pointer
    {
        status = CIRCBUF_NULL_ERROR;
    }
    else
    {
        if (ptr ->buffer != NULL) //check if pointer not null
        {
            free(ptr->buffer); //deallocate the memory
            ptr->buffer = NULL; //make pointer null
        }
        ptr->size = 0;
        ptr->head = NULL;
        ptr->tail = NULL;
        ptr->count = 0;
        status = CIRCBUF_SUCCESS;
    }
    END_CRITICAL();
    return status;

}

/*function to add items to the buffer*/
circbuf_enum circbuf_buffer_add_item(circbuf_struct *ptr, uint8_t data_add)
{
    BEGIN_CRITICAL();
    circbuf_enum status = CIRCBUF_SUCCESS;
    if (ptr == NULL || ptr->head == NULL || ptr->tail == NULL || ptr->buffer == NULL) //check for null pointer
    {
        status = CIRCBUF_NULL_ERROR;
    }
    else if (circbuf_is_full(ptr) == 1)
    {
        status = CIRCBUF_FULL_ERROR;
    }
    else if(ptr->head == ptr->buffer + ptr->size -1 ) //wrap around situation, head to wrap around to base when at the end
    {
        *(ptr->head) = data_add;
        ptr->head = ptr->buffer;
        ptr->count++;
        status = CIRCBUF_SUCCESS;
    }
    else
    {// buffer has empty spaces, hence increment the head to the next buffer location
        *ptr->head = data_add;
        ptr->head++;
        ptr->count++;
        status = CIRCBUF_SUCCESS;
    }
    END_CRITICAL();
    return status;

}

/*function to remove items from buffer*/
circbuf_enum circbuf_buffer_remove_item(circbuf_struct *ptr, uint8_t *data_remove)
{
    BEGIN_CRITICAL();
    circbuf_enum status = CIRCBUF_SUCCESS;
    if (ptr == NULL || ptr->head == NULL || ptr->tail == NULL || ptr->buffer == NULL) //check for null pointer
    {
        status = CIRCBUF_NULL_ERROR;
    }

    if (circbuf_is_empty(ptr))
    {
        status = CIRCBUF_EMPTY_ERROR;
    }
    else
    {
        if(ptr->tail == ptr->buffer + ptr->size -1) //wrap around situation, tail to wrap around to base when at the end
        {
            *data_remove = *(ptr->tail);
            *(ptr->tail) = 0;
            ptr->tail = ptr-> buffer;
            ptr->count--;
            status = CIRCBUF_SUCCESS;
        }
        else
        {// buffer has full spaces, hence increment the tail to the next buffer location
            *data_remove = *(ptr->tail);
            *(ptr->tail) = 0;
            ptr->tail++;
            ptr->count--;
            status = CIRCBUF_SUCCESS;
        }
    }
    END_CRITICAL();
    return status;
}

/*function to peek into buffer*/
circbuf_enum circbuf_peek(circbuf_struct *ptr, uint8_t position, uint8_t *data)
{
    circbuf_enum status = CIRCBUF_SUCCESS;
    if (ptr == NULL || ptr->head == NULL || ptr->tail == NULL || ptr ->buffer == NULL || data == NULL) //check for null pointer
    {
        status = CIRCBUF_NULL_ERROR;
    }
    else if (status == CIRCBUF_SUCCESS || status == CIRCBUF_EMPTY_ERROR)
    {
        status = CIRCBUF_EMPTY_ERROR;
    }
    else if (position <= 0 || position > ptr->size)
    {
        status =  CIRCBUF_POSITION_ERROR;
    }

    else
    {
        *data = *( ptr->buffer + position );
        status = CIRCBUF_SUCCESS;
    }
    return status;
}
