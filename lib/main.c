#include <stdio.h>
#include "Chunk.h"
#include "VoxelGraph.h"
#include "HashUtils.h"
#include "VoxelHashMap.h"
#include "VoxelPriorityQueue.h"

int main(){
    printf("%ld\n", sizeof(GraphNode_t));
    printf("%ld\n", sizeof(AltChunk_t));
    VoxelGraph_t* graph = voxel_graph_init(1<<15);
    printf("graph chunk_array_size is %ld \n",(long) graph->chunk_amount);
    
    printf("graph chunk_hash_table_size is %ld \n", (long) graph->chunk_hash_table_size);
    printf("size of base graph is %ld\n", sizeof(*graph) + sizeof((graph->chunk_hash_table[0])) * graph->chunk_hash_table_size);
    printf("hash-test 1: %d\n", build_chunk_hash_table_hash(2354, 123, 4, 032540327));

    uint16_t coords = build_vertex_coords(16, 16, 16);
    printf("coords sanity check %d\n", coords);
    printf("%d\n%d\n%d\n", vertex_pick_x_coord(coords), vertex_pick_y_coord(coords), vertex_pick_z_coord(coords));

    printf("anchor coord sanity test\n");
    printf("%ld\n", build_anchor_coord(32));
    printf("%ld\n", build_anchor_coord(0));
    printf("%ld\n", build_anchor_coord(-1));

    printf("hash-test 2: %d\n", build_node_hash_table_hash(coords, 032540327));
    voxel_graph_insert(graph, 1, 1, 1);
    AltChunk_t* chunk = voxel_graph_chunk_hash_table_lookup(graph, 1, 1, 1);
    assert(graph->current_chunk_index == 1);
    assert(chunk->x_offset == 0);
    assert(chunk->y_offset == 0);
    assert(chunk->z_offset == 0);
    //assert(chunk->current_node_index == 1);
    //assert(chunk->nodes[0].coord_and_mesh_info.vertex_coords != 0);
    assert(alt_chunk_lookup(chunk, 1, 1, 1));
    voxel_graph_insert(graph, 2, 2, 2);
    assert(graph->current_chunk_index == 1);
    assert(chunk->x_offset == 0);
    assert(chunk->y_offset == 0);
    assert(chunk->z_offset == 0);
    assert(chunk->change_occurred == true);
    //assert(chunk->current_node_index == 2);
    //assert(chunk->nodes[1].coord_and_mesh_info.vertex_coords != 0);
    assert(alt_chunk_lookup(chunk, 2, 2, 2));
    voxel_graph_insert(graph, 2, 2, 2);
    assert(graph->current_chunk_index == 1);
    assert(chunk->x_offset == 0);
    assert(chunk->y_offset == 0);
    assert(chunk->z_offset == 0);
    //assert(chunk->current_node_index == 2);

    printf("testing chunk insertion with negative coords\n");
    voxel_graph_insert(graph, 2, -1, 2);
    AltChunk_t* chunk_two = voxel_graph_chunk_hash_table_lookup(graph, 2, -1, 2);
    assert(graph->current_chunk_index == 2);
    assert(chunk_two->x_offset == 0);
    assert(chunk_two->y_offset == -16);
    assert(chunk_two->z_offset == 0);
    //assert(chunk_two->current_node_index == 1);

    printf("depricated test: neighbour connection sanity check:\n");
    voxel_graph_insert(graph, 1, 1, 2);
    assert(graph->current_chunk_index == 2);
    assert(chunk->x_offset == 0);
    assert(chunk->y_offset == 0);
    assert(chunk->z_offset == 0);
    //assert(chunk->current_node_index == 3);
    //assert(chunk->nodes[2].coord_and_mesh_info.buf[2] != 0);
    //assert(chunk->nodes[0].coord_and_mesh_info.buf[2] != 0);
    //for debugging
    voxel_graph_insert(graph, -9, -32, 12);
    voxel_graph_insert(graph,167, 82 ,-1);

    printf("inserting every node in chunk\n");

    for(int x = 32; x < 48; x++){
        for(int y = 32; y < 48; y++){
            for(int z = 32; z < 48; z++){
                printf("inserting %d %d %d\n", x, y, z);
                voxel_graph_insert(graph, x,y,z);
            }
        }
    }
    printf("checking insertion\n");
    for(int x = 32; x < 48; x++){
        for(int y = 32; y < 48; y++){
            for(int z = 32; z < 48; z++){
                assert(voxel_graph_lookup(graph, x,y,z) == 2);
            }
        }
    }
    printf("deleting every node in chunk\n");

    for(int x = 32; x < 48; x++){
        for(int y = 32; y < 48; y++){
            for(int z = 32; z < 48; z++){
                printf("deleting %d %d %d\n", x, y, z);
                voxel_graph_delete(graph, x,y,z);
            }
        }
    }
    printf("checking deletion\n");
    for(int x = 32; x < 48; x++){
        for(int y = 32; y < 48; y++){
            for(int z = 32; z < 48; z++){
                assert(voxel_graph_lookup(graph, x,y,z) == 1);
            }
        }
    }

    voxel_graph_delete(graph, -10, -18, 0);
    voxel_graph_delete(graph, -10, -18, 1);
    voxel_graph_insert(graph, -9, -18, 0);

    //printf("depricated test: neighbour connection and disconnection check:\n");
    //voxel_graph_insert(graph, 1, 1, 2);
    //voxel_graph_insert(graph, 1, 2, 1);
    //voxel_graph_insert(graph, 2, 1, 1);
    //voxel_graph_delete(graph, 1, 1, 1);
    /*
    uint16_t test_coord_center = build_vertex_coords(1, 1, 1);
    uint16_t test_coord_above = build_vertex_coords(1, 1, 2);
    uint16_t test_coord_left = build_vertex_coords(1, 2, 1);
    uint16_t test_coord_ahead = build_vertex_coords(2, 1, 1);

    int64_t test_index_center = chunk_node_lookup(chunk, test_coord_center);
    int64_t test_index_above = chunk_node_lookup(chunk, test_coord_above);
    int64_t test_index_left = chunk_node_lookup(chunk, test_coord_left);
    int64_t test_index_ahead = chunk_node_lookup(chunk, test_coord_ahead);

    assert(chunk->nodes[test_index_center].coord_and_mesh_info.buf[2] == 1);
    assert(!vertex_get_down_bit(&chunk->nodes[test_index_above].coord_and_mesh_info));
    assert(!vertex_get_right_bit(&chunk->nodes[test_index_left].coord_and_mesh_info));
    assert(!vertex_get_back_bit(&chunk->nodes[test_index_ahead].coord_and_mesh_info));
    
    voxel_graph_insert(graph, 1, 1, 1);

    assert(vertex_get_down_bit(&chunk->nodes[test_index_above].coord_and_mesh_info));
    assert(vertex_get_right_bit(&chunk->nodes[test_index_left].coord_and_mesh_info));
    assert(vertex_get_back_bit(&chunk->nodes[test_index_ahead].coord_and_mesh_info));
    
    assert(vertex_get_up_bit(&chunk->nodes[test_index_center].coord_and_mesh_info));
    assert(vertex_get_left_bit(&chunk->nodes[test_index_center].coord_and_mesh_info));
    assert(vertex_get_foward_bit(&chunk->nodes[test_index_center].coord_and_mesh_info));
    assert(!vertex_get_dead_bit(&chunk->nodes[test_index_center].coord_and_mesh_info));

    voxel_graph_insert(graph, 31, 0, 0);
    voxel_graph_insert(graph, 32, 0, 0);
    AltChunk_t* chunk_three = voxel_graph_chunk_hash_table_lookup(graph, 32, 0, 0);
    uint16_t test_chunk_border = build_vertex_coords(31, 0, 0);
    uint16_t test_chunk_border_2 = build_vertex_coords(0, 0, 0);
    int64_t chunk_node_index_2 = chunk_node_lookup(chunk_three, test_chunk_border_2);
    int64_t chunk_node_index_1 = chunk_node_lookup(chunk, test_chunk_border);

    assert(vertex_get_foward_bit(&chunk->nodes[chunk_node_index_1].coord_and_mesh_info));
    assert(vertex_get_back_bit(&chunk_three->nodes[chunk_node_index_2].coord_and_mesh_info));

    voxel_graph_delete(graph, 31, 0, 0);

    assert(!vertex_get_foward_bit(&chunk->nodes[chunk_node_index_1].coord_and_mesh_info));
    assert(!vertex_get_back_bit(&chunk_three->nodes[chunk_node_index_2].coord_and_mesh_info));
    */
    printf("performing inflation tests\n");
    voxel_graph_build_inflation(graph, 3, 3);
    assert(chunk->change_occurred == false);
    assert(voxel_graph_lookup_inflation(graph,1,1,1));
    assert(voxel_graph_lookup_inflation(graph,0,0,0));
    assert(voxel_graph_lookup_inflation(graph,2,2,2));
    assert(!voxel_graph_lookup_inflation(graph,1,1,3));
    printf("inflation tests passed!\n");
    printf("checking hashing metrics logic\n");
    assert(graph->total_hash_table_insertions != graph->total_hash_collisions);
    printf("%ld insertions have been performed, %ld hash collosions have occurred\n", graph->total_hash_table_insertions, graph->total_hash_collisions);
    voxel_graph_free(&graph);
    printf("all voxelgraph tests passed\n");

    printf("initiaing hashmap tests:\n");
    VoxelHashMap_t* hashmap = voxel_hash_map_init(2, 10, 0.5f);
    assert(hashmap != NULL);
    assert(hashmap->capacity == 2);
    PointSlot_t* sanity_check = voxel_hash_map_insert(hashmap, 1, 2, 3);
    assert(sanity_check != NULL);
    assert(sanity_check->state == SLOT_OCCUPIED);
    assert(sanity_check->key.x == 1);
    assert(sanity_check->key.y == 2);
    assert(sanity_check->key.z == 3);
    PointSlot_t* lookup_test = voxel_hash_map_lookup(hashmap, 1, 2, 3);
    assert(lookup_test == sanity_check);

    PointSlot_t* resize_sanity_check = voxel_hash_map_insert(hashmap, 2, 3, 4);
    assert(hashmap->capacity == 4);
    int64_t slots_occupied = hashmap->occupied_slot_count;
    PointSlot_t* resize_sanity_duplicate_insertion = voxel_hash_map_insert(hashmap, 1, 2, 3);
    assert(resize_sanity_duplicate_insertion != NULL);
    assert(hashmap->occupied_slot_count == slots_occupied);
    assert(voxel_hash_map_remove(hashmap, 1, 2, 3));
    int64_t dead_slots = 1;
    assert(slots_occupied - 1 == hashmap->occupied_slot_count);
    slots_occupied--;
    assert(dead_slots == hashmap->tombstome_count);
    assert(!voxel_hash_map_remove(hashmap, 1, 2, 3));
    assert(slots_occupied == hashmap->occupied_slot_count);
    assert(dead_slots == hashmap->tombstome_count);
    voxel_hash_map_insert(hashmap, 1, 2, 3);
    assert(slots_occupied + 1 == hashmap->occupied_slot_count);
    slots_occupied++;
    assert(dead_slots - 1 == hashmap->tombstome_count);
    dead_slots--;
    PointSlot_t* slot1 = voxel_hash_map_insert(hashmap, 1, 2, 3);
    slot1->astar_heuristic = 1.0;
    slot1->traveled_dist = 2.0;
    PointSlot_t* slot2 = voxel_hash_map_insert(hashmap, 2, 2, 3);
    slot2->astar_heuristic = 2.0;
    slot2->traveled_dist = 2.0;
    PointSlot_t* slot3 = voxel_hash_map_insert(hashmap, 3, 2, 3);
    slot3->astar_heuristic = 1.0;
    slot3->traveled_dist = 0;
    PointSlot_t* slot4 = voxel_hash_map_insert(hashmap, 4, 2, 3);
    slot4->astar_heuristic = 4.0;
    slot4->traveled_dist = 2.0;
    VoxelPriorityQueue_t* prio_queue = voxel_priority_queue_init(3);
    voxel_priority_queue_enqueue(prio_queue, slot1);
    voxel_priority_queue_enqueue(prio_queue, slot2);
    voxel_priority_queue_enqueue(prio_queue, slot3);
    voxel_priority_queue_enqueue(prio_queue, slot4);
    assert(voxel_priority_queue_peek(prio_queue) == slot3);
    voxel_priority_queue_free(prio_queue);
    voxel_hash_map_free(hashmap);
    printf("all hash map tests passed\n");
    return 0;
}
