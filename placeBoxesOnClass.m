function output_img = placeBoxesOnClass( img, class_num, classes, color )
  binary_map = zeros( size( classes ), 'logical' );
  block_size = size( img(:,:,1) ) ./ size( binary_map );
  block_size_y = block_size(1);
  block_size_x = block_size(2);
  [y_size, x_size] = size( classes );
  x_block_start = inf;
  y_block_start = 0;
  x_block_end = 0;
  y_block_end = 0;
  start_found = 0;
  for y = 1 : 1 : y_size
    for x = 1 : 1 : x_size
      if classes(y,x) == class_num
        binary_map(y,x) = 1;
        if ~start_found
          start_found = 1;
          y_block_start = y;
        end
        if x < x_block_start
          x_block_start = x;
        end
        if x > x_block_end
          x_block_end = x;
        end
        y_block_end = y;
      end
    end
  end
  x_start = ( x_block_start - 1 ) * block_size_x + 1;
  x_end   = x_block_end * block_size_x;
  y_start = ( y_block_start - 1 ) * block_size_y + 1;
  y_end   = y_block_end * block_size_y;
  patch = img(y_start : y_end,x_start : x_end,:);
  img(y_start : y_end, x_start : x_end, :) = drawOutline(patch, color);
  output_img = img;
end