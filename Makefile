all:
	g++ motion_segmentation.cpp find_motion_blocks.cpp place_boxes.cpp estimate_motion_vectors.cpp visualize_motion_vectors.cpp -o motion_segmentation `pkg-config --cflags --libs opencv4`

clean:
	rm -rf motiom_segmentation

.PHONY: all
