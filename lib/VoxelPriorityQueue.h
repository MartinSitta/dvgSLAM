#ifndef VOXELPRIORITYQUEUE_H
#define VOXELPRIORITYQUEUE_H

#ifdef __cplusplus
extern "C"{
#endif
#include "PointSlot.h"
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
typedef struct VoxelPriorityQueue_t{
    PointSlot_t** array;
    int64_t capacity;
    int64_t current_element;
}VoxelPriorityQueue_t;

VoxelPriorityQueue_t* voxel_priority_queue_init(int64_t capacity);
void voxel_priority_queue_free(VoxelPriorityQueue_t* queue);
void voxel_priority_queue_enqueue(VoxelPriorityQueue_t*, PointSlot_t* point);
PointSlot_t* voxel_priority_queue_dequeue(VoxelPriorityQueue_t*);
PointSlot_t* voxel_priority_queue_peek(VoxelPriorityQueue_t*);


#ifdef __cplusplus
};
#endif



#endif