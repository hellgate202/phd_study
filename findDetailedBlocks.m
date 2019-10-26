function output = findDetailedBlocks( img, block, threshold )
  [img_size_y, img_size_x] = size( img );
  output = zeros( img_size_y / block, img_size_x / block, 'logical' );
  output_y = 1;
  for y = 1 : block : img_size_y
    output_x = 1;
    for x = 1 : block : img_size_x
      patch = img(y : y + block - 1,x : x + block - 1);
      if sum( patch, 'all' ) / ( block ^ 2 ) >= threshold
        output(output_y,output_x) = 1;
      end
      output_x = output_x + 1;
    end
    output_y = output_y + 1;
  end
end