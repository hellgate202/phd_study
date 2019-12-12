function [vec_length, vec_angle, motion_blocks] = findMotionVectors( frame_0, frame_1, block_map, min_length, MSE_THRESHOLD )
  [block_map_size_y, block_map_size_x] = size( block_map );
  block_size = size( frame_0 ) ./ size( block_map );
  block_size_y = block_size(1);
  block_size_x = block_size(2);
  vec_length = zeros( size( block_map ), 'uint16' );
  vec_angle = zeros( size( block_map ), 'double' );
  motion_blocks = zeros( size( block_map), 'logical' );
  for y = 2 : 1 : block_map_size_y - 1
    for x = 2 : 1 : block_map_size_x - 1
      if block_map(y,x)
        patch_y = ( y - 1 ) * block_size_y + 1;
        patch_x = ( x - 1 ) * block_size_x + 1;
        search_area_y = ( y - 2 ) * block_size_y + 1;
        search_area_x = ( x - 2 ) * block_size_x + 1;
        patch = frame_0(patch_y : patch_y + block_size_y - 1,...
                        patch_x : patch_x + block_size_x - 1);
        search_area = frame_1(search_area_y : search_area_y + block_size_y * 3 - 1,...
                              search_area_x : search_area_x + block_size_x * 3 - 1);
        [new_pos_y, new_pos_x] = findPatch( patch, search_area, MSE_THRESHOLD );
        length = sqrt( ( new_pos_y - block_size_y - 1 ) ^ 2 +...
                       ( new_pos_x - block_size_x - 1 ) ^ 2 );
        if length > min_length
          angle = findAngle( new_pos_y - block_size_y - 1, new_pos_x - block_size_x - 1 );
          vec_length(y,x) = length;
          vec_angle(y,x) = angle;
          motion_blocks(y,x) = 1;
        else
          vec_length(y,x) = 0;
          vec_angle(y,x) = 0;
        end
      end
    end
  end
end