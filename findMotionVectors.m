function findMotionVectors( frame_0, frame_1, block_map )
  [block_map_size_y, block_map_size_x] = size( block_map );
  block_size = size( img ) ./ size( binary_map );
  block_size_y = block_size(1);
  block_size_x = block_size(2);
  for y = 2 : 1 : block_map_size_y - 1
    for x = 2 : 1 : block_map_size_x - 1
      if block_map(y,x)
      else
      end
    end
  end
end