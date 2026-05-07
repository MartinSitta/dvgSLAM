#include "VoxelPriorityQueue.h"
//NOTE: THE IMPLEMENTATION IS PRETTY SIMILAR TO THE IMPLEMENTATION OF GEEKSFORGEEKS
//KEY DIFFERENCE IS I PLAN TO RESIZE THE ARRAY AS I GO
//helpers
static inline void swap(PointSlot_t** min_heap, int64_t index_1, int64_t index_2){
    PointSlot_t* temp_ptr = min_heap[index_2];
    min_heap[index_2] = min_heap[index_1];
    min_heap[index_1] = temp_ptr;
}
static inline void heapify_up(PointSlot_t** heap, int64_t index){
    if(index && heap[(index - 1) / 2]->astar_heuristic + heap[(index -1) / 2]->traveled_dist >
        heap[index]->astar_heuristic + heap[index]->traveled_dist){
        swap(heap, (index - 1) / 2, index);
        heapify_up(heap, (index - 1) / 2);
    }
}
static inline void heapify_down(PointSlot_t** heap, int64_t index, int64_t capacity){
    int64_t root = index;
    int64_t left_sub_tree = 2 * index + 1;
    int64_t right_sub_tree = 2 * index + 2;
    int64_t smallest_index = index;
    if(left_sub_tree < capacity &&
        heap[left_sub_tree]->astar_heuristic + heap[left_sub_tree]->traveled_dist <
        heap[smallest_index]->astar_heuristic + heap[smallest_index]->traveled_dist){
        smallest_index = left_sub_tree;
    }
    if(right_sub_tree < capacity &&
        heap[right_sub_tree]->astar_heuristic + heap[right_sub_tree]->traveled_dist <
        heap[smallest_index]->astar_heuristic + heap[smallest_index]->traveled_dist){
        smallest_index = right_sub_tree;
    }
    if(smallest_index == root){
        swap(heap, root, smallest_index);
        heapify_down(heap, smallest_index, capacity);
    }

}

void resize(VoxelPriorityQueue_t* queue){
    PointSlot_t** new_array = malloc(sizeof(PointSlot_t*) * queue->capacity * 2);
    for(int64_t cnt = 0; cnt < queue->capacity; cnt++){
        new_array[cnt] = queue->array[cnt];
    }
    for(int64_t cnt = queue->capacity; cnt < queue->capacity * 2; cnt++){
        new_array[cnt] = NULL;
    }
    queue->capacity = queue->capacity * 2;
    PointSlot_t** old_array = queue->array;
    queue->array = new_array;
    free(old_array);
}




//end helpers

VoxelPriorityQueue_t* voxel_priority_queue_init(int64_t capacity){
    VoxelPriorityQueue_t* output = malloc(sizeof(VoxelPriorityQueue_t));
    output->capacity = capacity;
    output->current_element = 0;
    output->array = malloc(sizeof(PointSlot_t*) * output->capacity);
    if(output->array == NULL){
        return NULL;
    }
    for(int64_t cnt = 0; cnt < capacity; cnt++){

    }
    return output;
}

void voxel_priority_queue_free(VoxelPriorityQueue_t* queue){
    free(queue->array);
    free(queue);
}

void voxel_priority_queue_enqueue(VoxelPriorityQueue_t* queue, PointSlot_t* slot){
    if(queue->current_element >= queue->capacity){
        resize(queue);
        return;
        //TODO: IMPLEMENT A ARRAY RESIZE OPERATION!
    }
    queue->array[queue->current_element] = slot;
    heapify_up(queue->array, queue->current_element);
    queue->current_element++;
}
PointSlot_t* voxel_priority_queue_dequeue(VoxelPriorityQueue_t* queue){
    PointSlot_t* output = queue->array[0];
    queue->array[0] = queue->array[queue->current_element];
    queue->array[queue->current_element] = NULL;
    queue->current_element--;
    heapify_down(queue->array, 0, queue->capacity);
    return output;
}
PointSlot_t* voxel_priority_queue_peek(VoxelPriorityQueue_t* queue){
    return queue->array[0];
}