function output_img = placeBoxes( img, binary_map, color )
  [img_size_y, img_size_x, colors] = size( img );
  clear( 'colors' );
  block_size = size( img(:,:,1) ) ./ size( binary_map );
  block_size_y = block_size(1);
  block_size_x = block_size(2);
  map_y = 1;
  for y = 1 : block_size_y : img_size_y
    map_x = 1;
    for x = 1 : block_size_x : img_size_x
      patch = img(y : y + block_size_y - 1,x : x + block_size_x - 1,:);
      if binary_map( map_y, map_x )
        output_img(y : y + block_size_y - 1,x : x + block_size_x - 1,:) = drawOutline( patch, color );
      else
        output_img(y : y + block_size_y - 1,x : x + block_size_x - 1,:) = patch;
      end
      map_x = map_x + 1;
    end
    map_y = map_y + 1;
  end
end
