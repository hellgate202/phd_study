all:
	g++ motion_segmentation.cpp find_motion_blocks.cpp place_boxes.cpp -o motion_segmentation `pkg-config --cflags --libs opencv4`

clean:
	rm -rf motiom_segmentation

.PHONY: all
